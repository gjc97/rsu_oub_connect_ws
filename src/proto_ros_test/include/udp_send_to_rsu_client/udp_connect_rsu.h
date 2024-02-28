#ifndef UDP_CONNECT_RSU_H
#define UDP_CONNECT_RSU_H



#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <map>
#include <utility>

#include "udp_send_to_rsu_client/rsu_message.h"

class UDP_RSU
{
public:
    UDP_RSU();
    void recvData();
    std::string getValueFromIni(const std::string& filePath, const std::string& section, const std::string& key);
    int sock_fd;

    struct sockaddr_in dst_addr;
    struct sockaddr_in local_addr;

    // std::string strLocalIP = "192.168.20.226";
    std::string strLocalIP = "127.0.0.1";
    int nLocalPort = 10087;

    std::string strIP = "192.168.20.224";
    // std::string strIP = "127.0.0.1";

    int nPort = 10086;

    //感知数据来源map
    std::map<std::string,unsigned char> perception_type;

};

#endif