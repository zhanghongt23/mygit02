Author: zht
Date:	2017-12-22

主题:
	1.安装ubuntu+Windows双系统
	2.基于Parallel虚拟机安装Ubuntu(MacOS)
	3.基于VmWare虚拟机安装Ubuntu(Windows)



1.安装ubuntu+Windows双系统
==========================
在众多的linux发行版中,ubuntu是被开发人员选择最多的一款;

如果大家的pc中已经安装了windows，可以安装VmWare，以虚拟机的形式运行ubuntu;
但在实践中发现，在虚拟机下ubuntu运行较慢，因此推荐大家安装双系统;

关于双系统的完整安装过程，可以参考others目录下的文档:
"Windows10+Ubuntu双系统安装.pdf"

用ubuntu启动盘启动时，可能会通报一个错误:
"gfxboot.c32: not a COM32R image"
"boot: "
此时按Tab键，输入live-install即可安装ubuntu

在安装双系统时，如果windows采用UEFI/EFI模式的BIOS，则无法启动双系统；
格式化重装windows吧。。。



2.基于Parallel虚拟机安装ubuntu
=============================
如果在MacOS系统下工作，推荐使用Parallel虚拟机(好贵，肉疼);
虚拟机很好使，尤其适合MacBook Pro;

(1)下载ubuntu镜像

(2)新建虚拟机安装时，一定不要选快速安装
把该选项去掉后，安装时会弹出语言选择界面，此时选择中文即可;
如果使用快速安装，默认采用的是英文，且安装后，怎么也切换不到中文;

(3)默认的虚拟机硬盘为64GB
如果觉得不够大，应该新建一个空白的虚拟机，然后修改大小;
最后连接光盘镜像安装;

(4)安装Parallel Tools
采用中文安装后，默认只支持800×600的分辨率，这是虚拟机本身的问题；
将鼠标放到屏幕的最上沿，或者用Ctrl+Alt将光标从虚拟机释放，在虚拟机控制菜单中，会出现一个黄色的！；
点击后，会提示安装Parallel Tools，按继续，该工具将挂载到光盘上，点击光盘图标，右键启动控制台，安装install脚本：
$>sudo ./install
再次启动虚拟机，就可以支持各种分辨率了；



3.基于VmWare安装虚拟机
===========================
如果使用Windows系统，那么基本都是采用VmWare作为虚拟机的;

VmWare的下载，安装，注册，使用略;

安装ubuntu时，不要选择快速安装，在虚拟机启动前连接光盘镜像;

安装完ubuntu后，一定记得安装VmWare的辅助工具;
该工具和Parallel Tools的安装过程类似, 都是在ubuntu中运行一个脚本;


