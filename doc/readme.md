# RhythKey Pro 使用手册

## 控制器外壳的 3D 打印

请前往 [Releases](https://github.com/pStrikeZ/RhythKeyPro/releases) 下载外壳的 `.3mf` 模型文件，  
或者自行使用喜欢的 3D 建模软件打开 `/case` 目录下的 `.step` 文件进行修改。  
可以联系拥有 3D 打印机的朋友或者互联网上的付费 3D 模型打印服务进行打印。

带有 `with Text` 后缀的上盖 3mf 文件是有整活文字的版本，如果您选择的 3D 打印服务提供商支持多颜色材料混合打印，推荐使用此模型文件，并在提交打印请求时充分告知提供商所使用的颜色配置。

![](img/0DF8D29E-5A36-44F4-87DD-D1579D7350B3.png)

## PCB 的打印

您可以使用 [嘉立创EDA Pro](https://pro.lceda.cn/) 登录账号并导入位于 `/pcb` 目录中的 `.epro2` 格式文件，并进行 Gerber 文件的导出，然后前往嘉立创在线 PCB 下单服务进行 PCB 的打印。

或者如果您不需要对 PCB 进行审阅或修改，也可以直接前往 [Releases](https://github.com/pStrikeZ/RhythKeyPro/releases) 进行 Gerber 文件的下载。

推荐选择白色的 PCB 以使电路板正面的丝印更加美观。

注意：由于我在绘制 PCB 的时候使用了一些没有电气信息的封装，所以 EDA 中的电气原理图功能不具备参考价值。

## BOM 配件采购

首先，您需要购买用于 BOM 的电子元件，这些元件的清单如下，大概分为两类，一类是 `必购配件`，一类是 `可选配件`，但是推荐所有的配件全部都购买并组装。

### 必购配件

* 1x [Sparkfun Arduino Pro Micro](https://www.sparkfun.com/pro-micro-3-3v-8mhz.html)  
也可以是其他厂商生产的兼容版本，只要是搭载 ATMega32U4 主控芯片的模块就可以了。  
切勿购买使用 Mini-USB 接口的版本，其 PCB 尺寸和我们的 PCB 不符，~~而且我相信您应该很难在2026年找到一根那样的数据线~~  
推荐购买预先焊接好插针的版本，使用方便快捷。
* 12x 您喜欢的五脚机械键盘轴体  
我在改良设计这款音游控制器的时候，有朋友询问过我能不能使用光轴或者静电容轴来提升其在音游场景下的绝对游玩体验。  
遗憾的是，答案是不能，光轴或静电容轴的触发力度 / 键程调节功能不是一位在校学生可以在不借助任何专业调校设备的帮助下使用基于 Arduino 的主控方案实现的。
* 12x 凯华机械键盘轴座
* 8x 1U 尺寸的机械键盘键帽和 4x 2.75U 尺寸的机械键盘键帽
* 4x 2U 尺寸的机械键盘卫星轴  
在大多数时候您应该会被店家捆绑销售一根适用于机械键盘空格键的 6.5U 或者 7U 长度的卫星轴，这是正常现象。
* 2x ALPS EC11 系列旋转电位器以及2个电位器旋钮帽  
由 ALPS 品牌原装进口的 EC11 电位器有两个版本，分别是带有刻度的 `EC11E09244BS` 和不带刻度的 `EC11E1834403`。  
两种版本的电位器我都有采购，强烈推荐购买不带刻度的 `EC11E1834403` 型号电位器，因为带刻度的电位器每旋转一格就会有强烈的段落感，在游玩需要拧动旋钮的音游（ SOUND VOLTEX ）时会有非常强烈的违和感。  
从日本原装进口的 EC11 系列电位器价格非常高昂，您可以购买成本低廉的国产平替型 EC11 电位器，但是我无法保证使用这类第三方电位器时的游玩体验。  
在购买电位器旋钮帽的时候请确保旋钮帽内部的形状一致。
* 1x ALPS B10K 75mm 滑动电位器以及1个滑动电位器推子帽  
您同样可以购买国产平替版本的 B10K 75mm 滑动电位器，但是我无法保证使用这类第三方电位器时的游玩体验。
* 7x 6mm*6mm 贴片按键式轻触开关，按键高度 7.5mm
* 4x R0805 10KΩ 贴片电阻
* 16x R0805 100nF 贴片电容
* 1x R0805 330Ω 贴片电阻
* 12x R0805 1Ω 贴片电阻
* 12x WS2812 反贴 RGB LED 灯珠  
务必购买反贴版本的灯珠，否则将无法焊接到 PCB 上。
* 18x R1206 1N4148 反冲二极管
* 3x 1N4007 SMA 二极管
* 6x M2*8 自攻螺丝
* 5x M2*16 自攻螺丝
* 可以调整温度的电烙铁，焊锡丝，焊油，松香，**以及一把弯头尖嘴镊子**  
*或者如果您拥有热风枪的话，也许可以试试回流焊？*

### 可选配件

* 2x 2.54mm 直插单排母排针母座  
如果您不考虑模块的可插拔特性，可以直接将模块焊死在主板上，虽然我不推荐您这么做。
* 6x 6mm 厚度的橡胶脚垫  
在使用排针插座连接 Arduino 模块时，模块的高度会超出 3D 打印件的厚度，所以需要几个脚垫将其垫高，旋钮侧垫高 12mm ，用户侧垫高 6mm 即可。
* 12x 机械轴的轴下垫  
主要是我真不觉得几片泡沫能改变以 3D 打印的材料为外壳的音游控制器的按键听感到什么地步，那么还不如什么都不贴。但因为您可以选择贴上，所以在这里提一嘴。

## 焊接

除了位于主板正面的 6 个贴片轻触开关以外，所有的贴片电子元件全部都被设计在了主板的背面，丝印直接指示元器件型号，对号入座即可。  
先完成背面的贴片元件焊接之后，再焊接电位器、正面的贴片开关、排针母座等部件。  
完成的电路板应该如下图所示，**虽然图中的 PCB 是曾经的 Prototype 版本，和仓库中的 PCB 不一致**：

![](img/photo_2025-09-07_23-01-35.jpg)

![](img/photo_2025-09-07_23-01-37.jpg)

## 组装

* 将卫星轴嵌入到上盖当中
* 将 PCB 放置在底壳中，拧上 M2*8 的自攻螺丝
* 将顶壳放置在底壳上，拧上 M2*16 的自攻螺丝
* 安装机械轴、旋钮帽、电位器帽、键帽  
如果觉得电位器帽在电位器上面很松动，可以在电位器帽里面塞一张小纸片，这会增大电位器帽与电位器的摩擦力。

完成组装的产品如图所示：

![](img/photo_2025-09-07_21-05-48.jpg)

## 固件烧录

### 第一次刷入

首先下载并安装 [Arduino IDE](https://www.arduino.cc/en/software)，打开 Preferences 选项卡。

在 Additional Boards Manager URLs 中填入 `https://raw.githubusercontent.com/dmadison/ArduinoXInput_Boards/master/package_dmadison_xinput_index.json` ，如图所示。如果这一行里已经有其他的内容了，就追加到后面，以分号间隔开。

![](img/4AF872F5-66ED-403F-9668-8D7AD8C9A4F6.png)

然后打开 Boards Manager ，搜索并安装 XInput SparkFun Boards 。

![](img/C669478A-9825-449B-BC53-AC3749E21E64.png)

接下来再打开 Library Manager ，搜索并安装 XInput 库和 FastLED 库。

![](img/5DD11E6F-D4BB-4D52-8B6E-4DE5271D5FEE.png)

![](img/446A5C16-07A0-4C32-A101-41BE26E88EA4.png)

然后，将装配好的 RhythKey Pro 的 USB-C 接口连接到电脑。  
第一次刷入固件时不需要 RESET 按钮便可识别到暴露的 COM 端口，点击识别到串口旁边的笔图标，搜索并选中 `SparkFun Pro Micro w/ XInput` ，选中右侧对应的端口再选择 OK 。

![](img/A767EAC8-6E66-4B97-9027-A1FE7ACB7AE0.png)

![](img/CAFC25C6-4EF5-4317-B0F2-D91EAE2669FF.png)

接下来（非常重要！）点击 Tools 选项卡，其中有一个 `Processor: ATMega32U4` 的选项，点击它，务必在弹出的菜单中选择 `ATMega32U4 (5V, 16 MHz)` 这一项，否则刷入固件后模块将无法被识别。

![](img/4F5AFB17-A21A-4B92-8C3F-19AF3A24C8E1.png)

然后打开仓库中的 `rhythkeyoro_fw.ino` 文件，点击左上角的 `Upload` 按钮，等待固件烧录完成。

### 固件升级

由于在刷入 XInput 固件之后 Pro Micro 模块会变成一个 XInput 设备，所以 Arduino IDE 将无法识别到它，此时需要按下 Pro Micro 模块的 RESET 按钮，使其重新进入 Bootloader 模式，再进行后续的固件升级。

这个 Bootloader 模式仅会持续 8 秒钟的时间，所以会很考验用户对电脑操作的熟练度。

先打开 Arduino IDE ，按下 RESET 键一次，使模块进入 Bootloader 模式。  
您需要在这 8 秒钟之内完成对模块端口和开发板类型的绑定设置（点击笔图标，选择端口，选择开发板类型）。  
然后在模块不在 Bootloader 模式的情况下，点击 Upload 按钮，烧录固件。  
在 Arduino IDE 完成固件编译后（如下图所示），显示 Uploading 时，立刻按一下 RESET 按钮，使其重新进入 Bootloader 模式，新固件才能被成功刷入。

![](img/26B513CE-9E7E-43BD-9B95-F8A1E58A603F.png)

## 游戏设置

**免责声明**：本项目不提供任何游戏的安装文件及游戏自身的使用教程。

按下控制器面板右下角的贴片按键，可以在 ONGEKI 模式和 VARIOUS 模式之间切换。

在 ONGEKI 模式下，菜单键需要长按 3 秒钟，才会被触发；  
在 VARIOUS 模式下，侧键与菜单键会被直接屏蔽，以防止游戏过程中误触。

### 音击

打开 `segatools.ini` ，调整好其他设置，滚动到配置文件的末尾。

```ini
[io4]
enable=1
; Test button virtual-key code. Default is the F1 key.
test=0x70
; Service button virtual-key code. Default is the F2 key.
service=0x71
; Keyboard button to increment coin counter. Default is the F3 key.
coin=0x72

; Set "1" to enable mouse lever emulation, "0" to use XInput
mouse=0
```

将 `mouse` 项更改为 `0` ，然后启动游戏，按 `F1` 进入测试模式，选择 `レバー設定` 并进入。

![](img/A0AC07B9-7E17-4014-87DB-FF74D8B1B509.png)

![](img/F37B6166-CD39-4C0E-A306-2C2299E3DFFE.png)

先选择 `初期設定に戻す` ，然后缓慢地移动电位器到最左端，再移动到最右端。  
接下来选择 `終了` ，然后 `保存する` ，完成其他需要的设置后退出测试模式，开始享受游戏。

### SOUND VOLTEX

首先切换到 VARIOUS 模式。

运行 `spicecfg.exe` 或者在进入游戏后按 `F4` 按钮，打开 spicetools 的设置界面。

![](img/69BDBD09-95C8-41A5-897E-45F3F30AEDA7.png)

按照图中的方式逐个点击 `Bind` 按钮，然后按下控制器上对应的按键进行绑定。  
绑定完按键后切换到 `Analogs` 选项卡。

![](img/5417F45D-8848-4352-8696-1087133996B3.png)

将两个旋钮分别绑定到控制器的摇杆 `X` 轴和 `Y` 轴上，并启用 `Smooth Axis` ，然后退出设置，开始享受游戏。

### DJMAX RESPECT V

切换到 VARIOUS 模式，进入游戏，进入设置 -> 控制 -> 输入设置 -> 编辑。

![](img/9A561E22-5F77-403A-9E1D-07CB06490577.png)

设置好按键映射之后直接开始享受游戏。

### osu!

使用类似 DJMAX 的方式设置按键映射后即可开始享受游戏。

### In Falsus Demo

切换到 VARIOUS 模式，然后确保在 Steam 中导入这个布局。

steam://controllerconfig/4355100/3667527437

然后在 Steam 设置 -> 控制器 -> 校准及高级设置 中，关闭右摇杆的死区，然后进入游戏。

注：根据不同显示器尺寸的差异，可能需要自行微调映射摇杆的区域垂直位置和区域大小。
