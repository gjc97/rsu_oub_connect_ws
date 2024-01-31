#ifndef UDP_CONNECT_RSU_H
#define UDP_CONNECT_RSU_H



#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
class UDP_RSU
{
public:
    UDP_RSU();

    int serverSocket;
    int clientSocket;

    struct sockaddr_in Server; // 创建服务端sockaddr_in结构体
    struct sockaddr_in Client; // 创建客户端sockaddr_in结构体
    


};

#endif