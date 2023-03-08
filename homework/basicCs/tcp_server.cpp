#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>

int main(){
	// ipv4 TCP
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		perror("socket error");
		return -1;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET; // ipv4
	//htons  host to net short 都转成大端序(网络字节序)
	servAddr.sin_port = htons(2345); // port 大端
	// inet_addr 网络地址转换成二进制
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip地址
	if (bind(sockfd,(sockaddr*)&servAddr,sizeof(servAddr)) == -1){
		perror("bind error");
		return -1;
	}
	// backlog = 128 请求队列最大长度128
	if(listen(sockfd,128) == -1){
		perror("listen error");
		return -1;
	}

	int clntfd = accept(sockfd,nullptr,nullptr);	
	if(clntfd == -1){
		perror("accept error");
		return -1;
	}

	char recvBuf[128] = {0};
	int ret;
	while(1){
		ret = recv(clntfd,recvBuf,128,0);
		if(ret == 0){
			std::cout << "客户端关闭了连接.\n";
			break;
		} else if(ret < 0){
			perror("recv error");
			close(sockfd);
			close(clntfd);
			return -1;
		}
		std::cout << "recv from client:" << recvBuf << std::endl;

		// 把接受到的消息发送回客户端
		if(send(clntfd,recvBuf,ret,0) == -1){
			perror("send error");
			close(sockfd);
			close(clntfd);
			return -1;
		}
	}
	close(sockfd);
	close(clntfd);
}
