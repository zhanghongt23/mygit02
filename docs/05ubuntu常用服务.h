/*******************************
 * 本文介绍Ubuntu下各项服务的配置
 * 包括nfs, ftp, tftp, VNC等
 * Author: zht
 * Date: 2017-12-04
 *******************************/

内容包括:
1. NFS服务
2. FTP服务
3. lftp客户端
4. TFTP服务
5. VNC服务

http服务 / MySQL服务


1.NFS服务
=========================
NFS服务用于共享服务器端的文件, 客户机可以通过mount命令把服务器的目录挂载到本地目录下;

(1)安装NFS服务器
----------------------
安装Ubuntu nfs服务器端：
$>sudo apt install nfs-kernel-server
安装服务器时，默认安装客户端nfs-common


(2)准备好要共享的目录
----------------------
$>sudo mkdir /var/uplook/


(3)修改NFS的配置文件/etc/exports
-----------------------
$>sudo vim /etc/exports
在最后一行加入如下内容：
/var/uplook *(rw,sync,no_root_squash)

/var/uplook: 是要共享的目录;
*: 		代表允许所有的网段访问（也可以使用具体的IP）
rw：	挂接此目录的客户端对该共享目录具有读写权限
sync：	资料同步写入内存和硬盘
no_root_squash：客户机用root访问时，不映射为匿名用户
注意！逗号之间不要有空格；


(4)重启nfs服务
--------------------
$>sudo service nfs-kernel-server restart


(5)测试
-----------------------
$>sudo exportfs
/var/uplook	<world>
可以看到，服务器的/var/uplook目录对所有的机器都开放了


(6)客户端登录
----------------------
客户机通过输入服务器的IP地址和目录，可以把服务器的/var/uplook目录mount到本地的/mnt目录下：
$>sudo mount 10.211.55.10:/var/uplook /mnt
$>cd /mnt
可以看到，/mnt目录下的内容和服务器是一致的




2.FTP服务
======================
ftp服务用于文件下载;

(1)安装ftp服务器
-------------------
$>sudo apt install vsftpd
安装完后，ftp的配置文件为/etc/vsftpd.conf;

可以看到, 系统中多了ftp用户组和ftp用户:
$>cat /etc/group
$>cat /etc/passwd


(2)FTP服务的启动和关闭
-------------------
可以使用下列命令来打开，关闭，重启ftp服务;
$>sudo /etc/init.d/vsftpd start
$>sudo /etc/init.d/vsftpd stop
$>sudo /etc/init.d/vsftpd restart

ftp服务器的文件共享目录为/srv/ftp， 这也是匿名用户访问时的根目录;
将需要共享的文件放到/srv/ftp目录下;


(3)FTP服务器的配置
-------------------
编辑/etc/vsftpd.conf文件, 可以改变FTP服务器的配置;
改变配置后,重新启动ftp服务让新的配置生效;

$>sudo /etc/vsftpd.conf
//允许匿名用户登录
anonymous_enable=YES

//允许本地用户登录
local_enable=YES

//开启全局上传
write_enable=YES

//允许匿名用户上传文件
anon_upload_enable=YES  

//允许匿名用户新建文件夹
anon_mkdir_write_enable=YES


(4)创建匿名用户上传目录
----------------------
注意! 除非必要, 否则最好不要允许匿名用户上传文件;
如果要允许匿名上传, 可以创建特定的文件夹:

$>cd /srv/ftp
$>sudo mkdir file-up/
$>sudo chown ftp:ftp file-up/
$>ls -lh
drwxr-xr-x 13 zht zht 4.0K 11月 20 09:44 class-e6818
drwxr-xr-x  2 ftp ftp 4.0K 12月  4 17:35 file-up


(5)采用浏览器作为ftp客户端
--------------------------
打开浏览器,在地址栏输入:
ftp://localhost
或
ftp://127.0.0.1
可以看到, 根目录就是/srv/ftp目录, 可以点击目录下的文件下载;
采用浏览器作为ftp客户端时,无法支持下载整个目录;



3.lftp客户端
=======================
要想支持目录下载,断点续传等功能,可以使用lftp工具;
lftp是一个文件下载工具, 除了FTP协议外,lftp还支持FTPS,HTTP,HTTPS,HFTP,FISH,以及SFTP等协议;

(1)lftp的安装
--------------------
$>sudo apt install lftp


(2)登录到ftp服务器
---------------------
如果ftp服务器允许匿名用户登录,则可以不输入用户名,密码等;
登录到ftp服务器:
$>cd /home/zht
$>lftp 172.25.0.200	//172.25...为ftp服务器的IP地址

如果ftp服务器使用了非标准的端口号,则可以这样登录:
$>lftp 172.25.0.200:1234	//标准的ftp服务端口为21,可以不写


(3)ls和cd
----------------------
lftp登录成功后,可以使用ls和cd命令, 用法同普通控制台:
lftp 172.25.0.200:~>ls
drwxr-xr-x   13 1000     1000         4096 Nov 20 09:44 class-e6818
drwxr-xr-x    3 123      131          4096 Dec 04 17:49 file-up

lftp 172.25.0.200:/>cd class-e6818
lftp 172.25.0.200:/class-e6818>ls
drwxr-xr-x    5 1000     1000         4096 Aug 26 23:19 00arm
drwxrwxr-x    5 1000     1000         4096 Nov 20 09:45 01driver
...
lftp 172.25.0.200:/class-e6818> 


