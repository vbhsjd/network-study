#include<sys/types.h>      
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int listenfd; // 监听套接字
	struct sockaddr_in server_addr,client_addr;
	
	memset(&server_addr,0,sizeof(server_addr));
	memset(&client_addr,0,sizeof(client_addr));
	
	//创建一个数据报套接口：协议族为ipv4。
	if((listenfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	//函数声明在#include <sys/types.h>      #include <sys/socket.h>中
	{
		printf("%s\n",strerror(errno));
	}
	
	int opt=1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	//接下来的3行是做绑定前的参数处理：进程将该套接口的地址结构传递给内核
	server_addr.sin_family = AF_INET;//协议族为ipv4
	server_addr.sin_port=htons(8888);//端口号为8888，htons()函数是将主机端字节序列转换为网络端字节序列
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR-ANY：0，内核选择ip地址
	if(bind(listenfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1)//绑定端口信息
	{
		printf("%s\n",strerror(errno));
		exit(1);
	}
/*	
	该段注释表示TCP套接口和UDP套接口的区别
	
	//将套接字转换为被动套接字 即：从closed状态转变为listen状态
	if(listen(listenfd,128)==-1)//设置请求队列中最大链接数为128，次数字可以随便设置
	{
		printf("%s\n",strerror(errno));
		exit(1);
	}
	
	//接受客户端的链接请求：该函数为阻塞函数，若没有客户端链接请求将会一直阻塞
	socklen_t len=sizeof(client_addr);
	if((connectfd=accept(listenfd,(struct sockaddr *)&client_addr,&len))==-1)
	{
		printf("%s\n",strerror(errno));
		exit(1);
	}
*/	
	int num = 0;
	char buf[1024] = {0};
	while(1){
		socklen_t len = sizeof(client_addr);
		//从客户端接受消息存放到buf缓冲区
		num = recvfrom(listenfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&len);
		printf("you got a message:\"%s\" from client's \n ip is %s\n port is %d\n",
				buf,inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));
		// 下面一行是把接受的消息,以及客户端的ip和端口都存进buf里
		sprintf(buf,"%s from ip:%s port:%d",buf,inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));
		sendto(listenfd,buf,strlen(buf) + 1,0,(struct sockaddr *)&client_addr,len);
		buf[strlen(buf)] = 0;
	}
	close(listenfd);//关闭套接字
	return 0;
}
