Author: zht
Date: 2017-12-22

主题：
	1. 和开发板建立通讯连接
	2. 安装交叉编译环境
	3. 将QT文件系统刷入开发板
	4. 在QT系统中安装和运行C程序
	5. 将Android文件系统刷入开发板
	6. 在Android系统中安装和运行C程序



1. 和开发板建立通讯连接
=========================
我们使用的是深圳九鼎公司的e6818开发板；
使用该开发板的第一步是用串口或usb口建立通讯连接；

如果开发板上是android系统，则用usb口或者串口连接；
如果开发板上是qt或ubuntu系统，则用串口相连;


(1)连接串口线
-------------------------
用串口线将开发板的串口和PC的串口相连，如果是台式机，可以使用机器后面的普通串口；如果是笔记本，需要使用开发板中配套提供的USB转串口设备;

连好串口线后，就可以利用串口通讯工具和开发板交流了，在windows中的串口通讯工具为超级终端/串口助手等，而linux中为minicom;


(2)minicom的安装和配置
-------------------------
minicom是linux环境下的串口通讯工具，安装如下：
$>sudo apt install minicom

第一次运行minicom时要先配置一下：
$>sudo minicom -s

进入配置界面，配置过程如下：
--> 将光标定位到第三项(Serial port setup)敲Enter键进入;
--> 按A键，设定串口对应的设备
    如果开发板连接到PC后面的串口，则设备文件名为/dev/ttyS0;
    如果使用USB转串口线连接开发板，则设备文件名为/dev/ttyUSB0;
    输入完成后按enter键
--> 按E键，设置串口通信的波特率为115200，8N1
--> 按F键，关闭硬件流控
--> 修改完后，选择Save setup as dfl


(3)minicom的使用
------------------------
再次启动minicom时，如果串口对应的设备文件名没有变化，则可以直接运行minicom；
$>sudo minicom
启动开发板后马上按PC上任意按键，则和uboot交互；否则和文件系统交互；


(4)minicom的退出
-------------------------
如果要退出minicom，同时按下Ctrl+A，按X则退出(Ctrl+A,X)；

如果没有正确退出，下一次启动minicom时可能会出现如下错误：
$>sudo minicom
Device /dev/ttyUSB0 lock failed: 不允许的操作.

此时，可以删除minicom的锁文件：
$>rm -f /var/lock/LCK*

删除后，再次启动minicom:
$>sudo minicom



2.安装交叉编译环境(Cross-Compiler)
====================================
gcc是linux环境下的c语言编译器，生成的是可以在x86平台上运行的可执行程序；
而在开发板上运行的程序则需要arm平台专用的编译器，也称为交叉编译器；
交叉编译器可以由开发人员自行编译，也可从网上下载，如arm-linux-gcc；

(1)安装交叉编译器
----------------------
$>cd /srv/ftp/class-e6818/tools/
$>sudo tar xzvf crosstools-451.tar.gz  -C /usr/local/
$>cd /usr/local
$>ls
可以看到一个子目录toolschain/，该目录下存放了用于arm平台的交叉编译器和其他工具，arm-linux-gcc等可执行程序位于bin子目录下


(2)环境变量的设置
----------------------
如果希望在控制台中直接运行arm-linux-gcc，则必须把arm-linux-gcc所在的路径记录到控制台的默认环境变量PATH中，修改文件：
$>vim  ~/.bashrc
在文件的最后加入以下内容：
export  PATH=/usr/local/toolschain/4.5.1/bin:$PATH

退出vim后，使新的环境变量生效：
$>source  ~/.bashrc
也可以写成：
$>. ~/.bashrc


(3)验证交叉编译器是否安装正确
-----------------------------
$>which arm-linux-gcc
如显示/usr/local/toolschain/4.5.1/bin/arm-linux-gcc则说明环境变量配置OK;
如果没有，则检查一下~/.bashrc，并再次运行source ~/.bashrc

用交叉编译器编译hello.c:
$>vim hello.c
$>arm-linux-gcc  hello.c  -o arm-hello
$>file arm-hello
可以看到，这个应用程序需要在arm平台上运行




3. 将QT文件系统刷入开发板
===================================
九鼎为e6818移植了多种不同的文件系统，包括Android，QT，Ubuntu等；
我们可以把自己想要的文件系统烧写到开发板上；

(1)安装fastboot
-----------------------
由于串口的速度比较慢，因此无法用于传输大文件，需要连接usb线进行传输；
fastboot是一个通过usb接口传输文件的工具；

fastboot的安装：
$>sudo apt install android-tools-fastboot


