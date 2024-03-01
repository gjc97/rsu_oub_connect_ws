#ifndef RSU_MESSAGE_H
#define RSU_MESSAGE_H

#include <float.h>
typedef int32_t int32;
typedef int64_t int64;
//---------------------------------------------置信度----------------------------------------------------------
// 经纬度坐标置信度

namespace rsu_data_ns
{
	struct PointGPS_CFD
	{
		double position_confidence;	 // 经纬度置信度
		double plevation_confidence; // 高程置信度
	};

	// 尺寸置信度
	struct TargetSize_CFD
	{
		float object_width_cfd;	 // 目标宽度置信度
		float object_length_cfd; // 目标长度置信度
		float object_height_cfd; // 目标高度置信度
	};

	// 四轴加速度置信度
	struct Acc4Way_CFD
	{
		float lonAccConfidence;	 // Long：纵向加速度置信度
		float latAccConfidence;	 // Lat： 横向加速度置信度
		float vertAccConfidence; // Vert：垂直加速度置信度
		float yawRateCon;		 // Yaw： 角加速度置信度
	};
	// 四轴加速度置信度
	struct Motion_CFD
	{
		float speedCfd;	  // 速度置信度
		float headingCfd; // 航向角精度置信度
		float steerCfd;	  // 方向盘转向角置信度
	};

	//--------------------------------------------基础数据-----------------------------------------------------------
	// 目标四轴加速度
	struct Acc4Way
	{
		float acc4WayLon;  // Long：纵向加速度
		float acc4WayLat;  // Lat： 横向加速度
		float acc4WayVert; // Vert：垂直加速度
		float acc4WayYaw;  // Yaw： 角加速度
	};

	// 感知目标列表 成员-目标尺寸
	struct TargetSize
	{
		float object_width;	 // 目标宽度.范围0-10m
		float object_length; // 目标长度.范围0-40m
		float object_height; // 目标高度.范围0-6.35m
	};

	// 时间戳
	struct TimeBase
	{
		int32 year;
		int32 month;
		int32 day;
		int32 hour;
		int32 min;
		int32 second;
		int32 miilsecond;
	};

	// 目标经纬度坐标
	struct PointGPS
	{
		double object_longitude; // 经度.范围0-180
		double object_latitude;	 // 维度.范围0-90
		double object_elevation; // 海拔.范围0-4096；
	};

	// 路径点属性
	struct PointDesc
	{
		double p_longitude;			 // 经度
		double p_latitude;			 // 纬度
		double p_altitude;			 // 海拔高度
		int32 p_NS;					 // 南北纬
		int32 p_EW;					 // 东西经
		float p_speed;				 // 速度
		float p_heading;			 // 航向角
		Acc4Way p_accel_4way;		 // 目标四轴加速度
		double p_dis2end;			 // 距结束点距离
		int64 p_up_region_node_id;	 // 上游节点ID
		int64 p_down_region_node_id; // 下游节点ID
		int32 p_relate_lane_id;		 // 点所属车道
		int32 satellite_num;		 // 定位点卫星数
		PointGPS_CFD p_pos_cfd;		 // 位置置信度
		int32 time_offset;			 // 当前描述时刻相对于参考时间点的偏差
	};

	// 轨迹点集合
	struct PathPoint
	{
		TimeBase pp_time;
		PointDesc pp_point; // gps，speed,heading,
	};

	// 关联路段属性
	struct RelateLinkDesc
	{
		int32 up_region_node_id;   // 上游节点ID
		int32 down_region_node_id; // 下游节点ID
		struct RelateLane
		{
			int32 rtl_lane_id; // 车道ID
		};
		RelateLane rtl_lanes; // 关联车道
	};

	// 关联路径属性
	struct RelatePathDesc
	{
		PointDesc rpd_pathPoint1; // 点经纬度
		float rpd_radius;		  // 关联半径
	};

