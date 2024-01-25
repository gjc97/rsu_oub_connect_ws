#include <iostream>
#include <string>
#include "test_protobuf.pb.h"

#include "nebulalink.perceptron3.0.5.pb.h"
#include <ros/ros.h>


#include <udp_connect_rsu.h>


struct TEST
{
    int test;
};
struct INFO
{
    int year;
    int month;
    int day;
    TEST test_struct;
    double temperature;
};


int main(int argc, char **argv)
{
    ros::init(argc, argv, "proto_test");
    ros::NodeHandle nh("~");

    UDP_RSU *udp_rsu = new UDP_RSU();



   //*************************************************************************************************************//
	
	//创建发送和接受的结构体以及转化时用到的字符串，用作测试
	INFO sendinfo,recvinfo;
    char send_info_buf[100];
    char recv_info_buf[100];


	//对发送的结构体成员赋值
	sendinfo.year = 2021;
	sendinfo.month = 7;
	sendinfo.day = 27;
	sendinfo.test_struct.test = 10;
	sendinfo.temperature = 23.50;


    
    // std::string *sen_str = "hello";
    const char* msg = "Jane Doe"; //发送字符串
    // char send_info_buf1[100] =sen_str;
    // send_info_buf1 = "test";
	//将发送的结构体sendinfo转化为字符串send_info_buf
	// memcpy(send_info_buf1, &sen_str, sizeof(sen_str));
//************************************************************************************************************//


	while (ros::ok())
	{	
		usleep(2000000);
		//发送
			/*  
				int sendto(int s, const void* buf, int len, unsigned int flags, const struct sockaddr* to, int tolen);
				s:            接收端的socket描述符
				buf：    UDP数据报缓存区（包含待发送数据）
				len：     UDP数据报长度
				flags： 调用方式标志位（一般设置为0）
				to：      指向接收数据的主机地址信息的结构体，（sockaddr_in需类型转换）；
				tolen：所指的结构体的长度
			*/	
			//字符串发送和打印所用语句
			//int t = sendto(clientSocket, sendbuf, sizeof(sendbuf), 0, (sockaddr*)&Client, sizeof(Client));	
			//cout << "sendto_len:  "<<t << endl<< endl;//若发送失败。则返回-1	
			//结构体发送和打印所用语句
			int t = sendto(udp_rsu->clientSocket, msg, sizeof(msg)+1  , 0, (sockaddr*)&(udp_rsu->Client), sizeof(udp_rsu->Client));
			std::cout << "sendto_len:  "<<t << std::endl<< std::endl;//若发送失败。则返回-1

            std::cout << "size of client:  "<< sizeof(udp_rsu->Client)<< std::endl<< std::endl;//若发送失败。则返回-1

		//接收
			/*  
				int recvfrom(int s, void *buf, int len, unsigned int flags,struct sockaddr *from, int *fromlen);
				s:       socket描述符
				buf：    UDP数据报缓存区（包含所要接受数据）
				len：    缓存区长度
				flags：  调用方式标志位（一般设置为0）
				from：   指向发送数据的客户端地址信息的结构体，（sockaddr_in需类型转换）；
				fromlen：指针，指向from中结构体的长度
			*/

			//字符串接收和打印所用语句
			//socklen_t len = sizeof(sockaddr);
			//recvfrom(serverSocket, recvbuf, sizeof(recvbuf), 0, (sockaddr*)&Server, &len);
			//printf("%s\n\n", recvBuf);

			//结构体接收和打印所用语句
			//recvfrom(serverSocket, recv_info_buf, sizeof(recv_info_buf), 0, (sockaddr *)&Server, &len);
			//memcpy(&recvinfo, recv_info_buf, sizeof(recv_info_buf));
			//cout << "Today is  " << recvinfo.year << "." << recvinfo.month<<"." << recvinfo.day << endl;
			//cout << "The template is " << recvinfo.temperature << endl;
			//cout << "The struct-test number is " << recvinfo.test_struct.test << endl<< endl;
			
	}
	//closesocket(sockClient);
	close(udp_rsu->serverSocket);
	return 0;


    std::string people_str;
    {
        // 对一个联系人的信息使用PB进行序列化，并将结果打印出来
        contacts::PeopleInfo people;
        people.set_name("fl");
        people.set_age(22);
        if (!people.SerializeToString(&people_str))
        {
            std::cerr << "序列化联系人失败" << std::endl;
            return -1;
        }
        std::cout << "序列化成功，结果：" << people_str << std::endl;
    }

    {
        // 对序列化后的内容使用PB进行反序列化，解析出联系人信息并打印出来
        contacts::PeopleInfo people;
        if(!people.ParseFromString(people_str))
        {
            std::cerr << "反序列化联系人失败" << std::endl;
            return -1;
        }
        std::cout << "反序列化成功！" << people_str << std::endl
                  << "姓名： " << people.name() << std::endl
                  << "年龄： " << people.age() << std::endl;
    }
    // return 0;



    
}