(2)镜像文件的说明
-----------------------
九鼎已经在光盘中准备好了多种文件系统的镜像，位于X6818_A/Image目录下；
具体烧写过程可以参考对应的目录；
我们后面的课程主要用Qt来设计用户交互程序，因此必须将QT文件系统刷到开发板上；
镜像位于Image/x6818-qt目录下；

镜像文件的说明：
Bootloader:
|                  \   
├── 2ndboot ------ |
├── nsih.txt ----- | => ubootpak.bin
└── u-boot.bin --- /

Kernel:
|                           \
├── battery.bmp ----------- |
├── debug-ramdisk.img ----- |
├── logo.bmp -------------- |
├── ramdisk-recovery.img -- | => boot.img
├── root.img.gz ----------- |
├── uImage ---------------- |
└── upate.bmp ------------- /
                                    
Filesystem:
└── qt-rootfs.img


(3)QT镜像文件的烧写
-------------------------
A.系统上电，在串口终端中快速按空格键进入uboot命令行

B.确认USB线已正确连接至PC端，在uboot的串口终端中键入命令：
x6818#fastboot

C.在PC端键入如下命令:
$>cd /srv/ftp/class-e6818/X6818_A/Image/x6818-qt/

烧写uboot:
$>sudo fastboot flash ubootpak ubootpak.bin
烧写内核：
$>sudo fastboot flash boot boot.img
烧写文件系统：	
$>sudo fastboot flash system qt-rootfs.img

如果系统环境变量有改变，特别是分区相关的变量，可以在开发板上执行如下指令复位：
x6818#env default -f -a 
x6818#env save


(4)更新环境变量
-----------------------
uboot默认的环境变量针对Android，因此需要更新为针对QT文件系统的；

在uboot的交互环境中输入：
x6818#env set bootcmd "ext4load mmc 2:1 0x48000000 uImage; bootm 0x48000000"

如果是并行RGB接口的LCD屏幕，则bootargs如下：
x6818#env set bootargs "root=/dev/mmcblk0p2 rw rootfstype=ext4 lcd=vs070cxn tp=gslx680-linux"

如果是MIPI接口的LCD屏幕，bootargs如下：
x6818#env set bootargs "root=/dev/mmcblk0p2 rw rootfstype=ext4 lcd=wy070ml tp=gslx680-linux"

x6818#env set fastboot "flash=mmc,2:ubootpak:2nd:0x200,0x78000;flash=mmc,2:2ndboot:2nd:0x200,0x4000;flash=mmc,2:bootloader:boot:0x8000,0x70000;flash=mmc,2:boot:ext4:0x00100000,0x04000000;flash=mmc,2:system:ext4:0x04100000,0x2F200000;flash=mmc,2:cache:ext4:0x33300000,0x1AC00000;flash=mmc,2:misc:emmc:0x4E000000,0x00800000;flash=mmc,2:recovery:emmc:0x4E900000,0x01600000;flash=mmc,2:userdata:ext4:0x50000000,0x0;"

x6818#env save
minicom默认是不换行的，如果要手工输入上面的环境参数，可以用Ctrl+A,W启动换行；
推荐直接用鼠标右键拷贝/粘贴；


(5)重新启动开发板
-------------------------
如果之前的步骤执行正确，则重新启动开发板以后，开发板会启动QT文件系统；




4. 在QT系统中安装和运行C程序
===============================
如果开发板上烧写的是QT文件系统，则通过串口把C应用程序拷贝到开发板运行；
minicom大概每秒传10KB左右，不适合传输过大的文件;

(1)minicom的配置
-----------------------
接下来要对minicom进行配置：
$>sudo minicom -s

如果已经打开minicom，则直接用"Ctrl+A,O"开启配置界面;

进入minicom的配置界面，如下：
 +-----[configuration]------+
 | Filenames and paths		|
 | File transfer protocols  |
 | Serial port setup        |
 ....

选择Filenames and paths，内容为：
A - Download directory :    
B - Upload directory   : 
Download的含义是从开发板拷贝文件到PC;
Upload的含义是从PC拷贝文件到开发板;
比如有文件myhello，要拷贝到开发板上，则应放入Upload目录；

目录设置如下：
A - Download directory : /srv/ftp/class-x6818/test/
B - Upload directory   : /srv/ftp/class-x6818/test/

修改完配置后，存储一下: save setup as dfl;
接下来退出配置界面；


(2)Upload(上传文件)
------------------------
将之前交叉编译的应用程序arm-hello拷贝到开发板上，步骤如下:
* 在minicom中按"Ctrl+A,S"
* 在弹出的窗口中选择"zmodem"
* 按enter，minicom会显示之前Upload目录中的文件名
* 手工输入要上传的文件，按enter启动上传
* 等待上传完毕

