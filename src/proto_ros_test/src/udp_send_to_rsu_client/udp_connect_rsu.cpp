#include <udp_send_to_rsu_client/udp_connect_rsu.h>


UDP_RSU::UDP_RSU()
{

	//建立一个数据报类型的UDP套接字  ******************//
	// serverSocket = socket(PF_INET, SOCK_DGRAM, 0); //配置模式，
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0); //配置模式，参考供应商例子
	//设置服务器地址addrSrv和监听端口
	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = inet_addr("192.168.43.105"); //设置服务器主机ip地址（与接收方客户端的IP对应）
	Server.sin_port = htons(10086);					 //发送用的端口，可以根据需要更改


	//使用bind（）函数绑定监听端口，将socket文件描述符sockSrv与地址类型变量（struct sockaddr_in ）进行绑定
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int bind_ret = bind(serverSocket, (sockaddr *)&Server, sizeof(sockaddr));//不使用此行，也可以发送,
	

	std::cout << "bind_ret:" << bind_ret << std::endl;
	if (bind_ret < 0)
	{
		std::cout << "=====================bind socket erro================="<< std::endl;
		close(serverSocket);
		return;
	}

	//*************************************************************************************************************//

	clientSocket = socket(PF_INET, SOCK_DGRAM, 0); //配置模式，
	Client.sin_family = AF_INET;
	Client.sin_addr.s_addr = inet_addr("192.168.43.70");//要发送到的ip地址，根据rsu修改
     //********************************发送的ip地址**************************************
	Client.sin_port = htons(10086);

}




