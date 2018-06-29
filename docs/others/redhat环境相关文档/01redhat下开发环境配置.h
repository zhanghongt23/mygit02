主题：
	1. 和开发板建立通讯连接
	2. 将文件系统写入开发板
	3. 安装交叉编译环境
	4. 在QT系统中安装和运行C程序
	5. 在Android系统中安装和运行C程序(可选)

Author: zht
Date: 2017-06-19

请参考本文，安装红帽64位环境下的嵌入式开发相关工具，以及相关环境变量的配置；



1. 和开发板建立通讯连接
=========================
我们使用的是深圳九鼎公司的e6818开发板；
使用该开发板的第一步是用串口建立通讯连接；

(1)连接串口线
-------------------------
用串口线将开发板的串口和PC的串口相连，如果是台式机，可以使用机器后面的普通串口；如果是笔记本，需要使用开发板中配套提供的USB转串口设备;

连好串口线后，就可以利用串口通讯工具和开发板交流了，在windows中的串口通讯工具为超级终端/串口助手等，而linux中为minicom;


(2)minicom的安装和配置
-------------------------
minicom是linux环境下的串口通讯工具，可以直接用yum安装：
$>yum install minicom

第一次运行minicom时要先配置一下：
$>minicom -s

进入配置界面，配置过程如下：
--> 将光标定位到第三项(Serial port setup)敲Enter键进入;
--> 按A键，设定串口对应的设备
    如果开发板连接到PC后面的串口，则设备文件名为/dev/ttyS0;
    如果使用USB转串口线连接开发板，则设备文件名为/dev/ttyUSB0
    输入完成后按enter键
--> 按E键，设置串口通信的波特率为115200，8N1
--> 按F键，关闭硬件流控
--> 修改完后，选择Save setup as dfl


(3)minicom的使用
------------------------
再次启动minicom时，如果串口对应的设备文件名没有变化，则可以直接运行minicom；
$>minicom
启动开发板后马上按PC上任意按键，则和uboot交互；否则和文件系统交互；

如果要退出minicom，同时按下Ctrl+A，松开后再按Z，再按X则退出；


(4)minicom不能正确退出
-------------------------
如果没有正确退出，下一次启动minicom时可能会出现如下错误：
$>minicom
Device /dev/ttyUSB0 lock failed: 不允许的操作.

此时，可以删除minicom的锁文件：
$>rm -f /var/lock/LCK*

删除后，再次启动minicom:
$>minicom




2. 将文件系统写入开发板
===================================
九鼎为e6818移植了多种不同的文件系统，包括Android，QT，Ubuntu等；
我们可以把自己想要的文件系统烧写到开发板上；

(1)安装fastboot
-----------------------
由于串口的速度比较慢，因此无法用于传输大文件，需要连接usb线进行传输；
fastboot是一个通过usb接口传输文件的工具；

将fastboot拷贝到/usr/bin下：
$>cp /var/class-e6818/tools/fastboot /usr/bin


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


(3)镜像文件的烧写
-------------------------
A. 系统上电，在串口终端中按空格键进入uboot命令行

B. 确认USB线已正确连接至PC端，在uboot的串口终端中键入命令：
x6818#fastboot

C. 在PC端键入如下命令:
烧写uboot:
$>fastboot flash ubootpak ubootpak.bin
烧写内核：
$>fastboot flash boot boot.img
烧写文件系统：	
$>fastboot flash system qt-rootfs.img

如果系统环境变量有改变，特别是分区相关的变量，可以执行如下指令复位：
x6818#env default -f -a; save;


(4)更新环境变量
-----------------------
uboot默认的环境变量针对Android，因此需要更新为针对QT文件系统的；

在uboot的交互环境中输入：
x6818#env set bootcmd "ext4load mmc 2:1 0x48000000 uImage; bootm 0x48000000"
x6818#env set bootargs "root=/dev/mmcblk0p2 rw rootfstype=ext4 lcd=vs070cxn tp=gslx680-linux"
x6818#env save
minicom默认是不换行的，如果要手工数据上面的环境参数，可以用Ctrl+A,W启动换行；
推荐直接用鼠标右键拷贝/粘贴；


(5)重新启动开发板
-------------------------
如果之前的步骤执行正确，则重新启动开发板以后，开发板会启动QT文件系统；

其他文件系统的烧写可以都尝试一下；




3. 安装交叉编译环境
====================================
gcc是linux环境下的asm和c语言编译器，生成的是可以在x86平台上运行的可执行程序；
而在开发板上运行的程序则需要arm平台专用的编译器，也称为交叉编译器；
交叉编译器可以由开发人员手工定制，也可用使用别人已经编译好的，比如arm-linux-gcc；

