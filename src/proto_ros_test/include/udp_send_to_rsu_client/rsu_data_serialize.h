#ifndef RSU_DATA_SERIALIZE_H
#define RSU_DATA_SERIALIZE_H
#include "nebulalink_perceptron.pb.h"


typedef nebulalink::perceptron3::TargetSize NP3TARGETSIZE;
typedef nebulalink::perceptron3::Perceptron NP3PERCEPTION;
typedef nebulalink::perceptron3::FrameArray NP3FRAMEARRAY;

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

    void SerializePerceptron(NP3PERCEPTION &input_obstacles_info);
    void SerializePerceptronTest(int serialize_method);
    NP3PERCEPTION SerializePerceptron();

    std::string SerializeFrameArrayTest(NP3FRAMEARRAY &input_frame_array,int serialize_method);
    std::string DeserializeFrameArrayTest(const NP3FRAMEARRAY input_frame_array);


    NP3FRAMEARRAY SerializeFrameArray();
    void SerializeFrameArray(NP3FRAMEARRAY &input_frame_array);





    
};

#endif