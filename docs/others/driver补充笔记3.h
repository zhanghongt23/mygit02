
64位平台支持48bit地址，48bit虚拟空间为256TB;
对linux来说，用户态使用0x0~0xffff,ffff,ffff之间的虚拟地址(低256TB);
实际上用户态经常只使用低128TB，即0x0~0x7fff,ffff,ffff;

内核态使用0xffff,0000,0000,0000到0xffff,ffff,ffff,ffff之间的虚拟地址(高256TB);


耦合：软件a要调用软件b的代码，a和b直接有耦合．
有耦合一般认为是不太好的，因为b软件一旦修改，a软件也必须跟着修改；

为了克服软件之间的耦合：可以引入中间层的软件(中间件)c, a不再直接调用b，而是调用c，c再去调用b;
如果b变化，只要c不变，则a就不需要修改了;

网络协议的分层起始也体现了这一概念;


c++用namespace { }　来解决名字冲突问题;

c语言实现大规模程序，容易出现名字冲突；
	

int test_a;  int test_a;

注册：
例如cdev_add或misc_register()，是主动将自己的函数告诉特定的人(VFS);
EXPORT_SYMBOL:  
将自己的函数导出，任何人都可以使用;

mod01.ko
int a;
static int b;

EXPORT_SYMBOL(a);

mod02.ko
能不能访问mod01.ko中的a和b? 不能

如果确实希望在mod02.ko中访问变量a，则需要用EXPORT_SYMBOL宏来声明

内核模块中的符号默认是局部的(无论有static还是没有)；
其他模块是不能访问本模块中的符号的;

