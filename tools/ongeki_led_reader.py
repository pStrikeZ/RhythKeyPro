r"""
ongeki_led_reader.py - 音击灯效同步工具
从 \\.\pipe\ongeki_led 读取 Board 1 的 6 个按键灯颜色，
编码为 base-5+1 格式，通过 XInput 震动通道推送到 RhythKey Pro 控制器。

用法：
    python ongeki_led_reader.py              # 正常模式：读 pipe + 推送震动
    python ongeki_led_reader.py --monitor    # 只读模式：只打印，不发送震动
    python ongeki_led_reader.py --test-rumble # 测试模式：发送已知颜色验证固件解码
    python ongeki_led_reader.py --player N   # 指定 XInput player index（默认自动检测）
"""

import sys
import os

if os.name != "nt":
    print("[!] 本工具仅支持 Windows（依赖 Named Pipe 和 XInput API）")
    sys.exit(1)

import time
import ctypes
import ctypes.wintypes
import subprocess
import json
import argparse
import atexit
import threading

# ============================================================
# 常量定义
# ============================================================

PIPE_PATH = r"\\.\pipe\ongeki_led"
SYNC_BYTE = 0xE0
ESCAPE_BYTE = 0xD0
BOARD1_LED_COUNT = 6
BOARD0_LED_COUNT = 61
PIPE_READ_TIMEOUT = 3  # 秒，连接后无数据则重连

# 颜色枚举（与固件 RUMBLE_COLOR_TABLE 一致）
COLOR_OFF    = 0
COLOR_RED    = 1
COLOR_GREEN  = 2
COLOR_BLUE   = 3
COLOR_YELLOW = 4

# RGB → 颜色枚举映射
COLOR_NAMES = {
    COLOR_OFF:    "off",
    COLOR_RED:    "red",
    COLOR_GREEN:  "green",
    COLOR_BLUE:   "blue",
    COLOR_YELLOW: "yellow",
}

# 终端彩色输出
COLOR_SYMBOLS = {
    "off":    "\033[90m○\033[0m",
    "red":    "\033[91m●\033[0m",
    "yellow": "\033[93m●\033[0m",
    "blue":   "\033[94m●\033[0m",
    "green":  "\033[92m●\033[0m",
}

ARDUINO_SERIAL = "ARDUINO_XINPUT_AVR"

# ============================================================
# XInput API (ctypes)
# ============================================================

class XINPUT_VIBRATION(ctypes.Structure):
    _fields_ = [
        ("wLeftMotorSpeed", ctypes.wintypes.WORD),
        ("wRightMotorSpeed", ctypes.wintypes.WORD),
    ]

class XINPUT_STATE(ctypes.Structure):
    class XINPUT_GAMEPAD(ctypes.Structure):
        _fields_ = [
            ("wButtons", ctypes.wintypes.WORD),
            ("bLeftTrigger", ctypes.c_ubyte),
            ("bRightTrigger", ctypes.c_ubyte),
            ("sThumbLX", ctypes.c_short),
            ("sThumbLY", ctypes.c_short),
            ("sThumbRX", ctypes.c_short),
            ("sThumbRY", ctypes.c_short),
        ]
    _fields_ = [
        ("dwPacketNumber", ctypes.wintypes.DWORD),
        ("Gamepad", XINPUT_GAMEPAD),
    ]


def load_xinput():
    """加载 XInput DLL"""
    for dll_name in ["xinput1_4", "xinput1_3", "xinput9_1_0"]:
        try:
            return ctypes.windll.LoadLibrary(dll_name)
        except OSError:
            continue
    return None


def xinput_set_state(xinput_dll, player_index, left, right):
    """设置震动状态。left/right 为 0–125 的协议值。
    XInputSetState 接受 WORD (0–65535)，XUSB 驱动会 >> 8 转为 uint8_t 发给控制器，
    所以需要左移 8 位才能让控制器收到正确的值。"""
    vib = XINPUT_VIBRATION()
    vib.wLeftMotorSpeed = (left & 0xFF) << 8
    vib.wRightMotorSpeed = (right & 0xFF) << 8
    return xinput_dll.XInputSetState(player_index, ctypes.byref(vib))


