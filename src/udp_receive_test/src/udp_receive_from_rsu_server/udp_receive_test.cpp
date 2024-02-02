#include "udp_receive_from_rsu_server/udp_receive_test.h"

UDP_RECEIVE::UDP_RECEIVE()
{
	//建立一个数据报类型的UDP套接字  ******************//

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		std::cout << "Create socket failed...";
		return;
	}
    int on = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	   //本机地址
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(strLocalIP.c_str());
    local_addr.sin_port = htons(nLocalPort);
    int ret = bind(sock_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    if(ret < 0)
    {
        std::cout << "=====================bind socket erro================="<< std::endl;;
        close(sock_fd);
        return ;
    }
}

void UDP_RECEIVE::recvData()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    #define MAX_LEN 64
    char buf[MAX_LEN] = {0};
    int ret = recvfrom(sock_fd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&addr, &len);
    if (ret < 0)
    {
        std::cout << "receive bad" << std::endl;
    }
    else
    {
        std::cout << "receive data: " << buf << std::endl;
    }
}










