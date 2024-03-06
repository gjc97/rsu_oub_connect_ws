#ifndef APP_H_
#define APP_H_

#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <inttypes.h>
#include <fstream>

#include <nebulalink.serverdayii.pb.h>

#include "obu_connect/print_fun.h"


#include <ros/ros.h>
#include <ros/package.h>
#define C_MAX_PACK_SIZE 65535

class PRINT_FUN;



class OBU_CLIENT
{
public:
    // std::string cwd = ros::package::getPath("udp_receive_test");
    // std::string inifile_path = cwd + "/config/send_config.ini";

    // int socket_fd_ = 0;
    // static sockaddr_in server_addr_;

    // std::string tcp_ip = getValueFromIni(inifile_path, "ClientSet", "ClientIP");
    // unsigned short tcp_port = std::atoi(getValueFromIni(inifile_path, "ClientSet", "ClientPort").c_str());


	PRINT_FUN *print_fun;
    
    int socket_fd_;
    sockaddr_in server_addr_;
    std::string tcp_ip;
    unsigned short tcp_port;

    bool tcp_connected;
    // unsigned short tcp_port = 10086;
public:
    OBU_CLIENT();

    
    std::string getValueFromIni(const std::string &filePath, const std::string &section, const std::string &key);
    void SetConfig(std::string input_str);
    int ClientInit(void);
    void Reconnect();
    int CloseClient(void);
    int NewThread(void *(*func)(void *), void *args);
    void *CreateRecvThread(void *args);
    int RegisterFrame(void);
    void handle(unsigned char *buf, int len);
};

#endif
