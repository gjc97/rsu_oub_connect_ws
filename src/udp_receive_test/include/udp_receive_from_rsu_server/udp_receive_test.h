#ifndef UDP_RECEIVE_TEST_H
#define UDP_RECEIVE_TEST_H



#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
class UDP_RECEIVE
{
public:
    UDP_RECEIVE();

    void recvData();

    int sock_fd;

    struct sockaddr_in dst_addr;
    struct sockaddr_in local_addr;

    std::string strLocalIP = "192.168.1.111";
    int nLocalPort = 10086;

    std::string strIP = "192.168.1.104";
    int nPort = 10086;
};

#endif