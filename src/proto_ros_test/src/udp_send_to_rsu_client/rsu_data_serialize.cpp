
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include "udp_send_to_rsu_client/rsu_data_serialize.h"
#include "udp_send_to_rsu_client/rsu_message.h"


RsuDataProto::RsuDataProto()
{

}





void RsuDataProto::DataSerialize()
{
    
}

void RsuDataProto::DataDeserialize()
{
    
}

void RsuDataProto::GetUTC()
{


}
/**
 * @brief 返回时间戳
 * 
 * @param return_time_stamp_type 1：ms ,2:cs,3: ns//目前只有1可用
 * @return int 
 */
int RsuDataProto::GetTimeStamp(int return_time_stamp_type = 1)
{
     std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    struct std::tm* now_tm = std::localtime(&now_time_t);
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%F %T", now_tm);
    std::ostringstream ss;
    ss.fill('0');
    std::chrono::milliseconds ms;
    std::chrono::microseconds cs;
    std::chrono::nanoseconds ns;
    switch (return_time_stamp_type)
    {
    case 0:
        ss << buffer;//输出原始字符串
        break;
    case 1:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        return ms.count();
        break;
    case 2:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        return std::stoi(ss.str());
        break;
    case 3:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
        // ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000 << ":" << ns.count() % 1000;//输出到ns
        return std::stoi(ss.str());
        break;
    default:
        return 0;
        break;
    }
    return 0;
    

}

std::string RsuDataProto::GetCurrentTimeStamp(nebulalink::perceptron3::TimeBase & input_time_base)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();//获取当前系统时间
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);//系统时间转换为纪元时间，
    struct std::tm* now_tm = std::localtime(&now_time_t);//转换为本地时间结构体
    input_time_base.set_year(now_tm->tm_year + 1900);
    input_time_base.set_month(now_tm->tm_mon);
    input_time_base.set_day(now_tm->tm_mday);
    input_time_base.set_hour(now_tm->tm_hour);
    input_time_base.set_min(now_tm->tm_min);
    input_time_base.set_second(now_tm->tm_sec);

    std::chrono::milliseconds miil_second;
    miil_second = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    input_time_base.set_miilsecond(miil_second.count());
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%F %T", now_tm);//使用strftime对时间进行格式化
    //使用%F %T(完整的年月日，完成的时分秒）中定义的格式规则，格式化结构now_tm表示的时间，并存储在buffer中
    std::ostringstream ss;
    ss.fill('0');
    std::chrono::milliseconds ms;
    std::chrono::microseconds cs;
    std::chrono::nanoseconds ns;
    int time_stamp_type = 3;
    switch (time_stamp_type)
    {
    case 0:
        ss << buffer;//输出原始字符串
        break;
    case 1:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        ss << buffer << ":" << ms.count();//输出ms
        break;
    case 2:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000;//输出到cs
        break;
    case 3:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
        ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
        ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000 << ":" << ns.count() % 1000;//输出到ns
        break;
    default:
        ss << buffer;
        break;
    }
    return ss.str();
}

void RsuDataProto::SerializeTimeBase(nebulalink::perceptron3::TimeBase &input_time_base)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now(); // 获取当前系统时间
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);//系统时间转换为纪元时间，
    struct std::tm* now_tm = std::localtime(&now_time_t);//转换为本地时间结构体
    input_time_base.set_year(now_tm->tm_year + 1900);
    input_time_base.set_month(now_tm->tm_mon);
    input_time_base.set_day(now_tm->tm_mday);
    input_time_base.set_hour(now_tm->tm_hour);
    input_time_base.set_min(now_tm->tm_min);
    input_time_base.set_second(now_tm->tm_sec);

    std::chrono::milliseconds miil_second;
    miil_second = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    input_time_base.set_miilsecond(miil_second.count());
    return;
}






nebulalink::perceptron3::Point3 RsuDataProto::SerializePoint3()
{
    nebulalink::perceptron3::Point3 return_point3;
    return_point3.set_x(1.0);
    return_point3.set_y(1.1);
    return_point3.set_z(1.2);
    return return_point3;
}


