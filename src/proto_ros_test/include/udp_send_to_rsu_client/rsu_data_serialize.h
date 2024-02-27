#ifndef RSU_DATA_SERIALIZE_H
#define RSU_DATA_SERIALIZE_H
#include "nebulalink_perceptron.pb.h"
#include "udp_send_to_rsu_client/rsu_message.h"

typedef nebulalink::perceptron3::TargetSize NP3TARGETSIZE;
typedef nebulalink::perceptron3::Perceptron NP3PERCEPTION;
typedef nebulalink::perceptron3::PerceptronSet NP3PERCEPTIONSET;
typedef nebulalink::perceptron3::FrameArray NP3FRAMEARRAY;

class RsuDataProto
{
public:
    RsuDataProto();
    void DataSerialize();//数据序列化
    void DataDeserialize();//数据反序列化

    void GeneratePerceptronStruct(struct rsu_data_ns::Perceptron &input_perceptron_struct);
    void GeneratePerceptronSetStruct(struct rsu_data_ns::PerceptronSet &input_perceptronset_struct);

    void GetUTC();
    int GetTimeStamp(int return_time_stamp_type);
    std::string GetCurrentTimeStamp(nebulalink::perceptron3::TimeBase &input_time_base);
    void getTimeBase(rsu_data_ns::TimeBase &input_time_base);
    int64 getMiilsecondTimeStamp(int return_type);

    void SerializeTimeBase(nebulalink::perceptron3::TimeBase &input_time_base);
    void SerializeTimeBase(nebulalink::perceptron3::TimeBase *output_time_base,
                                     struct rsu_data_ns::TimeBase input_timebase_struct);
    
    nebulalink::perceptron3::Point3 SerializePoint3();
    void SerializePoint3(nebulalink::perceptron3::Point3 *input_point3);
    void SerializeSpeed3(nebulalink::perceptron3::Speed3 *input_speed3);
    void SerializeSpeed3(nebulalink::perceptron3::Speed3 *output_speed3,
                                   struct rsu_data_ns::Speed3 input_speed3);

    void SerializeTargetSize(nebulalink::perceptron3::TargetSize *input_target_size);
    void SerializeTargetSize(nebulalink::perceptron3::TargetSize *output_target_size,
                                       struct rsu_data_ns::TargetSize input_target_size);

    void SerializePointGPS(nebulalink::perceptron3::PointGPS *input_point_gps);
    void SerializePointGPS(nebulalink::perceptron3::PointGPS *output_point_gps,
                                         struct rsu_data_ns::PointGPS input_point_gps);

    void SerializeTargetSizeCFD(nebulalink::perceptron3::TargetSize_CFD *input_target_size_cfd);
    void SerializeTargetSizeCFD(nebulalink::perceptron3::TargetSize_CFD *output_target_size_cfd,
                                struct rsu_data_ns::TargetSize_CFD input_target_size_cfd);

    void SerializePerceptron(NP3PERCEPTION &output_obstacles_info,
                             struct rsu_data_ns::Perceptron input_percecpron_struct);

    void SerializePerceptron(NP3PERCEPTION *output_obstacles_info,
                                           struct rsu_data_ns::Perceptron input_percecpron_struct);

    void SerializePerceptronTest(NP3PERCEPTION &input_perceptron, int serialize_method);
    NP3PERCEPTION SerializePerceptron();



    void SerializePerceptronSet(NP3PERCEPTIONSET &output_msg,
                                       struct rsu_data_ns::PerceptronSet input_msg);

    void DeserializePerceptron(char input_serialized_buf,int buf_length,
                                       struct rsu_data_ns::Perceptron &output_percecpron_struct);

    std::string SerializeFrameArrayTest(NP3FRAMEARRAY &input_frame_array,int serialize_method);
    std::vector<rsu_data_ns::FrameArray> DeserializeFrameArrayTest(const std::string input_serialized_str);


    NP3FRAMEARRAY SerializeFrameArray();
    void SerializeFrameArray(NP3FRAMEARRAY &input_frame_array);





    
};

#endif