def xinput_get_state(xinput_dll, player_index):
    """获取控制器状态（用于检测连接）"""
    state = XINPUT_STATE()
    result = xinput_dll.XInputGetState(player_index, ctypes.byref(state))
    return result == 0  # ERROR_SUCCESS

# ============================================================
# 设备检测
# ============================================================

def check_arduino_connected():
    """检查 ARDUINO_XINPUT_AVR 是否通过 USB 连接"""
    ps_script = r"""
    Get-PnpDevice -Status 'OK' -ErrorAction SilentlyContinue |
        Where-Object { $_.InstanceId -like '*ARDUINO_XINPUT_AVR*' } |
        Select-Object InstanceId, FriendlyName |
        ConvertTo-Json -Depth 2
    """
    try:
        result = subprocess.run(
            ["powershell", "-NoProfile", "-Command", ps_script],
            capture_output=True, text=True, timeout=10
        )
        if result.stdout.strip():
            devices = json.loads(result.stdout)
            if not isinstance(devices, list):
                devices = [devices]
            return len(devices) > 0
    except Exception:
        pass
    return False


def find_player_index(xinput_dll):
    """自动检测 RhythKey Pro 的 XInput player index"""
    # 先确认 Arduino 设备在线
    if not check_arduino_connected():
        print("  [!] 未检测到 ARDUINO_XINPUT_AVR 设备")
        print("      请确认控制器已通过 USB 连接")
        return None

    # 扫描 XInput 0-3
    connected = []
    for i in range(4):
        if xinput_get_state(xinput_dll, i):
            connected.append(i)

    if len(connected) == 0:
        print("  [!] 没有检测到任何 XInput 控制器")
        return None
    elif len(connected) == 1:
        print(f"  [✓] 检测到控制器: Player {connected[0]}")
        return connected[0]
    else:
        print(f"  [!] 检测到多个 XInput 控制器: {connected}")
        print("      请使用 --player N 参数指定")
        return None

# ============================================================
# 颜色识别 & 编码
# ============================================================

def classify_rgb(r, g, b):
    """将 RGB 值分类为颜色枚举"""
    if max(r, g, b) < 30:
        return COLOR_OFF
    if r > 150 and g > 150 and b < 100:
        return COLOR_YELLOW
    if r > 100 and g < 80 and b < 80:
        return COLOR_RED
    if g > 100 and r < 80 and b < 80:
        return COLOR_GREEN
    if b > 100 and r < 80 and g < 80:
        return COLOR_BLUE
    # 未知颜色，当作熄灭
    return COLOR_OFF


def encode_base5(c0, c1, c2):
    """将 3 个颜色枚举编码为 base-5+1 值（1–125）"""
    return c0 * 25 + c1 * 5 + c2 + 1

# ============================================================
# Pipe 读取（复用已验证的实现）
# ============================================================

def read_byte(pipe):
    """从 pipe 读取一个字节"""
    data = pipe.read(1)
    if not data:
        raise EOFError("Pipe closed")
    return data[0]


def read_unescaped_byte(pipe):
    """读取一个 JVS 转义处理后的字节"""
    b = read_byte(pipe)
    if b == SYNC_BYTE:
        return None, True
    if b == ESCAPE_BYTE:
        next_b = read_byte(pipe)
        return (next_b + 1) & 0xFF, False
    return b, False