void RsuDataProto::SerializePoint3(nebulalink::perceptron3::Point3 *input_point3)
{
    
    input_point3->set_x(1.1);
    input_point3->set_y(1.2);
    input_point3->set_z(1.3);
    return;
}


void RsuDataProto::SerializeSpeed3(nebulalink::perceptron3::Speed3 *input_speed3)
{
    
    input_speed3->set_speed_x(1.5);
    input_speed3->set_speed_y(1.6);
    input_speed3->set_speed_z(1.7);
    return;
}
void RsuDataProto::SerializeTargetSize(nebulalink::perceptron3::TargetSize *input_target_size)
{
    
    input_target_size->set_object_width(1.8);
    input_target_size->set_object_length(1.9);
    input_target_size->set_object_height(1.99);
    return;
}

void RsuDataProto::SerializePointGPS(nebulalink::perceptron3::PointGPS *input_point_gps)
{
   
    input_point_gps->set_object_longitude(117.1234);// 经度.范围0-180
    input_point_gps->set_object_latitude(23.1234);// 维度.范围0-90
    input_point_gps->set_object_elevation(1.1234);
    return;
}

 void RsuDataProto::SerializeTargetSizeCFD(nebulalink::perceptron3::TargetSize_CFD *input_target_size_cfd)
 {
    input_target_size_cfd->set_object_width_cfd(0);
    input_target_size_cfd->set_object_length_cfd(0);
    input_target_size_cfd->set_object_height_cfd(0);
    return;
 }
 /**
  * @brief 序列化Perceptron消息
  *
  * @return nebulalink::perceptron3::Perceptron 返回序列化后的Perceptron消息
  */
 nebulalink::perceptron3::Perceptron RsuDataProto::SerializePerceptron()
 {
     nebulalink::perceptron3::Perceptron return_obstacles_info;
     return_obstacles_info.set_is_tracker(true);
     return_obstacles_info.set_object_confidence(9.9);
     return_obstacles_info.set_lane_id("2");
     return_obstacles_info.set_object_class_type(1);
     return_obstacles_info.set_object_id(2);
     return_obstacles_info.set_object_speed(30.0);
     this->SerializeSpeed3(return_obstacles_info.mutable_speed3f());
     return_obstacles_info.set_object_acceleration(2.0);
     this->SerializeTargetSize(return_obstacles_info.mutable_target_size());
     this->SerializePointGPS(return_obstacles_info.mutable_point_gps());
     return_obstacles_info.set_object_ns(0);
     return_obstacles_info.set_object_we(0);
     return_obstacles_info.set_object_heading(145.9);
     return_obstacles_info.set_obj_time_stamp(this->GetTimeStamp(1));
     return_obstacles_info.set_ptc_sourcetype(0x04);
     this->SerializeTimeBase(*(return_obstacles_info.mutable_ptc_time_stamp()));
     return_obstacles_info.set_ptc_veh_type(10);
     this->SerializeTargetSizeCFD(return_obstacles_info.mutable_ptc_size_cfd());
     return_obstacles_info.set_is_head_tail(1);
     return_obstacles_info.set_lane_type(1);
     this->SerializePoint3(return_obstacles_info.mutable_point3f());
     return return_obstacles_info;
}


void RsuDataProto::SerializePerceptron(nebulalink::perceptron3::Perceptron &input_obstacles_info)
{

    input_obstacles_info.set_is_tracker(true);
    input_obstacles_info.set_object_confidence(9.9);
    input_obstacles_info.set_lane_id("2");
    input_obstacles_info.set_object_class_type(1);
    input_obstacles_info.set_object_id(2);
    input_obstacles_info.set_object_speed(30.0);
    this->SerializeSpeed3(input_obstacles_info.mutable_speed3f());
    input_obstacles_info.set_object_acceleration(2.0);
    this->SerializeTargetSize(input_obstacles_info.mutable_target_size());
    this->SerializePointGPS(input_obstacles_info.mutable_point_gps());
    input_obstacles_info.set_object_ns(0);
    input_obstacles_info.set_object_we(0);
    input_obstacles_info.set_object_heading(145.9);
    input_obstacles_info.set_obj_time_stamp(this->GetTimeStamp(1));
    input_obstacles_info.set_ptc_sourcetype(0x04);
    this->SerializeTimeBase(*(input_obstacles_info.mutable_ptc_time_stamp()));
    input_obstacles_info.set_ptc_veh_type(10);
    this->SerializeTargetSizeCFD(input_obstacles_info.mutable_ptc_size_cfd());
    input_obstacles_info.set_is_head_tail(1);
    input_obstacles_info.set_lane_type(1);
    this->SerializePoint3(input_obstacles_info.mutable_point3f());

    return;
}