	// 感知目标列表 成员-目标的几何中心相对传感器坐标位置
	struct Point3
	{
		float x;
		float y;
		float z;
	};
	// 感知目标列表 成员-图像坐标位置
	struct Point4
	{
		int32 camera_x;
		int32 camera_y;
		int32 camera_w;
		int32 camera_h;
	};

	// 感知目标列表 成员-目标分轴速度
	struct Speed3
	{
		float speed_x;
		float speed_y;
		float speed_z;
	};

	struct PathPlanning_PB
	{
		PointDesc pplan_pos;
		float pplan_speed_cfd;
		float pplan_heading_cfd;
		Acc4Way pplan_acce;
		Acc4Way_CFD pplan_acce_cfd;
		int32 estimated_time;
		int32 time_confidence;
	};
	struct Planning_PB
	{
		int32 duration;
		int32 confid;
		std::string driving_behavior;
		PathPlanning_PB path_planning;
	};

	// 在离开线上的交通信息；
	struct InfoEndLineValues
	{
	};

	// 线上的交通信息
	struct InfoEntreLineValues
	{
	};

	// 车道级lane 信息列表
	struct LaneJamSenseParams
	{
		std::string lane_id;				 // 车道id
		int32 lane_types;					 // 车道类型
		float lane_sense_len;				 // 车道长度
		int32 lane_direction;				 // 车道方向
		float lane_avg_speed;				 // 车道内车辆的平均速度
		int32 lane_veh_num;					 // 该车道不区分车型机动车总流量
		float lane_space_occupancy;			 // 车道空间占有率
		int32 lane_queue_len;				 // 车道排队长度
		int32 lane_count_time;				 // 车道统计时间
		int32 lane_count_flow;				 // 车道统计车数量
		bool lane_is_count;					 // 是否统计成功
		int32 lane_ave_distance;			 // 车道平均车间距
		int32 lane_cur_distance;			 // 车道实时车间距
		float lane_time_occupancy;			 // 车道时间占有率
		InfoEntreLineValues lane_entre_info; // 进入车道的交通信息
		InfoEndLineValues lane_end_info;	 // 离开车道的交通信息
		int32 lane_num;						 // 车道数量----17以下襄阳项目定义
		int32 lane_no;						 // 车道编号
		int32 lane_peron_volume;			 // 该车道行人流量
		int32 lane_no_motor_volume;			 // 该车道非机动车流量
		int32 lane_minmotor_volume;			 // 该车道小车总流量
		int32 lane_medmotor_volume;			 // 该车道中车流量
		int32 lane_maxmotor_volume;			 // 该车道大车流量
		int32 lane_pcu;						 // 该车道交通当量
		float lane_avspeed;					 // 该车道平均速度
		float lane_headway;					 // 该车道平均车头时距
		float lane_gap;						 // 该车道平均车身间距
		float lane_avdistance;				 // 该车道平均车距
		float lane_avstop;					 // 该车道平均停车次数
		float lane_speed85;					 // 该车道85%以上的车辆的最高速度
		float lane_queueLength;				 // 该车道来向车道排队长度
		float lane_stopline;				 // 该车道停止线
	};

