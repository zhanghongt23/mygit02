
数据结构：数组，单链表，双链表，hash，二叉树，图．．．
内核用的最多的：数组，双链表，rb-tree


内核不会调用用户态的东西，头文件都是内核自己的；
用户态会不会调用内核的东西？肯定会．用户态也会用到内核的头文件，一般放到/usr/include目录下，其中的子目录linux包含的是内核头文件;
但是用户态和内核同名的头文件，只会包含用户态可以用的内容，如果头文件中的定义是给内核自己用的，用户态看不到;

VFS: Virtual File System, 虚拟文件系统
目的是隐藏具体文件系统的细节，用户态按照虚拟文件系统的要求去访问文件，VFS再转换程具体文件系统的要求；
比如/home目录对应/dev/sda1，文件系统为ext4
/media/test目录对应/dev/sdb1，文件系统为vfat

$>cp /home/a.txt /media/test



linux下的设备分类？
三类：
网络设备:
特征：收发数据包(有一定的包报式，典型的是以太网包，内核负责把包准备好，网络只负责收发)，例如以太网卡，wifi网卡, 4G网卡等


块设备(block设备): 
(特征：按块读写)，例如硬盘，固态硬盘,emmc,光盘.
磁盘买来以后第一件事情，就是要分区／格式化；
格式化时要指定文件系统类型，比如$>mkfs.ext4 /dev/sdb1
$>mkfs.vfat /dev/sdb2 
格式化后，就要按照文件系统的要求来读写文件；

扇区: sector(默认512字节)，硬件概念，记录硬盘的最小存储单元
块(block): 当按文件系统格式化磁盘以后，文件系统会设定一个磁盘的最小读写单位，就是block(可以sector的倍数，可以为512,1024,2048,4096字节，不会超过一个物理页)；
	比如一个300字节的文件，在磁盘上要占用１个block；


char设备(字符设备):
一个硬件，不是网络也不是块设备，就是char设备;
课程中，只涉及char设备，只讲char驱动的设计；
char设备涵盖了大多数硬件，从最简单的led，按键，到最复杂的摄像头，音频等硬件;


//定义
struct student {
	char *name;
	int math;
	int chinese;
	int english;
	...
};

//分配并初始化结构体
struct student std1 = {"zhangsan", 80, 0, 83, 90, ...};

//c99推荐用下面的方式来初始化
struct student std2 = {
	.name = "lisi",
	.english = 90,
	.math = 80,
};


struct abc {
	...;
} __attribute__(("packed"));

section: 段
一般来说，讨论elf格式时，会用到段；
elf格式将.c文件中的代码，数据等分别放到不同的section里；

abc.c a.out
int a;	//bss段
int b = 10;　//data段
char *e = "hello"; //e在data段，指向的hello在rodata段

int __my_func()
{}

//如果增加__section__，则编译器会把main函数放入.init.text段
int __attribute__((__section__(".init.text"))) main(void)
{
	int c = 20; //栈
	int *d = malloc(100); //d变量在栈上，指向的空间在堆
	...
}




加载(load): 将代码从磁盘拷贝到内存．最典型的加载就是执行a.out
$>./a.out
a.out在执行之前在磁盘上，一旦执行代码，则linux首先启动加载器，加载器将磁盘上的a.out，按照elf格式的不同段(section, .init, .data, .bss等段)拷贝到内存里的相应地址．
模块的加载需要手工调用insmod工具，该工具负责将磁盘上的模块文件拷贝到linux内核．

卸载(unload): 删除内存里的模块，磁盘上的还在．
			  使用rmmod工具卸载．

安装(install): 从磁盘上的某个位置拷贝到另外的位置．
			   make install 


内核通过make menuconfig来确定要编译哪些内容
对于每个.c或.s文件，都可以有三种不同的选择：
不选：
源代码不参与编译，内核不使用该功能

选中(*): 
该文件的内容很重要，要直接编译到内核中．
源代码编译为.o，最后要通过连接进入zImage．
zImage在内核中解压缩，然后常驻内存．
如果改变了zImage，还需要重新刷机．

选为模块(M):
如果代码在编译内核时无法确定以后会不会用到，就可以将代码编译为模块．
如果确实要用到该代码，则可以在用户态用insmod将该模块加载(load)到当前运行的linux内核．

可以把linux内核当成一个可以扩展的特殊库；




module 

#define ABC 100
#define wait_event(xxx) ...

inline函数会放在.h文件
inline函数和宏是比较类似的

static void inline mytest(...)
{
	...;
}


