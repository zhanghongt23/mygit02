/********************************
 * 本文介绍Redhat6下常用服务的配置和使用
 * 服务包括:
 * yum, apache, tomcat, ftp, nfs, tftp, dhcp
 * Author: zht
 * Date: 2017-06-21
 ********************************/

1.Redhat6_64下yum的配置
===============================
安装红帽6下的各种服务前，最好先将yum配置起来。这样就不必用rpm来安装软件了。

(1)检查yum是否已安装
----------------------------
$>rpm -qa | grep yum
(已安装则显示如下内容)
yum-utils-1.1.30-14.el6.noarch
yum-rhn-plugin-0.9.1-40.el6.noarch
yum-plugin-security-1.1.30-14.el6.noarch
yum-metadata-parser-1.1.2-16.el6.x86_64
yum-3.2.29-30.el6.noarch


(2)yum的配置
-----------------------
配置文件为/etc/yum.repos.d/rhel-source.repo
在其中加入如下内容：
[rhel]
name=rhel
baseurl=file:///iso
enabled=1
gpgcheck=0

上面的设置是通过光盘镜像来获取红帽的软件包，应该提前将红帽的iso镜像挂载到/iso目录下：
$>mkdir /iso
$>mount /home/tools/rhel6_64.iso /iso -o loop

如果网络中已经配置好了红帽镜像的ftp服务器，则可以指定yum从网络上直接下载，在rhel-source.repo文件中加入：
[rhel1]
name=rhel1
baseurl=ftp://172.16.8.100/rhel6.4/
enabled=1
gpgcheck=0
尚观目前的ftp服务器IP是这个，如果有变更，请咨询网管。


(3)search/provides
-----------------------------
$>yum search jpeg
显示所有包含字符串jpeg的包；
jpeg可以不出现在字符串开头，也不区分大小写；
xxx.x86_64的包为64位应用程序和库;
xxx.i386的包为32位应用程序和库；

$>yum provides mysql
检测给定包在哪个yum源，是否安装等;
mysql匹配mysql开头的包，如果要查找不是mysql开头但包含mysql的包，则支持：
$>yum provides *mysql
可以查找出php-mysql-5.3.3-22.el6.x86_64等包


(4)install/remove
----------------------
$>yum install glibc
安装给定的rpm包(可自动安装依赖的包，rpm命令必须手工安装)
支持通配符，如：
$>yum install tftp*
列出yum源中以tftp开头的包，并说明该包是否已安装

$>yum remove lftp
删除给定的安装包



2.Redhat6_64下apache服务器的配置
===============================
apache是linux下最常用的web服务器。

(1)检查是否已安装了apache服务器的安装包
$>rpm -qa | grep httpd
(如显示如下内容，说明apache已经安装，没有则自己装一下)
httpd-2.2.15-15.el6_2.1.x86_64
httpd-tools-2.2.15-15.el6_2.1.x86_64
httpd-devel-2.2.15-15.el6_2.1.x86_64

(2)启动apache服务
$>service httpd restart

(3)apache的配置文件
为/etc/httpd/conf/httpd.conf
(好像没什么要改的)

(4)网页的目录
网页默认存放在/var/www/html/目录下，可以用浏览器访问
也可以如下测试：
$>elinks --dump http://192.168.1.100/test.html

(5)测试注意事项
可以通过safari等浏览器访问网页，访问之前最好把防火墙和selinux关闭
关闭selinux的方法：
$>vim /etc/selinux/config
将SELINUX设置为:
SELINUX=disabled
$>setenforce 0  /* 使能该选项 */

(6)自动启动httpd服务
可以使用$>setup，用图形配置设置开机自动启动httpd



3. tomcat服务器的配置
=========================
tomcat也是apache基金会提供的web服务器，类似于apache，可用于执行jsp脚本；jsp类似于php，运行在服务器端，文件通常用xx.jsp命名；

tomcat服务器的启动：
(1)首先安装jdk，并配置环境
(2)进入tomcat目录，执行脚本：
	$>.../tools/tomcat/bin/startup.sh
3. 写测试网页如test.jsp，放入目录：
	$>cd .../tools/tomcat/webapps/ROOT
	$>vim test.jsp
4. 打开浏览器，输入地址：
	127.0.0.1:8080/test.jsp

test.jsp的内容如下：
<html>
<head>
	<title>JSP example</title>
</head>
<body>
	<%	out.println("Hello, jsp world!"); %>
</body>
</html>



4. Redhat6_64下ftp服务器的配置
===============================
vsftpd是Redhat6下比较常见的tftp服务器

服务器端
-------------------------
(1)检验是否安装了vsftpd服务器
$>rpm -qa | grep vsftpd
(如显示如下内容说明已经安装)
vsftpd-2.2.2-11.el6.x86_64
(没安装可以用yum安装)
$>yum install vsftpd

(2)配置文件
$>vim /etc/vsftpd/vsftpd.conf
确保anonymous_enable=YES

(3)默认的ftp共享目录
为/var/ftp/pub/，将要共享的文件放入其中

(4)启动服务
$>service vsftpd restart


客户端
-----------------------
(1)用浏览器访问
可以直接打开firefox浏览器，键入：
ftp://192.168.1.xx/
就会显示FTP服务器的默认共享目录pub

