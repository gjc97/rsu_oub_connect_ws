
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include "udp_send_to_rsu_client/rsu_data_serialize.h"



RsuDataProto::RsuDataProto()
{

    perception_type.insert(std::pair<std::string, unsigned char>("unknown", 0x00));
    perception_type.insert(std::pair<std::string, unsigned char>("rsu_self", 0x01));
    perception_type.insert(std::pair<std::string, unsigned char>("v2x_broadcast", 0x02));
    perception_type.insert(std::pair<std::string, unsigned char>("camera", 0x03));
    perception_type.insert(std::pair<std::string, unsigned char>("radar", 0x04));
    perception_type.insert(std::pair<std::string, unsigned char>("ground", 0x05));
    perception_type.insert(std::pair<std::string, unsigned char>("lidar", 0x06));
    perception_type.insert(std::pair<std::string, unsigned char>("mec", 0x07));

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



void RsuDataProto::GeneratePerceptronStruct(struct rsu_data_ns::Perceptron &input_perceptron_struct)
{

    input_perceptron_struct.is_tracker = false;
    input_perceptron_struct.object_confidence = 9.9;
    input_perceptron_struct.lane_id = "2";
    input_perceptron_struct.object_class_type = 1;
    input_perceptron_struct.object_id = 2;
    input_perceptron_struct.object_speed = 30;
    input_perceptron_struct.speed3f.speed_x = 0.1;
    input_perceptron_struct.speed3f.speed_y = 0.1;
    input_perceptron_struct.speed3f.speed_z = 0.1;
    input_perceptron_struct.object_acceleration = 0.1;
    input_perceptron_struct.target_size.object_height = 0.7;
    input_perceptron_struct.target_size.object_length = 0.6;
    input_perceptron_struct.target_size.object_width = 0.5;
    input_perceptron_struct.point_gps.object_longitude = 100.123;
    input_perceptron_struct.point_gps.object_latitude = 45.123;
    input_perceptron_struct.point_gps.object_elevation = 100.123;
    input_perceptron_struct.object_NS = 0;
    input_perceptron_struct.object_WE = 0;
    input_perceptron_struct.is_head_tail = 1;
    input_perceptron_struct.object_heading = 145.9;
    input_perceptron_struct.obj_time_stamp = this->getMiilsecondTimeStamp(1);
    input_perceptron_struct.ptc_sourcetype = (*(perception_type.find("camera"))).second;
    this->getUTCTimeBase(input_perceptron_struct.ptc_time_stamp);
    input_perceptron_struct.ptc_veh_type = 10;
    input_perceptron_struct.ptc_size_cfd.object_height_cfd = 0.5;
    input_perceptron_struct.ptc_size_cfd.object_length_cfd = 0;
    input_perceptron_struct.ptc_size_cfd.object_width_cfd = 0;
    return;
}

void RsuDataProto::GeneratePerceptronSetStruct(struct rsu_data_ns::PerceptronSet &input_perceptronset_struct)
{
    input_perceptronset_struct.devide_id = "test0001";
    input_perceptronset_struct.devide_is_true = true;
    //utc时间ms
    input_perceptronset_struct.time_stamp = this->getMiilsecondTimeStamp(2);
    input_perceptronset_struct.number_frame = 999;
    input_perceptronset_struct.perception_gps.object_longitude = 112.1234;
    input_perceptronset_struct.perception_gps.object_latitude = 43.1234;
    input_perceptronset_struct.perception_gps.object_elevation = 10.1234;
    this->GeneratePerceptronStruct(input_perceptronset_struct.perceptron);
    return;

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

/**
 * @brief 返回时间戳字符串
 * 
 * @param input_time_base 
 * @return std::string 
 */
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


/**
 * @brief 对时间时间戳tm类型时间戳进行格式化和赋值
 * 
 * @param input_time_base 
 */
void RsuDataProto::getUTCTimeBase(rsu_data_ns::TimeBase &input_time_base)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now(); // 获取当前系统时间
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);//系统时间转换为纪元时间，
    struct std::tm* now_tm = std::gmtime(&now_time_t);//转换为本地时间结构体//localtime
    input_time_base.year = (now_tm->tm_year + 1900);
    input_time_base.month = (now_tm->tm_mon);
    input_time_base.day = (now_tm->tm_mday);
    input_time_base.hour = (now_tm->tm_hour);
    input_time_base.min = (now_tm->tm_min);
    input_time_base.second = (now_tm->tm_sec);

    std::chrono::milliseconds miil_second;
    miil_second = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    input_time_base.miilsecond = (miil_second.count());
    return;
}
/**
 * @brief 返回毫秒级时间戳
 * 
 * @param return_type，返回的时间戳类型 
 * @return int64 
 */
