Author: zht
Date: 2017-12-22

主题：
	1.ubuntu终端的配置
	2.ubuntu下安装软件包
	3.通过lftp拷贝课程资料
	4.安装需要的软件包
	5.安装搜狗输入法
	6.安装wps-office


1. ubuntu终端的配置
=======================
(1)打开终端
--------------------
在屏幕上用鼠标右键点击，即可打开终端


(2)修改终端配置
-------------------
将鼠标移动到屏幕的上方，会出现终端的配置菜单;
编辑-->配置文件首选项
//可以修改文字大小，配色方案等


(3)在标签中新建终端
-------------------
默认是用一个新窗口来新建终端，如果觉得不方便，可以改成在标签中新建；

将鼠标移动到屏幕的上方，会出现终端的配置菜单;
终端-->配置文件首选项
//选择新终端打开在"标签页"，默认的选项为"新窗口"

终端-->新建终端，或者右键菜单"打开终端"
//在标签页中新建终端

也可以直接用快捷键"Ctrl+Shift+T"在标签页中打开新的终端


(4)修改终端提示符的长度
-----------------------
打开终端时，默认的终端提示符为"用户名@host名 当前目录名$"
如果装系统时设置的名字太长，可以改短一些;

修改~/.bashrc中的变量PS1
$>vim ~/.bashrc
//若此时还没有安装vim，可以参考下面的安装过程

找到其中定义PS1变量的一行(原始内容如下):
PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '

去掉\h\[\033[00m\]: 不再显示host名
将\w改为\W: 只显示目录的最后一级
再次打开终端，提示符就变短了




2. ubuntu下安装软件包
=============================
ubuntu默认是不支持用root用户登录的，也不提倡大家用root登录；
如果要完成一些需要root权限的操作，请使用sudo xxx执行；
如果必须要用root登录，可以尝试用$>sudo -i

(1)安装已经下载的deb包
------------------------
如果已经下载了deb包，可以直接安装：
$>sudo dpkg -i xxx.deb
类似于redhat中用rpm -ivh安装rpm包；
不推荐这种安装方式，因为很可能会因为依赖的包没有提前装好而失败；


(2)通过联网方式安装软件包
-------------------------
在ubuntu中，强烈推荐采用联网方式安装：
$>sudo apt install xxx
//xxx为要安装的软件，如vim等，安装包的名字可以用Tab键尝试补齐

有时候可能会出现依赖方面的问题或者冲突，此时可以尝试：
$>sudo apt -f install

如果遇到找不到要安装的软件等问题，可以尝试：
$>sudo apt update 



3. 通过lftp拷贝课程资料
=============================
为方便大家下载，教师机通过FTP服务来共享课程；
可以直接用浏览器下载，在地址栏输入"ftp://192.168.100.144"；
推荐通过lftp工具下载文件或整个目录，参考"ubuntu基本服务.h"一文；

(1)lftp的安装
--------------------
$>sudo apt install lftp

(2)登录到ftp服务器
---------------------
$>cd ~
$>lftp 192.168.100.144
//首先进入家目录，获取ftp服务器的IP地址后登录

(3)拷贝资料
----------------------
lftp登录成功后,可以使用ls和cd命令查看服务器上的目录信息，也可以通过mirror命令下载整个目录:
lftp 192.168.100.144:~>ls
drwxr-xr-x	...	class-e6818
drwxr-xr-x	...	upload

lftp 192.168.100.144:/>mirror class-e6818/
//下载到登录lftp时所在的目录

也可以下载到指定目录:
lftp 192.168.100.144:/>mirror /home/zht

(4)退出
---------------------------
lftp 172.25.0.200:/>exit




4.安装需要的软件包
=========================
下面是课程中会用到的各种软件/开发库等，在有网络的情况下，建议一起都安装好；

安装vim
$>sudo apt install vim

安装minicom
$>sudo apt install minicom

安装tree
$>sudo apt install tree

安装ctags
$>sudo apt install exuberant-ctags

安装fastboot
$>sudo apt install android-tools-fastboot
//用于通过usb线向开发板拷贝数据

安装adb(android debug bridge)
$>sudo apt install android-tools-adb
//用于连接android设备

安裝git
$>sudo apt install git

安裝texinfo
$>sudo apt install texinfo

安裝bison
$>sudo apt install bison
//bison是一个语法分析器

安装kolourpaint4(可选)
$>sudo apt install kolourpaint4
//kolourpaint4为画图软件

安裝ncurses库
$>sudo apt install lib32ncurses5
$>sudo apt install libncurses5-dev

安装zlib库
$>sudo apt install lib32z1

安装C++库
$>sudo apt install lib32stdc++6

安装jpeg库
$>sudo apt install libjpeg9-dev

安装QT相关库(OpenGL等)
$>sudo apt install build-essential 
$>sudo apt install libgl1-mesa-dev
$>sudo apt install freeglut3-dev
$>sudo apt install libglew-dev
$>sudo apt install libsdl2-dev
$>sudo apt install libsdl2-image-dev 
$>sudo apt install libglm-dev 
$>sudo apt install libfreetype6-dev




5. 安装搜狗收入法
============================
ubuntu的默认输入法为拼音，如果用不习惯，可以考虑安装搜狗输入法；

安装搜狗输入法大体分为三步：
(1)添加fcitx键盘输入法系统
(2)安装sogou输入法
(3)设置系统参数

(1)添加fcitx键盘输入法系统
----------------------------
首先要添加fcitx键盘输入法系统，因为sogou是基于fcitx的，而系统默认的是iBus，
fcitx相当于是输入法的管理系统；

安装fcitx:
$>sudo apt install fcitx

有可能会报告说有一定依赖关系方面的问题，此时可以尝试：
$>sudo apt-get -f install


(2)安装搜狗输入法
---------------------------
fcitx相当于是输入法的管理器,并不包含具体的输入法；
下面就要安装搜狗输入法了，可以提前从网站下载对应的deb文件；
$>cd /srv/ftp/class-e6818/tools/
$>sudo dpkg -i sogoupinyin_2.1.0.0086_amd64.deb 


(3)系统配置
----------------------------
点击屏幕左端的按钮列:
系统设置-->语言支持
将输入法系统从iBus切换为fcitx；
此时，可能还会要求安装部分应用，点击安装即可；

点击屏幕上面的输入法配置，将搜狗设置为第一个即可；
如果老是找不到输入法，可以reset一下机器；




6.安装WPS-office
=============================
后期写简历或项目文档时，一般会用到office；
Ubuntu默认的LibreOffice很不好用，可以考虑安装WPS-Office；

(1)安装WPS
-------------------------
可以去wps的官网下载最新版本的wps:
	http://community.wps.cn/download/
官网上提供alpha和beta版，建议用alpha版，alpha版更新一些，且提供64位的版本；

安装tools目录下的deb包；
$>cd /srv/ftp/class-e6818/tools/WPS-Office
$>sudo dpkg -i wps-office_10.1.0.5672~a21_amd64.deb


(2)加入字体
--------------------------
默认安装时，wps只提供几种基本字体，可以去网上下载更多的字体；

拷贝tools目录下的字体:
$>sudo mkdir /usr/share/fonts/wps-office
$>cd /srv/ftp/class-e6818/tools/WPS-Office/wps-fonts/
$>sudo cp * /usr/share/fonts/wps-office/


(3)启动wps-office
--------------------------
在"搜索"中输入"wps"，就可以看到"wps文字"等应用程序的图标，点击即可启动；

