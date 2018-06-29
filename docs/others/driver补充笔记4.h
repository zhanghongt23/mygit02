
访问寄存器需要用到位运算：

例子1:
寄存器物理基地址0x8000;偏移0x4, 大小4字节;
将bit[7:4]设置为0xA，其他位不变;

static void __iomem *vir_base;
vir_base = ioremap(0x8000, 4K);

用读－修改－写的模式改变寄存器的特定位：
int value;
value = readl(vir_base + 0x4);
//应该首先将bit[7:4]的原有内容清除，然后在写入0xA
value &= ~(0xF<<4);
value |= (0xA<<4);
writel(value, vir_base+0x4);


例子2:
同样的寄存器，将bit[27:20]按位取反，其他位不变；
value = readl(vir_base + 0x4);
value ^= (0xFF<<20);
writel(value, vir_base+0x4);





4根线，一根电源，一根地，两根数据线传输差分信号（半双工）
usb1.1: 12Mbps
usb2.0: 480Mbps
usb3.0: 2.4Gbps(全双工，新增了两对差分线)
usb3.1: 5Gbps(全双工)

最快的串行线为pci-e，在x86中使用

bps: 指每秒钟线上电平改变多少次 ，传多少个bit; 4b/5b  baudrate 
115200bps/10 1字节


uart是同步还是异步？异步
i2c是同步还是异步传输？SCL/SDA, 同步


并行传输：
需要的线数量比较多，布线费劲；台式机后面有一个25pin的并口；
并行如果要快速传输，对布线要求非常高；


三总线(并行线)：地址＋数据＋控制

	SMP: Symmetrical Multiprocessor 对称多处理器


	mutex_lock_intertuptible(); 
	一旦发现当前临界区有人了，则调用睡眠方法;

	mutex_unlock();
	内部会调用wake_up

	int counter;
	counter++;

	atomic_t counter;
	counter++;

goto: 最适合用在错误处理，以及跳出多层循环;

