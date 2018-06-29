Author: zht
Date: 2017-12-22

主题：
	1. X6818开发板的相关源代码
	2. 编译uboot
	3. 编译并配置linux内核
	4. 编译并配置qt文件系统(buildroot)



1. X6818开发板的相关源代码
=========================
九鼎为x6818提供了完整的源代码，位于/srv/ftp/class-e6818/X6818_B/目录下；
解压缩后包括uboot, kernel, qt文件系统等；

(1)解压缩
-------------------------
$>cd /srv/ftp/class-e6818/X6818_B/
$>tar xjvf x6818_linux_160719.tar.bz2


(2)代码说明
-------------------------
解压缩后生成的目录包括：
uboot/
kernel/
buildroot/
linux/
prebuilts/
prototype/
tools/
前三个目录中分别包括uboot，linux内核以及文件系统的源代码；
编译完后会生成一个新目录out，生成的镜像文件存储到out/目录中；


(3)脚本mk
--------------------------
解压缩后还有一个重要的脚本mk，编译uboot，kernel，qt文件系统等都需要通过该脚本进行；
脚本在编译前会设定好各种环境变量，因此不要抛开脚本直接到uboot等目录下编译；

脚本支持的参数包括:
$>./mk -j -u -k -b
  -j=n				启动多个线程同时编译，如-j=8
  -u, --uboot       编译uboot
  -k, --kernel      编译linux内核
  -b, --buildroot   编译定制的文件系统(支持QT)
  -s, --system      编译android文件系统(google提供，九鼎修改)
  -a, --all         编译全部(别用这个)
  -h, --help        显示帮助信息



2. 编译uboot
=========================
uboot的源代码位于/srv/ftp/class-e6818/X6818_B/uboot目录下；

(1)利用脚本mk编译
-------------------------
可以利用mk脚本直接编译uboot，mk脚本需具备执行权限:
$>chmod +x mk
$>./mk -u
//编译uboot，uboot的源码位于uboot目录下


(2)烧写uboot
--------------------------
生成的镜像文件位于out/release目录下，为ubootpak.bin
镜像文件的说明：
Bootloader:
|                  \   
├── 2ndboot ------ |
├── nsih.txt ----- | => ubootpak.bin
└── u-boot.bin --- /

首先连接usb线，在uboot中输入：
x6818#fastboot

在PC端输入:
$>cd /srv/ftp/class-e6818/X6818_B/out/release/
$>sudo fastboot flash ubootpak ubootpak.bin


(3)uboot的核心工作
------------------------
6818上电以后首先运行内部ROM中的代码，该代码读emmc，将uboot最前面的部分拷贝到6818内部的64KB RAM中，然后运行uboot;

uboot负责完成的工作包括:
A. 初始化硬件
包括cpu的时钟，电源，看门狗，UART，尤其重要的是初始化DDR-RAM
B. 将uboot剩余的部分拷贝到DDR
C. 通过交换命令和pc交流，或者默认引导内核
D. 将内核从emmc拷贝到DDR，引导

一旦uboot将工作交给内核，uboot的使命就结束了，后续工作交给内核完成;




3. 编译并配置linux内核
========================
linux内核的源代码位于/srv/ftp/class-e6818/X6818_B/kernel/目录下；
我们可以根据自己的需求增加或去掉部分设备驱动;
如果开发板上连接了新的硬件，可能还需要开发人员修改现有的代码，或者写一个新的设备驱动;
这些操作统称为linux内核移植/驱动开发，是嵌入式开发工程师需要掌握的核心技能之一;


(1)内核代码修改
--------------------
首先修改一下内核源码根目录下的Makefike，以后统称为根Makefile;
$>vim /srv/ftp/class-e6818/X6818_B/kernel/Makefile
定位到195行，修改如下：
ARCH	?= arm
CROSS_COMPILE	?= arm-linux-

由于九鼎在近期对开发板升级后emmc的版本比较高，和内核代码不匹配;
如果要从源代码开始构建内核，需要修改一下代码:
$>vim /srv/ftp/class-e6818/X6818_B/kernel/drivers/mmc/core/mmc.c
定位到文件的295行，修改如下：
	card->ext_csd.rev = ext_csd[EXT_CSD_REV];
/* 
	if (card->ext_csd.rev > 7) {
		pr_err("%s: unrecognised EXT_CSD revision %d\n",
			mmc_hostname(card->host), card->ext_csd.rev);
		err = -EINVAL;
		goto out;
	}
*/



(2)内核的编译和烧写
--------------------
我们需要用九鼎提供的mk脚本来编译linux内核：
$>./mk -k -j=8
编译生成的内核镜像位于out/release目录下，为boot.img;

镜像文件说明:
|                           \
├── battery.bmp ----------- |
├── debug-ramdisk.img ----- |
├── logo.bmp -------------- |
├── ramdisk-recovery.img -- | => boot.img
├── root.img.gz ----------- |
├── uImage ---------------- |
└── upate.bmp ------------- /
 

(3)内核的配置
-----------------------
采用mk脚本编译时，使用九鼎默认的配置文件;
此时生成的内核会包含开发板上所有设备的驱动;
在后面的学习中，我们要使用自己写的驱动，这会和九鼎的驱动产生冲突;
因此，必须在编译内核时去掉部分现有的驱动;

