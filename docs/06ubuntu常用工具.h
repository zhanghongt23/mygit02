/*************************
 * linux下常见工具的使用
 * Author: zht
 * Date: 2017-12-26
 *************************/

包括:
find: 	文件查找
dd:		文件拷贝
wc:		文件信息统计
ps:		统计进程信息
ln:		创建硬连接和软连接


find
=========================
(1)列出文件路径
--------------------
$>find
$>find /home/test
列出当前或给定目录下所有文件的绝对路径，包括目录

(2)-name/-iname
--------------------
$>find /home/test -name "*.c"
列出所有后缀为.c的文件(可以用双引号把内容括起来"*.c")

$>find /home/test ! -name "*.c"
列出所有后缀不是.c的文件

$>find /home/test -name "[A-Z]*" | wc -l
找到目录中所有大写字母开头的文件(必须加双引号)，并统计数量

$>find /home/test -iname "make*"
忽略大小写，这样Makefile/makefile都能找到


(3)-type
---------------------
$>find /home -type d
列出/home下的所有目录

$>find /dev -type b -exec ls -l {} \;
用ls显示/dev下所有块设备文件的信息;
可用的type还有c(char设备文件)，f(普通文件)，p(FIFO或命名管道)，l(符号链接)，s(套接字)

$>find /home -type d -exec chmod 755 {} \;
将找到的所有目录的访问属性改为755

$>find -exec touch {} \;
将当前目录下所有文件的时间戳更新为当前时间

$>find /home -type d -name "*drv"
列出/home目录下所有以drv作为结尾的目录名

(4)-user/-mtime/-size
-----------------------
$>find /home -user root
列出/home下所有UID为root的文件

$>find /home -mtime -2
列出/home目录下两天之内修改过内容的文件;
"-mtime 2"为正好两天前修改过的文件;
"-mtime +2"为修改时间超过两天的文件;
还可以使用-atime(访问时间)和-ctime(改变uid等)
a: access
m: modify
c: change

$>find /home -size +5k
列出大小超过5K(1024)字节的文件，还可以用5M，5G等



dd
========================
(1)if/of/bs/count
------------------------
$>dd if=/dev/zero of=abc.txt bs=950 count=10
if: input file(输入文件)
of: output file(输出文件)
bs: block size(文件拷贝的单位)
count: 拷贝多少个bs大小的单元
从/dev/zero读入0，写入文件abc.txt，每个块950字节，一共拷贝10个块
(文件总大小为9500字节)


(2)skip(针对输入文件)
------------------------
$>dd if=a.img of=b.img bs=64 skip=1
输入文件为a.img，输出文件为b.img(新建);
以64字节为块大小，跳过第一个块(也就是不拷贝a.img最前面的64字节)


(3)seek(针对输出文件)
------------------------
$>dd if=uboot.bin of=abc.txt count=2 seek=1
如果不指定块大小(bs)，则默认每个块512字节;
生成文件abc.txt的大小为1536字节，前512字节为0，后面1024字节(2个block)的内容从uboot.bin读入

$>dd if=bl1.bin of=/dev/sdb seek=10
/dev/sdb为磁盘对应的设备文件;
从bl1.bin读入数据，跳过/dev/sdb的前10个块，从第11个块开始写入;



wc
======================
wc命令可用于统计行数,文件数量等;

(1)统计文件数量
-----------------------
$>cd /srv/ftp/cless-e6818/X6818_B/kernel/
$>find -type f -name "*.c" | wc -l
//统计内核中c文件的数量


(2)统计每个文件的行数
-----------------------
$>find -type f -name "*.c" -print0 | xargs -0 wc -l
//统计每个c文件的行数以及总行数(由wc自行加总)
//如果在kernel目录下运行, 统计出的总行数可能会有问题, 此时可以进入net等子目录分别统计

$>find -type f -name "*.c" -print0 | xargs -n10 -0 wc -l
//每10个文件计算一次总行数



ps
====================
显示当前系统中进程信息，可以用参数过滤显示的内容；
常用的参数后缀有：
$>ps aux
最全面的显示，包括user, pid, cpu占用率，内存占用率，comm等大量内容

$>ps fax
较全面的显示，包括pid, comm等

$>ps -eo pid,ppid,state,rtprio,nice,pri,comm
明确指定ps显示哪些内容;
包括进程ID，父进程ID,进程状态，实时优先级，nice值，优先级，进程名等

实时进程:
实时进程的nice字段为空;
优先级用rtprio来表达，范围从0到99，值越大优先级越高;
99为实时进程的最高优先级，ubuntu下所有实时进程rtprio都为99;

普通进程:
普通进程的rtprio字段为空，用nice字段来表达优先级;
nice值的范围从-20到19，值越小进程级别越高，-20为普通进程的最高优先级;
多数内核线程的nice值为-20，用户态线程的nice值一般为0;

内核进程优先级:
对于内核来说，会用prio来表达所有进程优先级(ps命令的参数为pri);
prio是一个从0到139的优先级，数字越小优先级越低;
普通进程的prio从0到39，0为最低，39最高;
nice值为-20的进程其prio为39，nice值为-2则prio为21;
nice值为0则prio为19，nice值为19则prio为0(nice+prio的和为19);
实时进程的prio从40到139，40对应rtprio的0，为最低;
139对应rtprio的99，为所有进程的最高值;



ln
====================
$>ln xxx yyy 
为源文件xxx建立链接文件yyy(硬链接)

$>ln -s xxx yyy
为源文件xxx建立链接文件yyy(软链接)



head/tail
====================
$>ps fax | head
显示内容的前面10行

$>ps fax | head -5
显示内容的前面5行

$>ps fax | tail
显示内容的后面10行


pstree
=======================
$>pstree
以树型显示线程关系，不含内核线程

$>pstree -p
在线程后面加上pid号

$>pstree -a
显示进程及其命令行参数



ifconfig/ifdown/ifup
===========================
$>ifconfig eth0 172.16.120.100
用ifconfig改IP地址

$>ifconfig eth0 hw ether 00:11:AA:BB:CC:DD
用ifconfig改MAC地址

$>ifdown eth0
关闭网卡

$>ifup eth0
打开网卡(需要由DHCP服务器分配ip地址)
dhcp服务器的地址可以查看文件/var/lib/dhclient/dhclient-xxx.lease



stat
=====================
$>stat shrek6410_config
显示文件的各种信息，如下：
File: "shrek6410_config"
  Size: 56896     	Blocks: 112        IO Block: 4096   普通文件
Device: 804h/2052d	Inode: 7367262     Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1000/ UNKNOWN)   Gid: ( 1000/ UNKNOWN)
Access: 2014-01-21 18:01:06.061534420 +0800
Modify: 2013-04-09 10:48:57.000000000 +0800
Change: 2014-01-21 18:01:11.207635482 +0800


locate
======================
$>locate test.c
列出文件系统中所有名字为test.c的文件的绝对路径


tune2fs
======================
$>tune2fs -l /dev/sda4
//只用于ext2/ext3/ext4文件系统的分区
//可调整inode/block数量等

