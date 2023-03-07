#include<iostream>
#include<arpa/inet.h>

int main(){
	// htons
	auto port = htons(2345);
	std::cout << "2345 port in net:" << port << std::endl;

	// inet_addr 将网络地址转换为二进制数字
	auto ip1 = inet_addr("127.0.0.1");
	std::cout << "127.0.0.1 in net:" << ip1 << std::endl;

	// inet_aton 网络地址从字符串转换为二进制数字
	sockaddr_in clntAddr;
	auto ip2 = inet_aton("127.0.0.1",&clntAddr.sin_addr);
	std::cout << "127.0.0.1 in net:" << ip1 << std::endl;

	// inet_ntoa 把二进制数字转换成字符串格式
	// 用于输出client的ip
	auto str = inet_ntoa(clntAddr.sin_addr);
	std::cout << "inet_atoa:" << str << std::endl;
}