nebulalink::perceptron3::FrameArray RsuDataProto::SerializeFrameArrayTest()
{
    //逐个添加元素
    nebulalink::perceptron3::Perceptron temp_obstacles_info;//后续需要设置其属性
    nebulalink::perceptron3::FrameArray return_frame_array;
    return_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //整个repeated赋值
    nebulalink::perceptron3::FrameArray return_frame_array1;
    //添加一些元素到return_frame_array1的Perceptron中
    nebulalink::perceptron3::Perceptron temp_obstacles_info1;
    nebulalink::perceptron3::Perceptron temp_obstacles_info2;
    nebulalink::perceptron3::Perceptron temp_obstacles_info3;
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info1);
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info2);
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info3);
    //直接将整个return_frame_array1的perceptron()赋值到return_frame_array
    return_frame_array.mutable_perceptron()->CopyFrom(return_frame_array1.perceptron());
    //先copy再add才可以
    return_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //获得大小
    std::cout << "size of return_frame_array:" << return_frame_array.perceptron_size() <<std::endl;

    
    return return_frame_array;
}


nebulalink::perceptron3::FrameArray RsuDataProto::SerializeFrameArray()
{
    //逐个添加元素
    nebulalink::perceptron3::Perceptron temp_obstacles_info;//后续需要设置其属性
    nebulalink::perceptron3::FrameArray return_frame_array;
    return_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //整个repeated赋值
    nebulalink::perceptron3::FrameArray return_frame_array1;
    //添加一些元素到return_frame_array1的Perceptron中
    nebulalink::perceptron3::Perceptron temp_obstacles_info1;
    nebulalink::perceptron3::Perceptron temp_obstacles_info2;
    nebulalink::perceptron3::Perceptron temp_obstacles_info3;
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info1);
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info2);
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info3);
    //直接将整个return_frame_array1的perceptron()赋值到return_frame_array
    return_frame_array.mutable_perceptron()->CopyFrom(return_frame_array1.perceptron());
    //先copy再add才可以
    return_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //获得大小
    std::cout << "size of return_frame_array:" << return_frame_array.perceptron_size() <<std::endl;

    
    return return_frame_array;
}
void RsuDataProto::SerializeFrameArray(nebulalink::perceptron3::FrameArray &input_frame_array)
{


        //逐个添加元素
    nebulalink::perceptron3::Perceptron temp_obstacles_info;//后续需要设置其属性
    nebulalink::perceptron3::FrameArray return_frame_array;
    input_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //整个repeated赋值
    nebulalink::perceptron3::FrameArray return_frame_array1;
    //添加一些元素到return_frame_array1的Perceptron中
    nebulalink::perceptron3::Perceptron temp_obstacles_info1;
    nebulalink::perceptron3::Perceptron temp_obstacles_info2;
    nebulalink::perceptron3::Perceptron temp_obstacles_info3;
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info1);
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info2);
    return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info3);
    //直接将整个return_frame_array1的perceptron()赋值到return_frame_array
    input_frame_array.mutable_perceptron()->CopyFrom(return_frame_array1.perceptron());
    //先copy再add才可以
    input_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //获得大小
    std::cout << "size of input_frame_array:" << input_frame_array.perceptron_size() <<std::endl;
    nebulalink::perceptron3::FrameArray a;
    return;
}