	// 道路级link 信息列表
	struct LinkJamSenseParams
	{
		std::string link_id;				 // 道路id
		float link_len;						 // 道路长度
		float link_avgspeed;				 // 道路平均速度
		int32 link_veh_num;					 // 道路车辆数
		int32 link_type;					 // 道路类型
		int32 link_direction;				 // 道路方向
		float link_space_occupancy;			 // 道路空间占有率
		float link_time_occupancy;			 // 道路时间占有率
		int32 link_count_time;				 // 道路统计时间
		int32 link_count_flow;				 // 道路统计车数量
		bool link_is_count;					 // 是否统计成功
		InfoEntreLineValues link_entre_info; // 进入道路的交通信息
		InfoEndLineValues link_end_info;	 // 离开道路的交通信息
		std::string link_deviceid;			 // 道路编号--17以下襄阳项目定义
		int32 link_heading;					 // 道路与道路入口中心点与正北方向的顺时针夹角
		int32 link_phaseid;					 // 相位ID
		std::string link_name;				 // 道路名称，
		int32 link_no;						 // 道路编码
		PointGPS link_gps;					 // 道路入口坐标
		int32 link_measnum;					 // 检测线编号
		int32 link_num;						 // 线圈数量
		int32 link_motor_volume;			 // 该道路不区分车型机动车总流量
		int32 link_peron_volume;			 // 该道路行人流量
		int32 link_no_motor_volume;			 // 该道路非机动车流量
		int32 link_minmotor_volume;			 // 该道路小车总流量
		int32 link_medmotor_volume;			 // 该道路中车流量
		int32 link_maxmotor_volume;			 // 该道路大车流量
		int32 link_pcu;						 // 该道路交通当量
		float link_avspeed;					 // 该道路平均速度
		float link_time_occupany;			 // 该道路时间占有率
		float link_headway;					 // 该道路平均车头时距
		float link_gap;						 // 该道路平均车身间距
		float link_avdistance;				 // 该道路平均车距
		float link_avstop;					 // 该道路平均停车次数
		float link_speed85;					 // 该道路85%以上的车辆的最高速度
		float link_queueLength;				 // 该道路来向车道排队长度
		float link_stopline;				 // 该道路平均速度
		float link_space_occupany;			 // 该道路空间占有率
		LaneJamSenseParams road_lanelist;	 // 感知车道级lane 统计信息
	};

	// 感知路口级 统计信息列表
	struct OnLineValues
	{
		int32 cycleid;					  // 线圈id
		PointGPS pos;					  // 虚拟线的位置
		int32 vehnum;					  // 线上车辆数
		float avgSpeed;					  // 线上车的平均速度
		std::string cross_name;			  // 路口名称----5以下为襄阳定义
		int32 cross_laneno;				  // 路口编号
		int32 cross_volume;				  // 该路口不区分车型机动车总流量
		int32 cross_peron_volume;		  // 该路口非机动车流量
		int32 cross_no_motor_volume;	  // 该路口大车流量
		int32 cross_minmotor_volume;	  // 该路口小车总流量
		int32 cross_medmotor_volume;	  // 该路口中车流量
		int32 cross_maxmotor_volume;	  // 该路口大车流量
		int32 cross_pcu;				  // 该路口交通当量
		float cross_avspeed;			  // 该路口平均速度
		float cross_time_occupany;		  // 该路口时间占有率
		float cross_headway;			  // 该路口平均车头时距
		float cross_gap;				  // 该路口平均车身间距
		float cross_avdistance;			  // 该路口平均车距
		float cross_avstop;				  // 该路口平均停车次数
		float cross_speed85;			  // 该路口目标超过85%的目标速度
		float cross_queueLength;		  // 该路口来向车道排队长度
		float cross_stopline;			  // 该路口车道停止线
		float cross_space_occupany;		  // 该路口空间占有率
		LinkJamSenseParams road_linklist; // 感知道路级link 信息统计
	};
	// 内部扩展数据段
	struct Nebulalink_Extend
	{
		std::string version; // 版本号
		float feature = 2;	 // 目标特征信息向量
	};

