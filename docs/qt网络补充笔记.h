
udp通讯一般认为参与的为发送者和接收者;

c语言实现udp的接收者?
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(); //绑定本机IP和给定端口号
	recvfrom(); //等待接收包

实现udp的发送者?
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	sendto(); //将数据发送到给定的IP地址和端口号


c语言怎么实现的tcp server?

	fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(); //绑定本机IP以及给定端口
	listen(); //将socket转为监听模式
	accept(); //返回新的fd, 用于和连接到的client通讯
	read/write(); //和client之间交换数据

c语言怎么实现tcp client?
	fd = socket(AF_INET, SOCK_STREAM, 0);
	connect(); //连接到server
	read/write(); //和server之间交换数据




怎么实现能同时连接多个客户端的服务器?

1.多进程
服务器每次连接到一个client,都fork一个子进程负责和client交流

2.多线程
和多进程差不多.

3.IO多路复用
用select/poll/epoll
很多高性能的web服务器,例如nginx, 都会采用epoll机制(效率最高,但只能用于Linux).



网络协议:

	(1)应用层协议	(http/ftp) 
	由用户态来实现,可以自行定义;
	c语言的libc库没有提供对http的支持;
	c++/java等高层语言一般会封装对http的支持;
	在实践中,应用层协议通常涵盖了表示层和会话层

-----------------------------------------------------
	
	(2)传输层 (tcp/udp)
	由内核来实现,最常见的协议为tcp/udp. 其他的传输层协议,比如Novell的协议,都几乎被淘汰了;
	tcp: 有连接,流式传输
	udp: 无连接,报式传输

	(3)网络层(IP)
	由内核来实现,最常见的协议为IP
	根据IP地址,将数据包传送到给定的IP处.

	(4)链路层(由内核以及驱动和硬件一起实现)/物理层(纯硬件实现)
	根据传输介质的不同,可以有不同的底层协议.
	比如通过以太网,wifi,蓝牙,4G,光纤,可见光...

	以太网包格式:
	14字节包头 | 1500字节数据 | 4字节CRC
	14字节包头: 6字节目的MAC地址,6字节源MAC地址,2字节协议
	1500字节数据: 包含TCP/UDP头, IP头
	4字节CRC: 可以用软件完成,也可以硬件负责计算