程序默认被上传到当前目录;
如果之前已经上传过同名的文件，请先删除开发板上的文件，再次上传;


(3)Download(下载文件)
------------------------
还可以用minicom把开发板上的文件拷贝到PC上;
进入文件所处的目录，运行下面命令:
x6818>cd /usr/share/demo/
x6818>lsz sample.wav
//会把sample.wav文件拷贝到pc的/srv/ftp/class-e6818/test目录


(4)在开发板上执行文件
-----------------------
x6818#./myhello




5. 将Android文件系统刷入开发板
===================================
九鼎还为x6818开发板移植了android文件系统，可以将其刷入开发板;
刷android文件系统时应该已经安装了必要的fastboot, minicom等工具;

(1)镜像文件的说明
-----------------------
九鼎准备好的android镜像位于X6818_A/Image/x6818-android目录下；

镜像文件的说明：
Bootloader:
|                  \   
├── 2ndboot ------ |
├── nsih.txt ----- | => ubootpak.bin
└── u-boot.bin --- /

Kernel:
|                           \
├── battery.bmp ----------- |
├── debug-ramdisk.img ----- |
├── logo.bmp -------------- |
├── ramdisk-recovery.img -- | => boot.img
├── root.img.gz ----------- |
├── uImage ---------------- |
└── upate.bmp ------------- /
                                    
Filesystem:
├── system.img
├── userdata.img
├── cache.img
└── recovery.img


(2)android镜像文件的烧写
-------------------------
A.系统上电，在串口终端中快速按空格键进入uboot命令行

B.确认USB线已正确连接至PC端，在uboot的串口终端中键入命令：
x6818#fastboot

C.在PC端键入如下命令:
$>cd /srv/ftp/class-e6818/X6818_A/Image/x6818-android/

烧写uboot:
$>sudo fastboot flash ubootpak ubootpak.bin
烧写内核：
$>sudo fastboot flash boot boot.img
烧写文件系统：	
$>sudo fastboot flash system system.img
	
可选择是否烧写其他分区：
$>sudo fastboot flash userdata userdata.img
$>sudo fastboot flash cache cache.img
$>sudo fastboot flash recovery recovery.img

注意！不能只烧写system.img，ubootpak.bin和boot.img也需要重新烧一下;

如果系统环境变量有改变，特别是分区相关的变量，可以在开发板上执行如下指令复位：
x6818#env default -f -a
x6818#env save


(3)更新环境变量
-----------------------
将uboot的环境变量更新为针对android文件系统的；

在uboot的交互环境中输入：
x6818#env set bootcmd "ext4load mmc 2:1 0x48000000 uImage;ext4load mmc 2:1 0x49000000 root.img.gz;bootm 0x48000000"

如果是并行RGB接口的LCD屏幕，则bootargs如下：
x6818#env set bootargs "lcd=vs070cxn tp=gslx680"

如果是MIPI接口的LCD屏幕，bootargs如下：
x6818#env set bootargs "lcd=wy070ml tp=gslx680"

x6818#env save
minicom默认是不换行的，如果要手工输入上面的环境参数，可以用Ctrl+A,W启动换行；
推荐直接用鼠标右键拷贝/粘贴；


(4)重新启动开发板
-------------------------
如果之前的步骤执行正确，则重新启动开发板以后，开发板会启动android文件系统；




6. 在Android系统中安装和运行C程序
==================================
如果开发板上运行android系统，则可以通过USB线和开发板进行交互；

(1)安装adb工具
-------------------
Android系统提供了一个很方便的工具，即adb;
adb: Android Debug Bridge(安卓调试桥)
有了adb以后，PC和开发板之间只需要连接USB线，就可以实现登录开发板，拷贝文件等工作了;

安装sdb:
$>sudo apt install android-tools-adb


(2)登录开发板
--------------------
如果开发板已经刷入Android系统，且pc通过usb线连接开发板;
则可以运行adb命令登录：
$>sudo adb shell
//登陆开发板，用exit退出登录


(3)静态编译应用程序
----------------------
$>vim hello.c
$>arm-linux-gcc -static -o hello hello.c
//由于无法链接到android系统中的libc库，本例必须用静态编译


(4)Upload(上传文件)
-----------------------
$>sudo adb push hello /data
//将pc上的文件hello拷贝到开发板的/data目录下


(5)Download(下载文件)
-----------------------
$>sudo adb pull /system/bin/ifconfig
//用pull参数可以把开发板上的文件拷贝到pc机上


(6)在开发板上执行程序
-----------------------
x6818>hello


