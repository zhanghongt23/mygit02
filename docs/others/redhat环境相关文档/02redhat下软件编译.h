主题：
	1. 升级glibc
	2. x6818开发板的相关源代码
	3. 编译uboot
	4. 编译linux内核
	5. 编译qt文件系统


1. 升级glibc
=========================
由于redhat6.4系统比较陈旧，使用的是glibc-2.12，而编译uboot时需要用到glibc-2.14，因此需要升级一下glibc库；
下面介绍如何通过源码包进行升级；

(1)解压glibc
------------------------
$>cd /var/class-e6818/tools/
$>tar xjvf glibc-2.14.tar.bz2


(2)编译glibc
------------------------
$>cd glibc-2.14/
$>mkdir build/ 
//在glibc-2.14目录下建立build文件夹
$>cd build/
$>../configure --prefix=/opt/glibc-2.14
$>make && make install
//配置并将glibc安装到/opt/glibc-2.14/目录下


(3)创建指向新库的软链接
-------------------------
在/lib64目录下有一个软链接文件libc.so.6，该链接指向有效的libc-XX.so;
用我们自己编译的glibc替代红帽自己的glibc;

$>rm -rf /lib64/libc.so.6 // 先删除先前的libc.so.6软链
//注意，此时执行ls, ln等命令都会失败

$>LD_PRELOAD=/opt/glibc-2.14/lib/libc-2.14.so ln -s /opt/glibc-2.14/lib/libc-2.14.so /lib64/libc.so.6
//重建软链接，在执行ln命令之前预加载libc库

可以用如下命令查看glibc的版本:
$>strings /lib64/libc.so.6 | grep GLIBC_



2. x6818开发板的相关源代码
=========================
九鼎为x6818提供了完整的源代码，位于/var/class-x6818/X6818_B/目录下；
解压缩后包括uboot, kernel, qt文件系统等；

(1)解压缩
-------------------------
$>cd /var/class-x6818/X6818_B/
$>tar xjvf x6818_linux_160719.tar.bz2


(2)代码说明
-------------------------
解压缩后生成的目录包括：
uboot/
kernel/
buildroot/
out/
linux/
prebuilts/
prototype/
tools/
前三个目录中分别包括uboot，linux内核以及文件系统的源代码；
编译生成的镜像文件存储到out/目录中；


(3)脚本mk
--------------------------
解压缩后还有一个重要的脚本mk，编译uboot，kernel，qt文件系统等都需要通过该脚本进行；
脚本在编译前会设定好各种环境变量，因此不要抛开脚本直接到uboot等目录下编译；

脚本支持的参数包括:
$>./mk -j -u -k -b
  -j=n				启动多个线程同时编译，如-j=4
  -u, --uboot       编译uboot
  -k, --kernel      编译linux内核
  -s, --system      编译android文件系统
  -b, --buildroot   编译QT文件系统
  -a, --all         编译全部(别用这个)
  -h, --help        显示帮助信息



3. 编译uboot
=========================
需要通过脚本mk来编译uboot, kernel, qt文件系统等；

(1)利用脚本mk编译
-------------------------
glibc升级完成后，就可以直接编译uboot了:
$>./mk -u
//编译uboot，uboot的源码位于uboot目录下


(2)说明
--------------------------
生成的镜像文件位于out/release目录下，为ubootpak.bin
镜像文件的说明：
Bootloader:
|                  \   
├── 2ndboot ------ |
├── nsih.txt ----- | => ubootpak.bin
└── u-boot.bin --- /

可以参考相关文档，用fastboot将ubootpak.bin烧写到x6818开发板上；



4. 编译linux内核
========================
内核的代码位于/var/class-x6818/X6818_B/kernel/目录下；

(1)内核的编译和烧写
--------------------
可以直接用mk脚本编译内核：
$>./mk -k
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
 
可以用fastboot烧写到x6818开发板上；


(2)内核的配置
-----------------------
可以通过make menuconfig用图形化的方式进行配置；
$>cd kernel/
$>make menuconfig
通过图形界面(依赖于ncurses库)，决定编译的内核都包含哪些部分;
最终的配置结果，存储在.config文件中;

但是，如果继续通过mk脚本编译，该脚本会使用默认的配置文件重新编译，会覆盖我们刚才进行的修改；
因此，我们需要手工改变一下配置文件(用tools目录下的配置文件替换);

先备份一下旧的配置文件；
$>cd kernel/arch/arm/configs/
$>cp x6818_defconfig config-def

用改过的配置文件替换默认配置文件：
$>cp /var/class-x6818/tools/config_uplook x6818_defconfig

可以用menuconfig看看都修改了哪些内容；


(3)重新编译内核
-------------------------
再次用mk脚本编译内核；
$>./mk -k
将生成的boot.img烧写到x6818开发板上；



5. 编译QT文件系统
=========================
仍然要用mk脚本进行编译：
$>./mk -b
但是很可惜，在redhat下编译qt5是无法成功的。。。