执行make menuconfig，对内核进行配置:
$>cd kernel/
$>make menuconfig
通过图形界面(依赖于ncurses库)，决定编译好的内核都包含哪些内容;
最终的配置结果，存储在.config文件中;

android内核的版本名为: 3.4.39+
qt内核的版本名为: 3.4.39-9tripod



(4)需要去掉的驱动
-----------------------
-->取消按键的驱动
Device Drivers -->
  Input Device Support -->
	[*]Keyboard -->
将下面选项前面的*号去掉:
  < > SLsiAP push Keypad support
//按键驱动针对开发板侧面的4个GPIO按键
//我们后面要用GPIO按键来测试中断，会和现有驱动冲突

-->取消蜂鸣器的驱动
Device Drivers -->
  Character devices -->
将下面选项前面的*号去掉:
  [ ] X6818 beep driver
//我们会写自己的蜂鸣器驱动，有可能会和九鼎的驱动冲突

-->取消LED灯的驱动
Device Drivers -->
  Led Support -->
将下面选项前面的*号去掉:
 < > LED Support for GPIO connected LEDs 
 [ ] LED Trigger support 
//内核默认带有LED灯的驱动，且其中一个灯持续闪烁，将驱动去掉

修改完成以后，退出图形界面，新的配置选项存储到文件.config中;


(5)覆盖默认配置文件
-----------------------
用mk脚本编译内核时，脚本会使用arch/arm/configs/目录下的文件x6818_defconfig作为默认的配置文件;
用.config文件覆盖该文件:
$>cp .config arch/arm/configs/x6818_defconfig
//每次修改完.config后，都需要执行上述操作


(6)重新编译并烧写内核
-------------------------
再次用mk脚本编译内核；
$>./mk -k -j=8
将生成的boot.img烧写到x6818开发板上；

首先连接usb线，在uboot中输入：
x6818#fastboot

在PC端运行fastboot:
$>cd /srv/ftp/class-e6818/X6818_B/out/release
$>sudo fastboot flash boot boot.img

烧写完成后，重启开发板即可，此时LED灯应该全部熄灭或全部点亮;




4.编译QT文件系统
=========================
(1)编译文件系统
-----------------------
开发板上运行的第三部分软件就是用户态的应用程序，库等;
仍然要用mk脚本进行编译：
$>./mk -b -j=8
//-b代表buildroot


(2)修改文件系统代码
----------------------
编译过程中，可能会在编译ncurses库时出现问题；
解决方法如下：
$>vim /srv/ftp/class-e6818/X6818_B/buildroot/output/build/host-ncurses-5.9/include/curses.tail
在104行有一句注释 /* generated */
将该句注释删掉即可，注意不要删除前面的语句，只删注释；
注意！由于这句话会在编译的过程中产生，只能碰到bug以后再改


(3)编译qt文件系统可能出错的原因
--------------------------
A. 目录结构问题
由于编译采用的是绝对路径，路径中绝对不能有空格，最好别有中文

B. 文件权限
编译之前，最好将整个目录的属主和组设为当前用户:
$>sudo chown -R zht:zht /srv/ftp/class-e6818/
//-R参数后面为用户名:组名

C. 磁盘满
整个buildroot编译完后大概占6.8G左右的磁盘，请用df -h确认一下当前磁盘是否有足够的空间

D. 以前笔记中要求装的包没装
比如bison, texinfo等

E. 如果不是新装的ubuntu，实在不行就重装一次


(4)buildroot的配置
-----------------------
buildroot是一个开源的项目，用于帮助大家利用网上的开源软件构建嵌入式文件系统;
X6818_B目录下的buildroot是九鼎针对x6818开发板准备的，大部分的软件包都已经下载完毕了;
如果对生成的文件系统不满意，还可以进一步对buildroot进行配置，在文件系统中增加更多的软件，库等;

基于图形界面配置buildroot:
$>cd X6818_B/buildroot/
$>make menuconfig
配置过程和内核的配置类似，按照个人需求修改;
配置完毕后，会将配置选项存储到.config文件中(同内核)


(5)buildroot的重新编译
------------------------
和内核的编译一样，九鼎也准备了一个默认的buildroot配置文件，如果用mk脚本进行编译，会用默认文件覆盖我们刚才的修改;
因此，要用刚才改过的.config文件覆盖默认配置文件;
$>cd X6818_B/buildroot/configs/
$>cp x6818_defconfig x6818_back
$>cp ../.config ./x6818_defconfig

退回到X6818_B目录，执行脚本mk编译新的文件系统:
$>cd X6818_B/
$>./mk -b -j=8
如果选择了新的软件包，buildroot会首先通过网络将其下载到本地，然后进行交叉编译，最后制作成文件系统;

注意！如果编译时选择了vim和bash，则需要在ubuntu中安装hg工具:
$>sudo apt install mercurial


(5)文件系统的烧写
-----------------------
首先连接usb线，在uboot中输入：
x6818#fastboot

在PC端运行fastboot:
$>cd /srv/ftp/class-e6818/X6818_B/out/release
$>sudo fastboot flash system qt-rootfs.img

烧写完成后重启开发板，看看新增加的软件，库等是否可用

