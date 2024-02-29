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

#define C_MAX_PACK_SIZE 65535





std::string getValueFromIni(const std::string& filePath, const std::string& section, const std::string& key);
void SetConfig(std::string input_str);
int ClientInit(void);
int CloseClient(void);
int NewThread(void* (*func)(void*), void* args);
void* CreateRecvThread(void*args);
int RegisterFrame(void);
void handle(unsigned char *buf, int len);

#endif