	// 感知目标属性列表.
	struct Perceptron
	{
		bool is_tracker;				 // 物体是否被跟踪
		float object_confidence;		 // 目标置信度，
		std::string lane_id;			 // 目标所在车道id
		int32 object_class_type;		 // 目标大类类型，路侧单元检测到的交通参与者类型。0 是未知，1 是机动车，2 是非机动车，3 是行人，4是 rsu 自身
		int32 object_id;				 // 单杆目标Id
		Point3 point3f;					 // 目标的几何中心相对传感器坐标位置
		Point4 point4f;					 // 图像坐标位置
		float object_speed;				 // 目标速度,km/h
		Speed3 speed3f;					 // 目标分轴速度
		float object_acceleration;		 // 目标加速度
		TargetSize target_size;			 // 目标尺寸.宽度.范围 0-10m；长度.范围 0-40m；高度.范围 0-6.35m
		PointGPS point_gps;				 // 目标经纬度坐标.
		int32 object_NS;				 // 南北纬.0:北纬，1，南纬
		int32 object_WE;				 // 东西经.0:东，1，西
		float object_direction;			 // 车辆车头朝向角度，与正北偏角
		float object_heading;			 // 行驶方向朝向航向角（
		int32 is_head_tail;				 // 车头还是车尾 1:头，0 尾
		int32 lane_type;				 // 目标所在车道类型,
		std::string plate_num;			 // 目标为车辆时，为车牌号
		std::string objects_identity;	 // 全域目标id
		int32 fuel_type;				 // 目标燃油类型
		Acc4Way accel_4way;				 // 目标四轴加速度
		int64 obj_time_stamp;			 // 目标时间戳-毫秒数
		int32 ptc_sourcetype;			 // 目标数据来源 ,0x00: 未知 0x01：RSU 自身 0x02：v2x 广播 0x03：摄像头 0x04：微波雷达 0x05：地磁线圈 0x06：激光雷达 0x07：融合/MEC
		TimeBase ptc_time_stamp;		 // 目标时间戳-带格式
		PointGPS_CFD ptc_gps_cfd;		 // 目标经纬度坐标置信度
		int32 ptc_tran_state;			 // 目标车辆档位状态
		float ptc_angle;				 // 目标转向角
		Motion_CFD ptc_motino_cfd;		 // 目标运行状态置信度
		int32 ptc_veh_type;				 // 目标车辆类型
		TargetSize_CFD ptc_size_cfd;	 // 目标尺寸置信度
		int32 ptc_Exttype;				 // 目标类型扩展
		float ptc_Exttype_cfd;			 // 目标类型置信度
		Acc4Way_CFD ptc_accel_4way_cfd;	 // 目标四轴加速度置信度
		int32 ptc_status_duration;		 // 目标状态点偏差
		PathPoint ptc_pathpoint_history; // 目标历史轨迹
		Planning_PB ptc_planning_list;	 // 目标行驶规划集合
		PointDesc ptc_polygonPoint;		 // 目标影响感知区域
		int32 ptc_satellite;			 // 目标所在点卫星数量
		int32 ptc_regionid;				 // 目标所在点区域id
		int32 ptc_nodeid;				 // 目标所在点区域内节点id
		int32 ptc_laneid;				 // 目标所在点车道id
		std::string ptc_link_name;		 // 目标所在点道路名称
		int32 ptc_link_width;			 // 目标所在点道路宽度
		int32 ptc_veh_plate_type;		 // 车牌类型
		int32 ptc_veh_plate_color;		 // 车牌颜色
		int32 ptc_veh_color;			 // 目标车辆颜色
		Nebulalink_Extend nebula_extend; // 扩展数据段
	};

	// 多帧目标历史数据.
	struct FrameArray
	{
		struct Perceptron perceptron;
	};

	// v2x网联车数据.
	struct V2XOBUs
	{
		PointDesc obu_point;	  // 车辆基础数据
		float obu_wheel_angle;	  // 转向轮角度
		int64 obu_time_stamp;	  // 时间戳
		int32 obu_veh_type;		  // 车辆类型
		int32 obu_fuel_type;	  // 车辆燃油类型
		int32 obu_light;		  // 状态灯
		int32 obu_brake_state;	  // 刹车状态
		int32 obu_veh_state;	  // 车辆状态标志
		float obu_cfd;			  // 目标置信度
		TargetSize obu_size;	  // 车辆长宽高
		std::string obu_platenum; // 车牌号
		std::string obu_deviceid; // OBU SN 设备号
	};