int64 RsuDataProto::getMiilsecondTimeStamp(int return_type = 0)
{

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now(); // 获取当前系统时间
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);//系统时间转换为纪元时间，
    struct std::tm* now_tm = std::localtime(&now_time_t);//转换为本地时间结构体



    std::chrono::milliseconds ms;
    std::chrono::microseconds cs;
    std::chrono::nanoseconds ns;
    int64 miilsecond;

    switch (return_type)
    {
    case 0:
    
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        return ms.count();
        break;
    case 1:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        miilsecond = ms.count() + now_tm->tm_sec * 1000;
        return miilsecond; 
        break;
    case 2:
    {
        // 将时间点转换为UTC时间（从1970年开始计算）
        auto utcTimePoint = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        // 提取UTC时间的ms
        miilsecond = (utcTimePoint.time_since_epoch()).count();
        return miilsecond;
        break;
    }
    default:
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        return ms.count();
        break;
    }
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

void RsuDataProto::SerializeTimeBase(nebulalink::perceptron3::TimeBase *output_time_base,
                                     struct rsu_data_ns::TimeBase input_timebase_struct)
{
    output_time_base->set_year(input_timebase_struct.year);
    output_time_base->set_month(input_timebase_struct.month);
    output_time_base->set_day(input_timebase_struct.day);
    output_time_base->set_hour(input_timebase_struct.hour);
    output_time_base->set_min(input_timebase_struct.min);
    output_time_base->set_second(input_timebase_struct.second);
    output_time_base->set_miilsecond(input_timebase_struct.miilsecond);
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

void RsuDataProto::SerializeSpeed3(nebulalink::perceptron3::Speed3 *output_speed3,
                                   struct rsu_data_ns::Speed3 input_speed3)
{
    
    output_speed3->set_speed_x(input_speed3.speed_x);
    output_speed3->set_speed_y(input_speed3.speed_y);
    output_speed3->set_speed_z(input_speed3.speed_z);
    return;
}
void RsuDataProto::SerializeTargetSize(nebulalink::perceptron3::TargetSize *input_target_size)
{
    
    input_target_size->set_object_width(1.8);
    input_target_size->set_object_length(1.9);
    input_target_size->set_object_height(1.99);
    return;
}

void RsuDataProto::SerializeTargetSize(nebulalink::perceptron3::TargetSize *output_target_size,
                                       struct rsu_data_ns::TargetSize input_target_size)
{
    
    output_target_size->set_object_width(input_target_size.object_width);
    output_target_size->set_object_length(input_target_size.object_length);
    output_target_size->set_object_height(input_target_size.object_height);
    return;
}

void RsuDataProto::SerializePointGPS(nebulalink::perceptron3::PointGPS *input_point_gps)
{
   
    input_point_gps->set_object_longitude(117.1234);// 经度.范围0-180
    input_point_gps->set_object_latitude(23.1234);// 维度.范围0-90
    input_point_gps->set_object_elevation(1.1234);
    return;
}

void RsuDataProto::SerializePointGPS(nebulalink::perceptron3::PointGPS *output_point_gps,
                                     struct rsu_data_ns::PointGPS input_point_gps)
{
   
    output_point_gps->set_object_longitude(input_point_gps.object_longitude);// 经度.范围0-180
    output_point_gps->set_object_latitude(input_point_gps.object_latitude);// 维度.范围0-90
    output_point_gps->set_object_elevation(input_point_gps.object_elevation);
    return;
}



