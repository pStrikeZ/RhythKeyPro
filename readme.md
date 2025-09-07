# RhythKey Pro

![](doc/img/photo_2025-09-07_21-05-48.jpg)

## 简介

这是一款可以自行动手制作的迷你便携式音乐游戏控制器。  
其通过模拟一台 Xbox 360 游戏手柄，进行对音乐游戏的控制。

支持的音乐游戏列表：

* 音击（ONGEKI）  
* SOUND VOLTEX  
* DJMAX RESPECT V
* osu!mania

还有一些未经测试但理论上来说能玩的游戏，例如 osu 的其他模式、初音未来：歌姬计划等。

## 装配方法

请移步 [手册](doc/readme.md) 进行详细步骤的查看。

## 致谢 / 项目渊源 (Acknowledgements / Origin)

本项目是 [Rhythkey_V1](https://github.com/TheSiriusS233/Rhythkey_V1) 的一个彻底重构和升级的版本。  
我们对原作者 [TheSiriusS233](https://github.com/TheSiriusS233) 的出色工作表示深深的敬意和感谢。  
如果你对原始版本感兴趣，请访问其官方仓库。

## 许可证

本项目采用混合许可模式：

* **外壳部分** (包括所有位于 `/case` 目录下的3D模型等文件)   
由于并未对上游仓库进行任何更改，沿用上游的 [MIT License](case/LICENSE) 许可证。
* **PCB部分** (包括所有位于 `/pcb` 目录下的EDA文件, 原理图, PCB布局等文件)   
采用 [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/) 许可证。
* **软件部分** (包括所有位于 `/firmware` 目录下的Arduino源代码)   
采用 [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html) 许可证。

简而言之，欢迎您基于本项目进行学习、修改和非商业性的分享。但严禁将本项目的硬件设计直接用于任何商业目的的生产和销售。  
也请您不要以接受他人委托并从中获利的形式使用本项目进行制造，这同样违反我们的条款。
