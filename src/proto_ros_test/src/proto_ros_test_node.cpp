#include <iostream>
#include <string>
#include "test_protobuf.pb.h"

#include "nebulalink_perceptron.pb.h"
#include <ros/ros.h>


#include "udp_send_to_rsu_client/udp_connect_rsu.h"
#include "udp_send_to_rsu_client/rsu_data_serialize.h"


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
    unsigned char sendPointData[128] = {0};

    RsuDataProto rsu_data_proto;

    struct Point3 my_object_pos;
    my_object_pos.x = 1.0;
    my_object_pos.y = 2.0;
    my_object_pos.z = 3.0;

    struct TargetSize my_object_size;
    my_object_size.object_width = 1.1;
    my_object_size.object_length = 1.2;
    my_object_size.object_height = 1.3;

    struct PointGPS my_object_gps;
    my_object_gps.object_latitude = 23.32;
    my_object_gps.object_longitude = 123.321;
    my_object_gps.object_elevation = 13.31;

    // 序列化时间戳
    nebulalink::perceptron3::TimeBase obs_time_base;
    rsu_data_proto.GetCurrentTimeStamp(obs_time_base);
    std::string str_obstacles_info;


    nebulalink::perceptron3::Perceptron obstacles_info = rsu_data_proto.SerializePerceptron();

    if (!(obstacles_info.SerializeToString(&str_obstacles_info)))
    {
        std::cout << "序列化失败!" << std::endl;
    }
    else
    {
        std::cout << "序列化成功" << std::endl;
    }
    nebulalink::perceptron3::Perceptron de_obstacles_info;
    if (!de_obstacles_info.ParseFromString(str_obstacles_info))
    {
        std::cerr << "反序列化失败" << std::endl;
    }
    else
    {
        std::cerr << "反序列化成功" << std::endl;
        std::cout << "de_obstacles_info->set_is_tracker:" << de_obstacles_info.is_tracker() <<
        " de_obstacle_info->set_point3:" <<de_obstacles_info.point3f().x()<< std::endl;
    }

    rsu_data_proto.SerializeFrameArray();
    //
    rsu_data_proto.SerializeFrameArray();
    

    ros::Rate loop_rate(10);
	while (0)
	{

		// 组建协议
		memset(sendPointData, 0, sizeof(sendPointData));

		// 帧头
		sendPointData[0] = 0xDA;
        sendPointData[1] = 0xDB;
		sendPointData[2] = 0xDC;
		sendPointData[3] = 0xDD;
		//数据来源
		sendPointData[4] = (*(udp_rsu->perception_type.find("camera"))).second;
		std::cout <<"perception_type:" <<int(sendPointData[4]) << std::endl;
        std::cout << "time:" << rsu_data_proto.GetCurrentTimeStamp(obs_time_base) << std::endl;
        //数据长度

//***********************************反序列化测试****************************************

        if (1)
        {
            nebulalink::perceptron3::Perceptron obstacles_info1;

            obstacles_info1.set_is_tracker(true);
            obstacles_info1.mutable_ptc_time_stamp()->set_year(obs_time_base.year());

            std::string str_time_base;
            std::string str_obstacles_info;
            if (!(obstacles_info1.SerializeToString(&str_obstacles_info) && obs_time_base.SerializeToString(&str_time_base)))
            {
                std::cout << "序列化失败!" << std::endl;
            }
            else
            {
                std::cout << "序列化成功" << std::endl;
            }
            nebulalink::perceptron3::TimeBase de_obs_time_base;
            nebulalink::perceptron3::Perceptron de_obstacles_info;

            if (!(de_obstacles_info.ParseFromString(str_obstacles_info) && de_obs_time_base.ParseFromString(str_time_base)))
            {
                std::cerr << "反序列化成功" << std::endl;
            }
            std::cout << "反序列化成功！" << str_time_base << std::endl
                      << "year: " << de_obs_time_base.year()
                      << " month:" << de_obs_time_base.month()
                      << " day:" << de_obs_time_base.day()
                      << " hour:" << de_obs_time_base.hour()
                      << " min:" << de_obs_time_base.min()
                      << " second:" << de_obs_time_base.second()
                      << " ms:" << de_obs_time_base.miilsecond()
                      << " obstacles_info->object_confidence:" << de_obstacles_info.is_tracker() 
                      << " obstacles_info->ptc_time_stamp:" << de_obstacles_info.ptc_time_stamp().year() << std::endl;
        }

        int nDataLen = 0;
        sendPointData[5] = nDataLen;

        int t = sendto(udp_rsu->sock_fd, msg, sizeof(msg) + 1, 0, (sockaddr *)&(udp_rsu->dst_addr), sizeof(udp_rsu->dst_addr));

        if (t == -1)
        {
            std::cout << "send fault" << std::endl; // 若发送失败。则返回-1
        }
        else
        {
            std::cout << "sendto_len:  " << t << std::endl
                      << std::endl; // 若发送失败。则返回-1
        }
        loop_rate.sleep();
    }
    // closesocket(sockClient);
    close(udp_rsu->sock_fd);
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