 void RsuDataProto::SerializeTargetSizeCFD(nebulalink::perceptron3::TargetSize_CFD *input_target_size_cfd)
 {
    input_target_size_cfd->set_object_width_cfd(0);
    input_target_size_cfd->set_object_length_cfd(0);
    input_target_size_cfd->set_object_height_cfd(0);
    return;
 }

 void RsuDataProto::SerializeTargetSizeCFD(nebulalink::perceptron3::TargetSize_CFD *input_target_size_cfd,
                                           struct rsu_data_ns::TargetSize_CFD input_target_cfd_struct)
 {
    input_target_size_cfd->set_object_width_cfd(input_target_cfd_struct.object_width_cfd);
    input_target_size_cfd->set_object_length_cfd(input_target_cfd_struct.object_length_cfd);
    input_target_size_cfd->set_object_height_cfd(input_target_cfd_struct.object_height_cfd);
    return;
 }


 /**
  * @brief 序列化Perceptron消息
  *
  * @return nebulalink::perceptron3::Perceptron 返回序列化后的Perceptron消息
  */
 NP3PERCEPTION RsuDataProto::SerializePerceptron()
 {
     NP3PERCEPTION return_obstacles_info;
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
     return_obstacles_info.set_obj_time_stamp(this->getMiilsecondTimeStamp(1));
     return_obstacles_info.set_ptc_sourcetype(0x04);
     this->SerializeTimeBase(*(return_obstacles_info.mutable_ptc_time_stamp()));
     return_obstacles_info.set_ptc_veh_type(10);
     this->SerializeTargetSizeCFD(return_obstacles_info.mutable_ptc_size_cfd());
     return_obstacles_info.set_is_head_tail(1);
     return_obstacles_info.set_lane_type(1);
     this->SerializePoint3(return_obstacles_info.mutable_point3f());
     return return_obstacles_info;
 }

/**
 * @brief 嵌套消息内容赋值测试
 * 
 * @param input_obstacles_info 
 */
void RsuDataProto::SerializePerceptronTest(NP3PERCEPTION &input_perceptron,int serialize_method)
{
    switch (serialize_method)
    {
    case 1:
    {
        //赋值方式1（使用set_allocated_）
        NP3PERCEPTION temp_peceptron;
        NP3TARGETSIZE *temp_target_size = new nebulalink::perceptron3::TargetSize();
        temp_target_size->set_object_width(1.1);
        temp_target_size->set_object_length(1.2);
        temp_target_size->set_object_height(1.3);
        
        temp_peceptron.set_allocated_target_size(temp_target_size);
        input_perceptron = temp_peceptron;
        break;
    }
    case 2:
    {
        //赋值方式2（使用mutable_），结合CopyFrom
        NP3PERCEPTION temp_peceptron;
        NP3TARGETSIZE temp_target_size;
        temp_target_size.set_object_width(1.4);
        temp_target_size.set_object_length(1.5);
        temp_target_size.set_object_height(1.6);
        temp_peceptron.mutable_target_size()->CopyFrom(temp_target_size);
        input_perceptron = temp_peceptron;
        break;
    }
    case 3:
    {
        //赋值方式3（使用mutable_）：
        NP3PERCEPTION temp_peceptron;
        NP3TARGETSIZE *temp_target_size = temp_peceptron.mutable_target_size();
        temp_target_size->set_object_width(1.7);
        temp_target_size->set_object_length(1.8);
        temp_target_size->set_object_height(1.9);
        input_perceptron = temp_peceptron;
        break;
    }
    
    default:
        break;
    }
    

    


    return;
}

void RsuDataProto::SerializePerceptron(NP3PERCEPTION &output_obstacles_info,
                                       struct rsu_data_ns::Perceptron input_percecpron_struct)
{