(1)安装交叉编译器
----------------------
$>cd /var/class-e6818/tools/
$>tar xzvf crosstools-451.tar.gz  -C /usr/local/
$>cd /usr/local
$>ls
可以看到一个子目录toolschain/，该目录下存放了用于arm平台的交叉编译器和其他工具。arm-linux-gcc等可执行程序位于bin子目录下。


(2)环境变量的设置
----------------------
如果希望在控制台中直接运行arm-linux-gcc，则必须把arm-linux-gcc所在的路径记录到控制台的默认环境变量PATH中，修改文件：
$>vim  ~/.bashrc
在文件的最后加入以下内容：
export  PATH=/usr/local/toolschain/4.5.1/bin:$PATH
//写入/etc/bashrc也可

退出vim后，使新的环境变量生效：
$>source ~/.bashrc
也可以写成：
$>. ~/.bashrc


(3)验证交叉编译器是否安装正确
-----------------------------
$>which arm-linux-gcc
如显示/usr/local/toolschain/4.5.1/bin/arm-linux-gcc则说明环境变量配置OK;
如果没有，则检查一下~/.bashrc，并再次运行$>source ~/.bashrc

用交叉编译器编译hello.c:
$>vim hello.c
$>arm-linux-gcc  hello.c  -o arm-hello (生成可执行程序)
$>file  arm-hello
可以看到，这个应用程序需要在arm平台上运行

如果执行arm-linux-gcc有问题，可能是缺libz库，安装：
$>yum install zlib*.i686
i686对应32位的安装包，如果不写后缀，则默认安装64位的包(xxx.x86_64)；

如果报告的错误如下：
/lib/ld-linux.so.2: bad ELF interpreter: 没有那个文件或目录
则应该安装：
$>yum install glibc*.i686



4. 在QT系统中安装和运行C程序
===============================
如果开发板上烧写的是QT文件系统，则可以通过串口把交叉编译的C应用程序拷贝到开发板运行；

(1)安装lrzsz工具
--------------------------
lrzsz是一个支持ymodem/zmodem等协议的文件传输工具，可以配合minicom使用，实现用串口传输文件的功能；

在pc端安装lrzsz:
$>yum install lrzsz

开发板上已经默认安装了工具lrz/lsz;


(2)minicom的配置
-----------------------
接下来要对minicom进行配置：
$>minicom -s
进入minicom的配置界面，如下：
 +-----[configuration]------+
 | Filenames and paths	|
 | File transfer protocols  |
 | Serial port setup        |
 ....

选择Filenames and paths，内容为：
A - Download directory :    
B - Upload directory   : 
Download目录是从开发板上拷贝文件到PC的目录
Upload目录则是从PC拷贝到开发板上文件所在的目录；
比如已经编译好一个应用程序myhello，要拷贝到开发板上，则应放入Upload目录；

目录设置如下：
A - Download directory : /var/class-x6818/app/download/    
B - Upload directory   : /var/class-x6818/app/upload/ 

修改完配置后，退出配置界面；


(3)在开发板上启动接收程序
------------------------
完成minicom的配置后，就可以收发文件了；
如果要从pc向开发板发送文件，则应该首先在开发板上启动接收程序：
x6818#cd /home
x6818#lrz
lrz开始等待接收应用程序，接收完毕后存入当前目录；


(4)通过minicom发送文件
-----------------------
用快捷键Ctrl+A,S可以启动文件的发送，minicom会显示Upload目录下的文件；
键入myhello，回车后，可以看到整个传输过程开始；
串口的速率较慢，每秒大概10KB左右;



5. 在Android系统中安装和运行C程序(可选)
==================================
如果开发板上运行android系统，则可以通过USB线和开发板进行交互；

(1)安装adb工具
-------------------
Android系统提供了一个很方便的工具，即adb: 
adb: Android Debug Bridge(安卓调试桥)
有了adb以后，PC和开发板之间就只需要连接USB线，就可以实现登录开发板，拷贝文件等工作了；

adb的使用：
首先将adb拷贝到/usr/bin下
$>cp /var/class-e6818/tools/adb /usr/bin

可能需要安装如下rpm包：
$>yum install ncurses*.i686
$>yum install libstdc++*.i686

正确安装后，运行adb命令测试：
$>adb shell
登陆开发板，用exit退出登录


(2)将elf程序安装到开发板
----------------------
$>vim hello.c
$>arm-linux-gcc -static -o hello hello.c
$>adb remount
$>adb push hello /system/bin/
//由于无法链接到android系统中的libc库，本例必须用静态编译
$>adb pull /system/bin/ifconfig
用pull参数可以把开发板上的文件拷贝到pc机上

$>adb shell
//登陆开发板执行hello