(2)用lftp访问
$>lftp 192.168.1.xxx
在lftp中执行:
lftp 192.168.1.xxx:~>cd pub/
lftp 192.168.1.xxx:/pub>get xxx.pdf
(将ftp共享目录中的文件拷贝到执行lftp命令时所处的目录下)
lftp 192.168.1.xxx:/pub>exit
(退出lftp)



5. Rhel6下nfs服务器的配置
=============================
nfs服务一般有两种用途：
(1)登录到nfs服务器下载文件
(2)在嵌入式开发板测试时，将开发板的文件系统放置到nfs服务器的共享目录下，开发板启动时直接mount到nfs-server

服务器端
---------------------------
一般来说，nfs服务器是默认安装的

(1)设置nfs的共享目录
$>vim /etc/exports
在文件的第一行加入如下内容：
/var/local *(ro,sync,no_root_squash)

(2)启动nfs服务
$>service nfs restart

(3)测试NFS服务是否启动
$>netstat -a | grep nfs 
tcp    0  0 *:nfs *:* LISTEN
udp    0  0 *:nfs *:*

(4)检查NFS的共享目录
$>exportfs  (看看输出中有没有/var/local目录)

客户端
----------------------
可以直接登录到开启了nfs_server的机器，server的IP为(192.168.1.20):
$>mount 192.168.1.20:/var/local /mnt
$>cd /mnt
目录下的内容和nfs-server共享目录中一致，可以cp到本机



6. Rhel6下tftp服务器的配置
===========================
tftp服务常用于嵌入式开发，bootloader(如uboot)利用tftp服务从pc上将linux内核下载到开发板上。

(1)安装TFTP服务器
可以用yum或rpm来安装。用rpm安装的方法：
$>cd  /iso/Packages
$>rpm  -ivh  xinetd…rpm  (用Tab键补齐)
$>rpm  -ivh  tftp-server….rpm  (Tab键补齐)

(2)修改TFTP服务的配置文件
$>vim  /etc/xinetd.d/tftp
将文件中disable后面的选项从yes改为no

(3)重新启动TFTP服务
$>service  xinetd  restart

(4)检查tftp服务
$>chkconfig --list 
如果tftp的服务没有打开,则用下面命令打开tftp服务开关
$>chkconfig tftp on 
$>netstat -a | grep tftp
udp 0 0 *:tftp *:* 

(5)关闭PC的防火墙
点击桌面最顶上的菜单，进入图形化的配置界面，先将防火墙关闭，然后按应用按钮，最后退出图形配置界面

(6)关闭PC的Selinux机制
如果有必要，还需要将PC的Selinux机制关闭。方法如下：
$>vim  /etc/selinux/config
将SELINUX=xxx一行修改为”SELINUX=disabled”。存储退出vim
要让修改过的配置马上生效，在控制台中执行：
$>setenforce  0

(7)将编译好的linux内核拷贝到TFTP共享目录
$>cp  /var/embed/bin/uImage  /var/lib/tftpboot



7. Rhel6下DHCP服务的配置
============================
DHCP服务的作用是动态为连接到服务器的客户机分配IP地址。比如教师机启动DHCP服务，可以为学员的机器动态分配IP。
再比如连接4412开发板和pc机时，可以在开发板的android中配置为利用DHCP自动获取ip地址，这样就可以不用静态指定IP了。

(1)获得dhcp的配置文件
/etc/dhcp目录下存放dhcp的配置文件。该目录下默认为空。可以从帮助文档中拷贝一个实例配置文件:
$>cp /usr/share/doc/dhcp-4.1.1/dhcpd.conf.sample /etc/dhcp/dhcpd.conf 

(2)修改配置文件
$>vim /etc/dhcp/dhcpd.conf
找到其中和内部子网相关的配置项，可能如下：
# A slightly different configuration for an internal subnet.
subnet 10.5.5.0 netmask 255.255.255.224 {
  range 10.5.5.26 10.5.5.30;
  option domain-name-servers ns1.internal.example.org;
  option domain-name "internal.example.org";
  option routers 10.5.5.1;
  option broadcast-address 10.5.5.31;
  default-lease-time 600;
  max-lease-time 7200;
}

将其修改为和要字段分配IP的网段相关的内容，如下：
subnet 192.168.4.0 netmask 255.255.255.0 {
  range 192.168.4.10 192.168.4.50;
  option domain-name-servers 202.106.0.20;
  option domain-name "internal.example.org";
  option routers 192.168.4.254;
# option broadcast-address 10.5.5.31;
  default-lease-time 600;
  max-lease-time 7200;
}
//其中202.106.0.20是北京地区的DNS服务器
//192.168.4.254是服务器的IP
//可动态分配的IP从192.168.4.10到192.168.4.50

(3)启动DHCP服务
$>service dhcpd restart
$>chkconfig | grep dhcpd

(4)客户端动态基于DHCP分配IP
如果是运行Redhat的客户端，则可以用setup设定通过DHCP动态获得IP.
也可以手工修改下面的文件：
$>vim /etc/sysconfig/network-scripts/ifcfg-eth2
DEVICE=eth2
BOOTPROTO=none //将none修改为dhcp
HWADDR=00:1C:25:A0:5B:3F
NM_CONTROLLED=no
ONBOOT=yes
TYPE=Ethernet
UUID="7da02a23-647c-428d-b067-c7f5a1b860f0"
IPADDR=192.168.4.253
NETMASK=255.255.255.0
IPV6INIT=no
USERCTL=no

修改完文件后，重启网络服务:
$>service network restart