    output_obstacles_info.set_is_tracker(input_percecpron_struct.is_tracker);
    output_obstacles_info.set_object_confidence(input_percecpron_struct.object_confidence);
    output_obstacles_info.set_lane_id(input_percecpron_struct.lane_id);
    output_obstacles_info.set_object_class_type(input_percecpron_struct.object_class_type);
    output_obstacles_info.set_object_id(input_percecpron_struct.object_id);
    output_obstacles_info.set_object_speed(input_percecpron_struct.object_speed);
    this->SerializeSpeed3(output_obstacles_info.mutable_speed3f(),input_percecpron_struct.speed3f);
    output_obstacles_info.set_object_acceleration(input_percecpron_struct.object_acceleration);
    this->SerializeTargetSize(output_obstacles_info.mutable_target_size());
    this->SerializePointGPS(output_obstacles_info.mutable_point_gps(),input_percecpron_struct.point_gps);
    output_obstacles_info.set_object_ns(input_percecpron_struct.object_NS);
    output_obstacles_info.set_object_we(input_percecpron_struct.object_WE);
    output_obstacles_info.set_object_heading(input_percecpron_struct.object_heading);
    output_obstacles_info.set_obj_time_stamp(input_percecpron_struct.obj_time_stamp);
    output_obstacles_info.set_ptc_sourcetype(input_percecpron_struct.ptc_sourcetype);
    this->SerializeTimeBase(output_obstacles_info.mutable_ptc_time_stamp(),input_percecpron_struct.ptc_time_stamp);
    
    output_obstacles_info.set_ptc_veh_type(input_percecpron_struct.ptc_veh_type);
    this->SerializeTargetSizeCFD(output_obstacles_info.mutable_ptc_size_cfd(),input_percecpron_struct.ptc_size_cfd);
    output_obstacles_info.set_is_head_tail(input_percecpron_struct.is_head_tail);
    output_obstacles_info.set_lane_type(input_percecpron_struct.lane_type);
    return;
}



void RsuDataProto::SerializePerceptron(NP3PERCEPTION *output_msg,
                                       struct rsu_data_ns::Perceptron input_percecpron_struct)
{

    output_msg->set_is_tracker(input_percecpron_struct.is_tracker);
    output_msg->set_object_confidence(input_percecpron_struct.object_confidence);
    output_msg->set_lane_id(input_percecpron_struct.lane_id);
    output_msg->set_object_class_type(input_percecpron_struct.object_class_type);
    output_msg->set_object_id(input_percecpron_struct.object_id);
    output_msg->set_object_speed(input_percecpron_struct.object_speed);
    this->SerializeSpeed3(output_msg->mutable_speed3f(),input_percecpron_struct.speed3f);
    output_msg->set_object_acceleration(input_percecpron_struct.object_acceleration);
    this->SerializeTargetSize(output_msg->mutable_target_size());
    this->SerializePointGPS(output_msg->mutable_point_gps(),input_percecpron_struct.point_gps);
    output_msg->set_object_ns(input_percecpron_struct.object_NS);
    output_msg->set_object_we(input_percecpron_struct.object_WE);
    output_msg->set_object_heading(input_percecpron_struct.object_heading);
    output_msg->set_obj_time_stamp(input_percecpron_struct.obj_time_stamp);
    output_msg->set_ptc_sourcetype(input_percecpron_struct.ptc_sourcetype);
    this->SerializeTimeBase(output_msg->mutable_ptc_time_stamp(),input_percecpron_struct.ptc_time_stamp);
    
