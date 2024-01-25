#include <udp_connect_rsu.h>


UDP_RSU::UDP_RSU()
{

	//建立一个数据报类型的UDP套接字  ******************//
	serverSocket = socket(PF_INET, SOCK_DGRAM, 0); //配置模式，
	//设置服务器地址addrSrv和监听端口
	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = inet_addr("127.0.0.1"); //设置服务器主机ip地址（与接收方客户端的IP对应）
	Server.sin_port = htons(8001);					 //发送用的端口，可以根据需要更改


	//使用bind（）函数绑定监听端口，将socket文件描述符sockSrv与地址类型变量（struct sockaddr_in ）进行绑定
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	bind(serverSocket, (sockaddr *)&Server, sizeof(sockaddr));//不使用此行，也可以发送,

	//*************************************************************************************************************//

	clientSocket = socket(PF_INET, SOCK_DGRAM, 0); //配置模式，
	Client.sin_family = AF_INET;
	Client.sin_addr.s_addr = inet_addr("127.0.0.4");//要发送到的ip地址，根据rsu修改
     //********************************发送的ip地址**************************************
	Client.sin_port = htons(10086);

}



