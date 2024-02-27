#include <iostream>
#include <string>
#include "test_protobuf.pb.h"

#include "nebulalink_perceptron.pb.h"
#include <ros/ros.h>

#include "udp_send_to_rsu_client/udp_connect_rsu.h"
#include "udp_send_to_rsu_client/rsu_data_serialize.h"


#define C_MAX_PACK_SIZE  30000

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

    unsigned char sendPointData[128] = {0};

    unsigned char send_buf[C_MAX_PACK_SIZE] = {0};
    char serialize_buf[C_MAX_PACK_SIZE] = {0};

    char deserialize_buf[C_MAX_PACK_SIZE] = {0};

    RsuDataProto rsu_data_proto;

    nebulalink::perceptron3::TimeBase obs_time_base;

    nebulalink::perceptron3::Perceptron perceptron_data;
    nebulalink::perceptron3::PerceptronSet perceptronset_data;
    rsu_data_ns::Perceptron perceptron_struct;
    rsu_data_ns::PerceptronSet perceptronset_struct;



    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        //1.填充自定义结构体
        rsu_data_proto.GeneratePerceptronSetStruct(perceptronset_struct);
        perceptronset_data.Clear();//清空对象
        rsu_data_proto.SerializePerceptronSet(perceptronset_data,perceptronset_struct);
  
        //序列化占用的字节数
        int data_length = 0;
        //是否序列化成功
        bool is_serialized =  false;
        // test*************************************************
        if (0)
        {
            nebulalink::perceptron3::Perceptron de_obstacles_info;
            std::string str_obstacles_info;

            if (!(perceptron_data.SerializeToString(&str_obstacles_info)))
            {
                std::cout << "perceptron_data!序列化失败" << std::endl;
            }
            else
            {
                std::cout << "perceptron_data序列化成功" << std::endl;
            }

            if (!de_obstacles_info.ParseFromString(str_obstacles_info))
            {
                std::cerr << "obstacles_info反序列化失败" << std::endl;
            }
            else
            {
                std::cerr << "obstacles_info反序列化成功" << std::endl;
                std::cout << "de_obstacles_info->set_is_tracker:" << de_obstacles_info.is_tracker() << " de_obstacle_info->speed_x:" << de_obstacles_info.speed3f().speed_x() << std::endl;
            }
        }
        //test******************************************************

        if (!(perceptronset_data.SerializeToArray(&serialize_buf,C_MAX_PACK_SIZE)))
        {
            std::cout << "perceptronset_data序列化失败!" << std::endl;
            is_serialized = false;
            
        }
        else
        {
            is_serialized = true;
            data_length = perceptronset_data.ByteSize();//返回序列化所需要的字节数量
            std::cout << "perceptron_data序列化成功,占用字节为:" << data_length << std::endl;
        }
        NP3PERCEPTIONSET de_perceptronset_info;
        if (!de_perceptronset_info.ParseFromArray(&serialize_buf,data_length))
        {
            std::cerr << "de_perceptronset_info反序列化失败" << std::endl;
        }
        else
        {
            std::cerr << "de_perceptronset_info反序列化成功" << std::endl;
            std::cout << "de_perceptronset_info->set_is_tracker:" << de_perceptronset_info.time_stamp()
            <<  " de_perceptronset_info->perceptron->obj_time_stamp:" << 
            de_perceptronset_info.perceptron(de_perceptronset_info.perceptron_size() - 1).obj_time_stamp() <<std::endl;
        }

        // 填充udp发送的buf
        int buf_index = 0;
        if (is_serialized)
        {
            send_buf[buf_index++] = 0xDA;
            send_buf[buf_index++] = 0xDB;
            send_buf[buf_index++] = 0xDC;
            send_buf[buf_index++] = 0xDD;
            send_buf[buf_index++] = (*(udp_rsu->perception_type.find("camera"))).second;
            send_buf[buf_index++] = (data_length >> 8) & 0xff;
            send_buf[buf_index++] = data_length & 0xff;

            for (int i = 0; i < data_length; i++)
            {
                send_buf[buf_index++] = serialize_buf[i];
            }
        }

        std::cout << "length: " << data_length << std::endl;

        int send_lenght = sendto(udp_rsu->sock_fd, send_buf, sizeof(send_buf)+0, 0, (sockaddr *)&(udp_rsu->dst_addr), sizeof(udp_rsu->dst_addr));

        // if (send_lenght == -1)
        // {
        //     std::cout << "send fault!!!!!!!!" << std::endl; // 若发送失败。则返回-1
        //     return -1;
        // }
        // else
        // {
        //     std::cout << "send Success!!!!!!!sendto_len:  " << send_lenght << std::endl
        //               << std::endl;
        // }
        loop_rate.sleep();
    }
    // closesocket(sockClient);
    close(udp_rsu->sock_fd);
    return 0;
}