    output_msg->set_ptc_veh_type(input_percecpron_struct.ptc_veh_type);
    this->SerializeTargetSizeCFD(output_msg->mutable_ptc_size_cfd(),input_percecpron_struct.ptc_size_cfd);
    output_msg->set_is_head_tail(input_percecpron_struct.is_head_tail);
    output_msg->set_lane_type(input_percecpron_struct.lane_type);
    return;
}

void RsuDataProto::PrintPerceptron(NP3PERCEPTION input_msg)
{
    std::cout << "*****************print perceptron********************" << std::endl;

    std::cout << "is_tracker:" << input_msg.is_tracker() << "\n"
              << "  object_confidence:" << input_msg.object_confidence() << "\n"
              << "  lane_id:" << input_msg.lane_id()<< "\n"
              << "  object_class_type:" << input_msg.object_class_type()<< "\n"
              << "  object_id:" << input_msg.object_id()<< "\n"
              << "  object_speed:" << input_msg.object_speed()<< "\n"
              << "  speed3f_x_y_z:" << input_msg.speed3f().speed_x() << "  " << input_msg.speed3f().speed_y() << "  " << input_msg.speed3f().speed_z()<< "\n"
              << "  object_acceleration:" << input_msg.object_acceleration()<< "\n"
              << "  target_size_l_w_h:" << input_msg.target_size().object_length() << "  " << input_msg.target_size().object_width() << "  " << input_msg.target_size().object_height()<< "\n"
              << "  point_gps_long_lat_ele:" << input_msg.point_gps().object_longitude() << "  " << input_msg.point_gps().object_latitude() << "  " << input_msg.point_gps().object_elevation()<< "\n"
              << "  object_NS:" << input_msg.object_ns()<< "\n"
              << "  object_ WE:" << input_msg.object_we()<< "\n"
              << "  is_head_tail:" << input_msg.is_head_tail()<< "\n"
              << "  object_heading:" << input_msg.object_heading()<< "\n"
              << "  obj_time_stamp:" << input_msg.obj_time_stamp()<< "\n"
              << "  ptc_sourcetype:" << input_msg.ptc_sourcetype()<< "\n"
              << "  ptc_time_stamp_y_m_d_h_m_s_ms:" << input_msg.ptc_time_stamp().year() << "  "
              << input_msg.ptc_time_stamp().month() << "  " << input_msg.ptc_time_stamp().day() << "  "
              << input_msg.ptc_time_stamp().hour() << "  " << input_msg.ptc_time_stamp().min() << "  "
              << input_msg.ptc_time_stamp().second() <<"  "<< input_msg.ptc_time_stamp().miilsecond()<< "\n"
              << "  ptc_veh_type:" << input_msg.ptc_veh_type()<< "\n"
              << "  ptc_size_cfd_l_w_h:" << input_msg.ptc_size_cfd().object_length_cfd() << "  " << input_msg.ptc_size_cfd().object_width_cfd() << "  " << input_msg.ptc_size_cfd().object_height_cfd()
              << std::endl;

    std::cout << "*****************end print perceptron****************" << std::endl;
}

void RsuDataProto::SerializePerceptronSet(NP3PERCEPTIONSET &output_msg,
                                       struct rsu_data_ns::PerceptronSet input_msg)
{
    output_msg.set_devide_id(input_msg.devide_id);
    output_msg.set_devide_is_true(input_msg.devide_is_true);
    output_msg.set_time_stamp(input_msg.time_stamp);
    output_msg.set_number_frame(input_msg.number_frame);
    this->SerializePointGPS(output_msg.mutable_perception_gps(),input_msg.perception_gps);

    NP3PERCEPTION * temp_perceptron = output_msg.add_perceptron();
    this->SerializePerceptron(temp_perceptron,input_msg.perceptron);    
    return;
}



void RsuDataProto::DeserializePerceptron(char input_serialized_buf,int buf_length,
                                       struct rsu_data_ns::Perceptron &output_percecpron_struct)
{
    NP3PERCEPTION temp_perceptron;
    output_percecpron_struct.ptc_time_stamp.miilsecond = temp_perceptron.ParseFromArray(&input_serialized_buf,buf_length);

