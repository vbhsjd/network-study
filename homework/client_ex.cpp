#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>
using std::cout;
using std::endl;

int main(){
	int servfd = socket(AF_INET,SOCK_STREAM,0);
	if(servfd == -1){
		perror("socket error");
		return -1;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(2345);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(servfd,(sockaddr*)&servAddr,sizeof(servAddr)) == -1){
		perror("connect error");
		return -1;
	}
	unsigned int arr[10]; // 1,2,3...10
	for(int i = 0;i < 10;++i) arr[i] = i+1;
	int sum; // sum每次从server传过来,不需要每次都初始化
	while(1){
		// init arr	
		for(int i = 0;i < 10;++i) arr[i] = i+1;
		// 把数组从主机字节序转换成网络字节序
		for(int i = 0;i < 10;++i){
			arr[i] = htonl(arr[i]);
		}
		// 发送数组
		if(send(servfd,&arr,sizeof(arr),0) == -1){
			perror("send error");
			close(servfd);
			return -1;
		}

		// 接受服务器响应 接受到的应该是一个整数 而且要转换字节序
		int recvLen = recv(servfd,&sum,sizeof(sum),0);
		sum = ntohl(sum); // 转换字节序
		if(recvLen == 0){
			std::cout << "服务器关闭了连接.\n";
			break;
		} else if(recvLen == -1){
			perror("recv error");
			close(servfd);
			return -1;
		}
		cout << "接收到服务器回复数组和为:" << sum << endl;
	}
	close(servfd);
	return 0;
}
