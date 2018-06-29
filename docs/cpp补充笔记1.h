
数据private, 方法public 

	class Student {
	private:
		string name;
		int value;
	public:
		int getValue();
		void setValue(int newValue);
	};



UML: 统一建模语言(Unified Model Language)

面向对象三大特征:
封装: class, private, protected, public

继承: 从基类扩展出派生类, 用:来实现继承, java用一个关键词extends

多态: 


template模板 : c的宏 : java的泛型

c++/java 

class Student {
	string name;
	int id;
	static int baseId;
	...;
};


static int a = 0;

static int myfunc()
{
	a++;
	printf("a = %d\n", a);
}


	static: 
	最早用于修改局部变量,


	word  xx.doc   xx.docx xml

html: Hyper Text Markup Language : 浏览器
	xml: eXtensible Markup Language : 很多框架在定义图像界面时,都会采用xml,用xml来描述界面;
	xml往往是自行定义,自行解析的;

	父类/子类
	基类(Base) / 派生类(Derived...);



重载(overload):
	可以定义若干方法名一样,但参数不一样的方法,称为重载;
	void setFlags(int flags);
	void setFlags(double flags);
	void setFlags(string name, int flags);
	double setFlags(double flags); //不算重载

	编译器在编译时,会为重载的方法提供另外的名字:
	setFlags_int(...);
	setFlags_double(...);
	setFlags_string_int(...);


使用时,根据传入的参数,判断调用哪个方法:
	setFlags(10);
	setFlags(23.27);
	setFlags("zhang", 20);


64位平台的app,当进入内核后,使用的地址空间为高256T
0xffff,0000,1234,5678


0xffff,ffff,ffff  256T
0x7fff,ffff,ffff  128T

struct mutex mylock;
spinlock_t lock;
wait_queue_head_t mywt;

初始化:
mutex_init(&mylock);
spin_lock_init(&lock);
init_waitqueue_head(&mywt...);



int n = 10;
int test[n];


int main(int argc, char *argv[])
{
	int array[n]; //栈上
	char *tmp = malloc(atoi(argv[2]);

}