def read_frame(pipe):
    """读取一个完整的 JVS 帧，返回 (board_id, rgb_bytes) 或 None"""
    while True:
        b = read_byte(pipe)
        if b == SYNC_BYTE:
            break

    board_id, is_sync = read_unescaped_byte(pipe)
    if is_sync or board_id is None:
        return None

    if board_id == 0:
        led_count = BOARD0_LED_COUNT
    elif board_id == 1:
        led_count = BOARD1_LED_COUNT
    else:
        return None

    rgb_data = []
    for _ in range(led_count * 3):
        val, is_sync = read_unescaped_byte(pipe)
        if is_sync or val is None:
            return None
        rgb_data.append(val)

    return board_id, bytes(rgb_data)

# ============================================================
# 显示格式化
# ============================================================

def format_leds(colors_enum):
    """格式化 6 个颜色枚举为可视化字符串"""
    parts = [COLOR_SYMBOLS.get(COLOR_NAMES[c], "?") for c in colors_enum]
    return " ".join(parts[:3]) + "  |  " + " ".join(parts[3:])

# ============================================================
# 主逻辑
# ============================================================

def run_test_rumble(xinput_dll, player_index):
    """测试模式：依次发送已知颜色组合验证固件解码"""
    test_cases = [
        # (描述, 左3色, 右3色)
        ("全红",   [COLOR_RED]*3,    [COLOR_RED]*3),
        ("全绿",   [COLOR_GREEN]*3,  [COLOR_GREEN]*3),
        ("全蓝",   [COLOR_BLUE]*3,   [COLOR_BLUE]*3),
        ("全黄",   [COLOR_YELLOW]*3, [COLOR_YELLOW]*3),
        ("左红右蓝", [COLOR_RED]*3,   [COLOR_BLUE]*3),
        ("彩虹",   [COLOR_RED, COLOR_GREEN, COLOR_BLUE],
                   [COLOR_YELLOW, COLOR_RED, COLOR_GREEN]),
        ("全灭",   [COLOR_OFF]*3,    [COLOR_OFF]*3),
    ]

    print(f"\n  测试震动编码 → Player {player_index}")
    print("  每组持续 2 秒，请观察控制器 LED")
    print("-" * 40)

    for desc, left_colors, right_colors in test_cases:
        left_val = encode_base5(*left_colors)
        right_val = encode_base5(*right_colors)

        left_names = [COLOR_NAMES[c] for c in left_colors]
        right_names = [COLOR_NAMES[c] for c in right_colors]

        print(f"\n  {desc}: L={left_names} R={right_names}")
        print(f"    编码: left={left_val} right={right_val}")

        xinput_set_state(xinput_dll, player_index, left_val, right_val)
        time.sleep(2)

    # 清除信号
    xinput_set_state(xinput_dll, player_index, 0, 0)
    print("\n  测试完成，已清除震动信号")