	//----------------------------------------------ssminfo基元数据---------------------------------------------------------
	// polygon属性列表.
	struct DetectorRegion
	{
		PointDesc polygonPoint; // 经纬度
	};

	// obs属性列表.
	struct Obstacles
	{
		int32 obstype;				 // 障碍物 类型
		int32 obstype_cfd;			 // 障碍物 类型置信度
		int32 obsId;				 // 障碍物 id
		int32 obs_source;			 // 障碍物 数据来源
		TimeBase obs_time_stamp;	 // 障碍物 时间戳
		PointDesc obs_gps;			 // 障碍物 相对位置精度
		PointGPS_CFD obs_gps_cfd;	 // 障碍物 相对位置精度置信度
		float obs_speed;			 // 障碍物 速度
		float obs_speed_cfd;		 // 障碍物 速度置信度
		float obs_heading;			 // 障碍物 航向角
		float obs_heading_cfd;		 // 障碍物 航向角置信度
		float obs_verSpeed;			 // 障碍物 角加速度
		float obs_verSpeed_cfd;		 // 障碍物 角加速度置信度
		Acc4Way obs_accel_4way;		 // 障碍物 四轴加速度
		TargetSize obs_size;		 // 障碍物 尺寸大小
		TargetSize_CFD obs_size_cfd; // 障碍物 尺寸大小置信度
		int32 obs_tracking;			 // 障碍物 追踪时间
		PointDesc obs_polygonPoint;	 // 障碍物 影响区域点集合
	};
	// 感知事件属性列表.
	struct Eventlist
	{
		int32 event_id;					// 事件ID
		int32 event_status;				// 事件状态
		int32 event_type;				// 交通事件类型.
		int32 rte_source;				// 事件源
		PointGPS event_gps;				// 事件发生的经纬度-海拔
		float event_radius;				// 事件影响半径.
		std::string event_desc;			// 事件描述
		int32 event_priority;			// 事件优先级
		RelateLinkDesc linklist;		// 事件关联的link路径集合
		RelatePathDesc pathlist;		// 事件关联的lan级路径集合
		int32 event_confid;				// 事件置信度.
		TimeBase event_timestamp_start; // 事件开始时间戳
		TimeBase event_timestamp_end;	// 事件结束时间戳
	};

	// 南向设备状态数据列表.
	struct Heartlist
	{
		int32 device_status;			// rsu外接设备状态
		std::string err_device_id;		// rsu外接设备id
		int32 err_code;					// rsu外接设备故障码
		int64 heart_time;				// rsu外接设备时间戳
		std::string err_desc;			// rsu外接设备故障描述
		int32 err_level;				// rsu外接设备故障等级.
		int32 err_device_type;			// rsu外接设备类型
		std::string err_device_version; // rsu外接设备版本号
		float device_temp;				// rsu外接设备温度
	};

	// 感知共享消息-结构化数据
	struct PerceptronSet
	{
		std::string devide_id;					  // 设备号.
		bool devide_is_true;					  // 判断设备数据的有效性.
		int64 time_stamp;						  // 数据帧时间戳.utc时间单位ms
		int32 number_frame;						  // 帧的序列号
		PointGPS perception_gps;				  // 感知设备安装位置的GPS信息.
		Perceptron perceptron;					  // 感知参与者列表
		LaneJamSenseParams lane_jam_sense_params; // lane级道路信息
		LinkJamSenseParams link_jam_sense_params; // link级道路信息
		OnLineValues on_line_values;			  // 交通路口统计类列表
		FrameArray frame_array;					  // 多帧历史数据
		Eventlist event_list;					  // 感知事件列表
		Heartlist heart_list;					  // RSU南向感知设备状态
		DetectorRegion polygon;					  // 感知区域
		Obstacles obstacle;						  // 感知障碍物列表
		V2XOBUs v2x_obus;						  // rsu探测的v2x网联车数据
	};

}

#endif