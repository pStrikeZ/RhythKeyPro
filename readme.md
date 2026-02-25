# RhythKey Pro

![](doc/img/photo_2025-09-07_21-05-48.jpg)

# [English readme](readme_en.md)

## 简介

这是一款可以自行动手制作的迷你便携式音乐游戏控制器。  
其通过基于 `ATmega32U4` 的 Arduino 模块模拟一台 Xbox 360 游戏手柄，进行对音乐游戏的控制。

支持的音乐游戏列表：

* 音击（ONGEKI）  
* SOUND VOLTEX  
* DJMAX RESPECT V
* osu!mania
* In Falsus Demo

还有一些未经测试但理论上来说能玩的游戏，例如 osu 的其他模式、初音未来：歌姬计划等。

## 开发计划

目前本项目的完成度达到了可以正常游玩的地步，但是还有一些提升空间：

- [x] 调整上盖模型的轴体卡扣部位厚度
- [x] 编写一个软件 Wrapper 从音击的灯效 Pipe 中获取实时的灯效信息，  
并编码成震动数据流推送到控制器的 RGB 灯组上实时变化
- [ ] 微调旋钮的精度并摆脱 `Smooth Axis` 设置
- [ ] 重绘一版将整块 PCB 分成两块版本的 EDA 工程文件，  
绕过嘉立创的免费打样尺寸限制，降低打板成本

不提供 ETA ，我也不知道完成这些改进需要花多少时间。当然，如果您想要提交 PR ，我也热烈欢迎。

## 装配方法

请移步 [手册](doc/readme.md) 进行详细步骤的查看。

## 为什么不用 ESP32 ？

本项目的核心功能是将控制器模拟为一台 Xbox 360 游戏手柄（XInput 设备），这要求主控芯片具备**原生 USB Device** 能力并能自定义设备描述符。

`ATmega32U4` 拥有硬件级 USB 控制器，配合 [ArduinoXInput](https://github.com/dmadison/ArduinoXInput) 库可以直接将设备枚举为 XInput 手柄，在 Windows 上免驱即插即用。这是本项目选择它的决定性原因。

而 ESP32 系列中：

* **ESP32-C3**：仅有 USB Serial/JTAG 接口，**不支持 USB Device 模式**，只能通过蓝牙模拟手柄，用蓝牙手台打音游还是太有生活了。
* **ESP32-S2 / S3**：支持 USB OTG，理论上可以通过 TinyUSB 实现 XInput，但需要自行编写 USB 描述符和协议栈适配，开发复杂度远高于 ATmega32U4 的成熟方案。

在性能方面，本项目的工作负载（按键矩阵扫描、编码器读取、ADC 采样、WS2812 驱动、XInput 上报）对 16MHz 的 AVR 来说绰绰有余，不存在性能瓶颈。

综合考虑兼容性、开发成本和生态成熟度，`ATmega32U4` 是 USB 游戏控制器类项目的最优选择。

如果您就是看不惯 Arduino ，欢迎 Fork 了自己改。

## 致谢 / 项目渊源

本项目是 [Rhythkey_V1](https://github.com/TheSiriusS233/Rhythkey_V1) 的一个彻底重构和升级的版本。  
我们对原作者 [TheSiriusS233](https://github.com/TheSiriusS233) 的出色工作表示深深的敬意和感谢。  
如果您对原始版本感兴趣，请访问其官方仓库。

## 许可证

本项目采用混合许可模式：

* **外壳部分** (包括所有位于 `/case` 目录下的3D模型等文件)   
采用 [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/) 许可证。
* **PCB部分** (包括所有位于 `/pcb` 目录下的EDA文件, 原理图, PCB布局等文件)   
采用 [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/) 许可证。
* **软件部分** (包括所有位于 `/firmware` 目录下的Arduino源代码)   
采用 [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html) 许可证。

简而言之，欢迎您基于本项目进行学习、修改和非商业性的分享。但严禁将本项目的硬件设计直接用于任何商业目的的生产和销售。  
也请您不要以接受他人委托并从中获利的形式使用本项目进行制造，这同样违反我们的条款。