(4)单个文件的下载和上传
------------------------
可以使用pget命令下载文件, put命令上传文件;
注意!要上传文件必须使能ftp服务器的匿名上传功能,且目录的属主和组为ftp:ftp;

文件下载:
lftp 172.25.0.200:/class-e6818/00arm/books>pget arm-history.pdf
//将ftp服务器上的文件arm-history.pdf下载到本地
//默认下载到登录lftp时所在的目录

文件上传:
lftp 172.25.0.200:/file-up>put abc.pdf
lftp 172.25.0.200:/file-up>put /home/zht/abc.pdf
//如果没有路径,则默认将当前目录的文件拷贝到ftp服务器
//也可以采用绝对路径, /home/zht为ftp客户端的路径


(5)整个目录的下载和上传
-------------------------
可以使用mirror命令下载整个目录;
目录下载:
lftp 172.25.0.200:/class-e6818/>mirror 00arm/
lftp 172.25.0.200:/class-e6818/>mirror 00arm/ /home/zht
//可以下载到客户端的当前目录,也可以到指定目录

目录上传:
lftp 172.25.0.200:/file-up>mirror -R /home/zht/misc 
//将客户端的/home/zht/misc目录拷贝到服务器的file-up目录下
//注意!测试时创建目录成功,但是在misc目录下创建文件失败


(6)退出
---------------------------
lftp 172.25.0.200:/file-up>exit
或:
lftp 172.25.0.200:/file-up>quit



4.tftp服务
======================
TFTP是简化版的FTP协议，常用于开发板的文件下载；

(1)安装tftpd
---------------------
sudo apt install tftpd-hpa

(2)创建tftp共享目录
---------------------
$>mkdir /var/uplook/tftp
$>chmod 755 /var/uplook/tftp/
$>cp logo.bmp /var/uplook/tftp/
//拷贝要共享的文件到共享目录

(3)修改配置文件
---------------------
$>sudo vim /etc/default/tftpd-hpa
TFTP_USERNAME="tftp"
TFTP_ADDRESS="0.0.0.0:69"
TFTP_DIRECTORY="/var/uplook/tftp/"
TFTP_OPTIONS="-l -c -s"

(4)重启tftp服务
-------------------------
$>sudo /etc/init.d/tftpd-hpa restart



5. VNC服务
==========================
如果需要共享桌面的客户机较少,那么可以使用Ubuntu自带的"远程桌面";
但如果要连接的客户较多,则会发现客户机的卡顿情况比较严重, 此时最好采用商用版的VNC-View和VNC-Server;

(1)启动VNC服务器
-------------------
点击屏幕左面图标最上面的一个(搜索计算机)，找到"桌面共享"；
勾选允许其他人查看你的桌面，不需密码；


(2)启动VNC客户端
------------------
在搜索中找到"Remmina远程桌面客户端"，启动;
通讯协议选择"VNC";
输入服务器的IP地址后，即可显示服务器的桌面；
如果无法连接，请先确认是否可以ping到服务器；

如果上述的默认VNC服务不好用,那么可以安装并启用下面的VNC服务:


(3)启动VNC-Viewer
-------------------------
VNC-View在客户端运行, 也就是在学生机上运行;
VNC-Viewer的安装:
$>cd /srv/ftp/class-e6818/tools/RealVNC
$>sudo dpkg -i VNC-Viewer-5.3.2-Linux-x64.deb

VNC-Viewer的启动:
打开屏幕左边的搜索栏, 输入VNC-Viewer后, 点击启动即可;
登录后,输入服务器的IP(也就是教师机的IP), 端口号(5902)以及密码(123), 就可以通过远程桌面观察教师机的运行状态了;


(4)启动VNC-Server
--------------------------
注意!如果是学生机就没必要安装了;
教师机需要安装VNC-Server, 向学生机提供远程桌面服务;
安装:
$>cd /srv/ftp/class-e6818/tools/
$>sudo dpkg -i VNC-Server-5.3.2-Linux-x64.deb

在安装服务器时, 需要输入注册码, 可以试试下面的这些;
如果不行,到baidu上搜一个能用的;

B7SLM-7MAX5-B4M74-UTDBE-K5WFA
7736G-H4YBP-6ZCEU-AM3H4-QBTWA
NUQQ6-UP89V-9XGPU-GUYU9-U6VTA

3YHED-MNEHC-RMJT5-4UAAK-6A5HA
FBV9V-7Z3V9-MED3U-47SEU-85T3A
HA7MG-J8R3J-R3528-HC6P5-HTQ6A

启动:
VNC-Server安装后,会在屏幕左边栏增加一个图标;
点击图标即可启动VNC服务;


(5)VNC-Server的配置
------------------------
主要应该包括如下配置:
Security-->
	Authentication: VNC password
	Encryption: Always on

Users & Permissions-->
	点击Add, 加入一个Standard user,用于学生机登录
	User类型为View-only user, 密码为"123"
	当学生机上运行VNC-Viewer时, 密码即为"123"

Connectivity-->
	选中Allow VNC connections over TCP, 端口号为5902
	选中Serve VNC Viewer for Java, 端口号为5802
	添加rule, ACtion为"Accept", IPv4地址/掩码为"Default"

Privacy-->
	选中Notify when users connect and disconnect
	选中Disconnect users who have been idle for an extended period

配置完Server以后, 重启Server后, 就可以连接VNC-Viewer了;


