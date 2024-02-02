#ifndef RSU_DATA_SERIALIZE_H
#define RSU_DATA_SERIALIZE_H
#include "nebulalink_perceptron.pb.h"


class RsuDataProto
{
public:
    RsuDataProto();
    void DataSerialize();//数据序列化
    void DataDeserialize();//数据反序列化

    void GetUTC();
    int GetTimeStamp(int return_time_stamp_type);
    std::string GetCurrentTimeStamp(nebulalink::perceptron3::TimeBase &input_time_base);
    void SerializeTimeBase(nebulalink::perceptron3::TimeBase &input_time_base);
    nebulalink::perceptron3::Point3 SerializePoint3();
    void SerializePoint3(nebulalink::perceptron3::Point3 *input_point3);
    void SerializeSpeed3(nebulalink::perceptron3::Speed3 *input_speed3);
    void SerializeTargetSize(nebulalink::perceptron3::TargetSize *input_target_size);
    void SerializePointGPS(nebulalink::perceptron3::PointGPS *input_point_gps);
    void SerializeTargetSizeCFD(nebulalink::perceptron3::TargetSize_CFD *input_target_size_cfd);

    void SerializePerceptron(nebulalink::perceptron3::Perceptron &input_obstacles_info);
    nebulalink::perceptron3::Perceptron SerializePerceptron();

    nebulalink::perceptron3::FrameArray SerializeFrameArrayTest();
    nebulalink::perceptron3::FrameArray SerializeFrameArray();
    void SerializeFrameArray(nebulalink::perceptron3::FrameArray &input_frame_array);
};

#endif