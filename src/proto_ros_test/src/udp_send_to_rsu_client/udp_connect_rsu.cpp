#include <fstream>

#include "udp_send_to_rsu_client/udp_connect_rsu.h"

UDP_RSU::UDP_RSU()
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
	//*************************************************************************************************************//


    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = inet_addr(strIP.c_str());
    dst_addr.sin_port = htons(nPort);
     //********************************发送的ip地址**************************************

}

void UDP_RSU::recvData()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    #define MAX_LEN 64
    char buf[MAX_LEN];
    int ret = recvfrom(sock_fd, buf, sizeof(buf) - 1, MSG_DONTWAIT, (struct sockaddr *)&addr, &len);
    if (ret < 0)
    {
        std::cout << "not received" << std::endl;
    }
    else
    {
        char buf1[64] = {0};
        // memcpy(&buf1, buf, sizeof(buf));
        std::cout << "receive data: " <<  buf[2] << std::endl;
    
    }
}


std::string UDP_RSU::getValueFromIni(const std::string& filePath, const std::string& section, const std::string& key) 
{
    std::ifstream file(filePath);
    std::string line;
    std::string value;
    bool sectionFound = false;

    while (std::getline(file, line))
    {
        if (!sectionFound && line == "[" + section + "]")
        {
            sectionFound = true;
        }
        else if (sectionFound)
        {
            size_t found = line.find(key);
            if (found != std::string::npos)
            {
                size_t equalsPos = line.find("=");
                value = line.substr(equalsPos + 1);
                break;
            }
        }
    }

    return value;
}


