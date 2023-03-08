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

	char recvBuf[128] = {0};
	while(1){
		char msg[128];
		scanf("%s",msg);
		if(strcmp(msg,"quit") == 0){ // 输入为"quit",退出
			close(servfd);
			exit(0);
		} // if
		if(send(servfd,msg,strlen(msg)+1,0) == -1){
			perror("send error");
			close(servfd);
			return -1;
		}
		// 接受服务器响应
		int recvLen = recv(servfd,recvBuf,sizeof(recvBuf),0);
		if(recvLen == 0){
			std::cout << "服务器关闭了连接.\n";
			break;
		} else if(recvLen < 0){
			perror("recv error");
			close(servfd);
			return -1;
		}
		cout << "recv from server:" << recvBuf << endl;
	}
	close(servfd);
	return 0;
}
