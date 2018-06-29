
echo命令当调用write，比如write(fd, "hello", 5)，如果write返回的值小于5，echo会默认继续调用，直到返回值和write的cnt一致;

如果在pc上向设备写数据，一旦设备写满，echo会不断重复写入，需要向进程发送SIGKILL信号将其杀死；

char *a = 0x8000;
a+1 指向 0x8001

int *b = 0x9000;
b+1 指向 0x9004

void *c = 0xa000;
c++ 指向　0xa001


当用户态fork一个进程时，除了分配task_struct, 页表，还需要分配一个内核栈，只有8K；
一旦程序通过系统调用进入内核，就会用到这8K的栈；
int my_open(...)
{
	int test[2000];

}


父进程fork了一个子进程，子进程能不能访问父进程打开的文件fd3? 
可以

内核会不会创建一个新的file? 
不会，只会增加file的计数(file->f_count)

IO多路复用：select, poll, epoll
都要求驱动提供file_operations->poll

后面讲输入设备(input)时，可以同时打开鼠标，键盘，触摸屏等多个设备，然后用poll来判断哪个设备有数据；

long, void *: 
32位平台这两个类型为4字节，64位平台这两个类型变为8字节;

long long:
无论32位还是64位平台，都是8字节;

同步IO:
普通的read/write，如果当前数据没有准备好，调用read/write的人会阻塞，等待直到数据准备好；

异步IO:
aio_read/aio_write，将数据请求直接提交给内核，进程提交完毕后可以做别的工作，内核准备好数据后，会通过信号来通知进程;


