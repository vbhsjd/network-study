#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>

/*
   open fd最大是255
   可以通过ulimit -a查看,也可以改
 */

int main(){
	// test open
	while(0){
		int fd = open("tmp",O_CREAT|O_RDONLY,0644);
		if(fd == -1){
			perror("open error");
			break;
		}
		std::cout << "fd is " << fd << std::endl;
	}
	// test socket
	while(1){
		int fd = socket(AF_INET,SOCK_DGRAM,0);
		if(fd == -1){
			perror("socket error");
			exit(1);
		}
		std::cout << "socket fd is " << fd << std::endl;
	}
}