def run_wrapper(xinput_dll, player_index, monitor_only=False):
    """主运行循环：读 Pipe → 编码 → 发送震动"""
    mode_label = "只读" if monitor_only else "同步"

    print("=" * 60)
    print("  音击灯效同步工具")
    print("=" * 60)
    print(f"  模式: {mode_label}")
    if not monitor_only:
        print(f"  目标: Player {player_index}")
    print(f"  Pipe: {PIPE_PATH}")
    print()
    print("  等待 Pipe 连接... (请先启动音击)")
    print("  按 Ctrl+C 退出")
    print("-" * 60)

    # 用于主线程等待退出信号
    stop_event = threading.Event()
    frame_count = [0]  # 用列表以便线程内可修改

    def reader_thread():
        """后台线程：读 Pipe → 编码 → 发送震动"""
        prev_colors = None

        while not stop_event.is_set():
            try:
                with open(PIPE_PATH, "rb") as pipe:
                    print(f"\n  [✓] 已连接到 Pipe!")
                    print()

                    # 看门狗：连接后 N 秒无数据则关闭 pipe 重连
                    got_data = threading.Event()

                    def watchdog():
                        if not got_data.wait(PIPE_READ_TIMEOUT):
                            if not stop_event.is_set():
                                print("\n  [!] 连接后无数据，重连...")
                            try:
                                pipe.close()
                            except Exception:
                                pass

                    wd = threading.Thread(target=watchdog, daemon=True)
                    wd.start()

                    while not stop_event.is_set():
                        result = read_frame(pipe)
                        if result is None:
                            continue

                        board_id, rgb_data = result

                        # 收到有效帧，取消看门狗
                        if not got_data.is_set():
                            got_data.set()

                        # 只处理 Board 1
                        if board_id != 1:
                            continue

                        frame_count[0] += 1

                        # 分类颜色
                        colors = []
                        for i in range(BOARD1_LED_COUNT):
                            r, g, b = rgb_data[i*3], rgb_data[i*3+1], rgb_data[i*3+2]
                            colors.append(classify_rgb(r, g, b))

                        # 仅在变化时处理
                        if colors == prev_colors:
                            continue

                        # 编码
                        left_val = encode_base5(colors[0], colors[1], colors[2])
                        right_val = encode_base5(colors[3], colors[4], colors[5])

                        # 打印
                        timestamp = time.strftime("%H:%M:%S")
                        visual = format_leds(colors)
                        print(f"[{timestamp}] #{frame_count[0]:4d}  {visual}  → L={left_val:3d} R={right_val:3d}")

                        # 发送震动
                        if not monitor_only:
                            try:
                                xinput_set_state(xinput_dll, player_index, left_val, right_val)
                            except Exception:
                                pass  # 控制器可能已断开

                        prev_colors = colors

            except FileNotFoundError:
                if not stop_event.is_set():
                    sys.stdout.write("\r  等待 Pipe... (音击尚未启动)")
                    sys.stdout.flush()
                    # 用 stop_event.wait 代替 time.sleep，可立即响应退出
                    stop_event.wait(1)
            except (EOFError, ValueError, OSError):
                if not stop_event.is_set():
                    print("\n  [!] Pipe 断开，尝试重连...")
                    stop_event.wait(1)

    # 启动读取线程
    thread = threading.Thread(target=reader_thread, daemon=True)
    thread.start()

    # 主线程：等待 Ctrl+C（Event.wait 在 Windows 上可被 SIGINT 中断）
    try:
        while not stop_event.is_set():
            stop_event.wait(0.5)
    except KeyboardInterrupt:
        pass
    finally:
        stop_event.set()
        # 清除震动
        if not monitor_only and xinput_dll:
            try:
                xinput_set_state(xinput_dll, player_index, 0, 0)
            except Exception:
                pass
        print(f"\n\n  总计处理: {frame_count[0]} 帧")
        print("  已退出。")


def main():
    parser = argparse.ArgumentParser(
        description="音击灯效同步工具 - 从 named pipe 读取 LED 数据并推送到 RhythKey Pro 控制器"
    )
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "--monitor", action="store_true",
        help="只读模式：只打印灯效数据，不发送震动"
    )
    group.add_argument(
        "--test-rumble", action="store_true",
        help="测试模式：发送已知颜色组合验证固件解码"
    )
    parser.add_argument(
        "--player", type=int, default=None, metavar="N",
        help="指定 XInput player index (0-3)，默认自动检测"
    )
    args = parser.parse_args()

    # 只读模式不需要 XInput
    if args.monitor:
        run_wrapper(None, None, monitor_only=True)
        return

    # 加载 XInput
    xinput_dll = load_xinput()
    if xinput_dll is None:
        print("[!] 无法加载 XInput DLL")
        sys.exit(1)
    print("[✓] XInput 已加载")

    # 自动检测或使用指定的 player index
    player_index = args.player
    if player_index is None:
        player_index = find_player_index(xinput_dll)
        if player_index is None:
            sys.exit(1)

    # 注册退出清理
    def cleanup():
        xinput_set_state(xinput_dll, player_index, 0, 0)
    atexit.register(cleanup)

    if args.test_rumble:
        run_test_rumble(xinput_dll, player_index)
    else:
        run_wrapper(xinput_dll, player_index, monitor_only=False)


if __name__ == "__main__":
    main()