    return;
}

void RsuDataProto::PrintPerceptronSet(NP3PERCEPTIONSET input_msg)
{

        std::cout << "*****************************print PerceptronSet *****************************" << std::endl;

        std::cout << "device_id:" << input_msg.devide_id()
                  << "  devide_is_true:" << input_msg.devide_is_true()
                  << "  time_stamp:" << input_msg.time_stamp()
                  << "  number_frame:" << input_msg.number_frame()
                  << "  PointGps_long_lat_ele" << input_msg.perception_gps().object_longitude() << "  "
                  << input_msg.perception_gps().object_latitude() << "  " << input_msg.perception_gps().object_elevation()
                  << std::endl;
                  for(int i = 0; i<input_msg.perceptron_size();i++)
                  {
                    PrintPerceptron(input_msg.perceptron(i));
                  }

        std::cout << "*************************end print PerceptronSet *****************************" << std::endl;



}

/**
 * @brief 测试FrameArray赋值功能,重复消息内容赋值（带有repeated字段的消息）
 * 
 * @return nebulalink::perceptron3::FrameArray 
 */
std::string RsuDataProto::SerializeFrameArrayTest(NP3FRAMEARRAY &input_frame_array,int serialize_method)
{

    switch (serialize_method)
    {
    case 1: //***********方法1：通过add_依次赋值，逐个添加元素，类似于Vector中的push_back()
    {
        //第一个
        NP3PERCEPTION *temp_obstacles_info1 = input_frame_array.add_perceptron();
        temp_obstacles_info1->mutable_point3f()->set_x(1.1);
        temp_obstacles_info1->mutable_point3f()->set_y(1.2);
        temp_obstacles_info1->mutable_point3f()->set_z(1.3);
        temp_obstacles_info1->mutable_target_size()->set_object_length(1.1);
        temp_obstacles_info1->mutable_target_size()->set_object_width(1.2);
        temp_obstacles_info1->mutable_target_size()->set_object_height(1.3);

      
        //第二个
        NP3PERCEPTION *temp_obstacles_info2 = input_frame_array.add_perceptron();
        temp_obstacles_info2->mutable_point3f()->set_x(1.4);
        temp_obstacles_info2->mutable_point3f()->set_y(1.5);
        temp_obstacles_info2->mutable_point3f()->set_z(1.6);
        temp_obstacles_info2->mutable_target_size()->set_object_length(1.4);
        temp_obstacles_info2->mutable_target_size()->set_object_width(1.5);
        temp_obstacles_info2->mutable_target_size()->set_object_height(1.6);
  
        //第三个
        NP3PERCEPTION *temp_obstacles_info3 = input_frame_array.add_perceptron();
        temp_obstacles_info3->mutable_point3f()->set_x(1.7);
        temp_obstacles_info3->mutable_point3f()->set_y(1.8);
        temp_obstacles_info3->mutable_point3f()->set_z(1.9);
        temp_obstacles_info3->mutable_target_size()->set_object_length(1.7);
        temp_obstacles_info3->mutable_target_size()->set_object_width(1.8);
        temp_obstacles_info3->mutable_target_size()->set_object_height(1.9);
        
        int size_of_frame_array = input_frame_array.perceptron_size();

        std::string serialized_string;

        if (!(input_frame_array.SerializeToString(&serialized_string)))
        {
            // 序列化失败返回空字符串
            std::cerr << "Frame_Array序列化失败!" << std::endl;
            return std::string();
            break;
        }
        else
        {
            std::cout << "Frame_Array序列化成功,size of str is:" <<serialized_string.size() << std::endl;
            std::cout << "Frame_Array序列化成功,size of array is:" <<size_of_frame_array << std::endl;

            return serialized_string;
            break;
        }
        
    }
    case 2:////***********方法1：整体赋值：将整个vector类型的变量整体赋值给repeated类型的变量
    {
        std::vector<NP3PERCEPTION> temp_perceptron_vector;
        NP3PERCEPTION temp_perceptron;
        //1.先对vector进行push_back
        this->SerializePerceptronTest(temp_perceptron,1);
        temp_perceptron_vector.push_back(temp_perceptron);
        this->SerializePerceptronTest(temp_perceptron, 2);
        temp_perceptron_vector.push_back(temp_perceptron);
        this->SerializePerceptronTest(temp_perceptron, 3);
        temp_perceptron_vector.push_back(temp_perceptron);

   
        input_frame_array.mutable_perceptron()->CopyFrom({temp_perceptron_vector.begin(),temp_perceptron_vector.end()});
        std::string serialized_string;
        int size_of_frame_array = input_frame_array.perceptron_size();

        if (!(input_frame_array.SerializeToString(&serialized_string)))
        {
            // 序列化失败返回空字符串
            std::cerr << "Frame_Array序列化失败!" << std::endl;
            return std::string();
            break;
        }
        else
        {
            std::cout << "Frame_Array序列化成功" << std::endl;
            std::cout << "Frame_Array序列化成功,size of str is:" <<serialized_string.size() << std::endl;
            std::cout << "Frame_Array序列化成功,size of array is:" <<size_of_frame_array << std::endl;
            return serialized_string;
            break;
        }

    }
    case 3:
    {
    }
    default:
        break;
        return std::string();
    }
    return std::string();
    
}

/**
 * @brief 测试FrameArray反序列化功能,将repeated类型的变量赋值给vector类型
 * 
 * @return std::string 返回反序列化后的string
 */
std::vector<rsu_data_ns::FrameArray> RsuDataProto::DeserializeFrameArrayTest(const std::string input_serialized_str)
{

    std::vector<rsu_data_ns::FrameArray> return_frame_array_vector;
    if(input_serialized_str.empty())
    {
        std::cerr << "input string is empty!!" << std::endl;
    }
    else
    {
        nebulalink::perceptron3::FrameArray deserialized_frame_array;
        if (!deserialized_frame_array.ParseFromString(input_serialized_str))
        {
            std::cerr << "FrameArray反序列化失败" << std::endl;
        }
        else
        {
            std::cout << "FrameArray反序列化成功" << std::endl;
            rsu_data_ns::FrameArray temp_frame_array_struct;
            for(int i = 0;i < deserialized_frame_array.perceptron_size();i++)
            {
                temp_frame_array_struct.perceptron.target_size.object_height = deserialized_frame_array.perceptron(i).target_size().object_height();
                return_frame_array_vector.push_back(temp_frame_array_struct);
                
            }
            return return_frame_array_vector;
        } 
    }   
    return return_frame_array_vector;
}

/**
 * @brief 通过返回值赋值
 * 
 * @return nebulalink::perceptron3::FrameArray 
 */
NP3FRAMEARRAY RsuDataProto::SerializeFrameArray()
{
    //逐个添加元素
    NP3PERCEPTION temp_obstacles_info;//后续需要设置其属性
    NP3FRAMEARRAY return_frame_array;
    return_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    //整个repeated赋值
    NP3FRAMEARRAY return_frame_array1;
    //添加一些元素到return_frame_array1的Perceptron中
    NP3PERCEPTION temp_obstacles_info1;
    NP3PERCEPTION temp_obstacles_info2;
    NP3PERCEPTION temp_obstacles_info3;
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
/**
 * @brief 通过引用进行赋值
 * 
 * @param input_frame_array 
 */
void RsuDataProto::SerializeFrameArray(NP3FRAMEARRAY &input_frame_array)
{
    NP3PERCEPTION temp_perceptron = this->SerializePerceptron();

    // 逐个添加元素
    NP3PERCEPTION temp_obstacles_info; // 后续需要设置其属性
    NP3FRAMEARRAY return_frame_array;
    input_frame_array.add_perceptron()->CopyFrom(temp_perceptron);

    // //整个repeated赋值
    // NP3FRAMEARRAY return_frame_array1;
    // //添加一些元素到return_frame_array1的Perceptron中
    // NP3PERCEPTION temp_obstacles_info1;
    // NP3PERCEPTION temp_obstacles_info2;
    // NP3PERCEPTION temp_obstacles_info3;
    // return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info1);
    // return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info2);
    // return_frame_array1.add_perceptron()->CopyFrom(temp_obstacles_info3);
    // //直接将整个return_frame_array1的perceptron()赋值到return_frame_array
    // input_frame_array.mutable_perceptron()->CopyFrom(return_frame_array1.perceptron());
    // //先copy再add才可以
    // input_frame_array.add_perceptron()->CopyFrom(temp_obstacles_info);

    // 获得大小
    //  std::cout << "size of input_frame_array:" << input_frame_array.perceptron_size() <<std::endl;

    return;
}