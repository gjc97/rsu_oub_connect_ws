#include "obu_connect/obu_client.h"
#include <ros/ros.h>
#include <ros/package.h>
#include <stdlib.h>
#include <string>
#include <iostream>







OBU_CLIENT::OBU_CLIENT()
{
	socket_fd_ = 0;
    
    tcp_ip = "192.168.10.224";
    
    tcp_port = 5050;
	print_fun = new PRINT_FUN();

}


std::string OBU_CLIENT::getValueFromIni(const std::string& filePath, const std::string& section, const std::string& key) 
{
    std::ifstream file(filePath);
    std::string line;
    std::string value;
    bool sectionFound = false;

    while (std::getline(file, line))
    {
        if (!sectionFound && line == "[" + section + "]")
        {
            sectionFound = true;
        }
        else if (sectionFound)
        {
            size_t found = line.find(key);
            if (found != std::string::npos)
            {
                size_t equalsPos = line.find("=");
                value = line.substr(equalsPos + 1);
                break;
            }
        }
    }

    return value;
}

void OBU_CLIENT::SetConfig(std::string input_str)
{
	tcp_ip = getValueFromIni(input_str, "ClientSet", "ClientIP");
	tcp_port = std::atoi(getValueFromIni(input_str, "ClientSet", "ClientPort").c_str());
	std::cout << "****************************************************" << std::endl;
	std::cout << "ip地址:" << tcp_ip << "本地端口号:" << tcp_port << std::endl;
	std::cout << "****************************************************" << std::endl;
}

int OBU_CLIENT::ClientInit(void)
{
	CloseClient();

    if( (socket_fd_ = socket(AF_INET,SOCK_STREAM,0)) < 0 )
    {
        printf("create socket error...\n)");
        return -1;
    }

    memset(&server_addr_,0,sizeof(server_addr_));

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(tcp_port);

    if(inet_pton(AF_INET,tcp_ip.c_str(),&server_addr_.sin_addr) <=0 ) {
        printf("IP address error ...\n");
        return -2;
    }

	server_addr_.sin_family = AF_INET;
	server_addr_.sin_port = htons(tcp_port);
	server_addr_.sin_addr.s_addr = inet_addr(tcp_ip.c_str());
	memset(server_addr_.sin_zero, '\0', sizeof server_addr_.sin_zero);  
	
    if(connect(socket_fd_,(struct sockaddr*)&server_addr_,sizeof(server_addr_)/sizeof(char))<0) {
        printf("TCP connect error: %s(errno: %d)\n",strerror(errno),errno);
        return -3;
    }

    printf("init socket successful...sockfd = %d\n",socket_fd_);
    return 1;
}

int OBU_CLIENT::CloseClient()
{
    if(socket_fd_ > 0)
    {
        close(socket_fd_);
        socket_fd_ = -1;
    }

    return 1;
}
int OBU_CLIENT::NewThread(void* (*func)(void*), void* args)
{
    pthread_t tid = 0;

    int ret = pthread_create(&tid, NULL, func, args);//参数：创建的线程id，线程参数，线程运行函数的起始地址，运行函数的参数
    if( ret == 0 ) //创建线程成功返回0
    {
        printf("pthread_create successfully...\n");

        usleep(50000);
    }
    else
    {
        printf("pthread_create failed...\n");
        return -1;
    }

    pthread_detach(tid);

    return 0;
}
void* OBU_CLIENT::CreateRecvThread(void*args)
{
    // unsigned char recv_buf[C_MAX_PACK_SIZE] = {0};
    // int recv_len = 0;

	// if(RegisterFrame()<0)
	// 	return 0;	
	// printf("Register Frame Send Suc!\r\n");
	
    // while(1)
    // {
    //     recv_len = 0;
    //     memset(recv_buf,0,C_MAX_PACK_SIZE);
    //     recv_len = read(socket_fd_,recv_buf,C_MAX_PACK_SIZE);
    //     if(recv_len <= 0)
    //     {
    //         printf("sockfd = %d\n",socket_fd_);
    //         RegisterFrame();
    //     }
    //     else
    //     {        
    //         handle(recv_buf,recv_len);
    //     }
    // }
    return 0;
}

int OBU_CLIENT::RegisterFrame(void)
{
    char register_buf[256] = {0};
    int reg_len = 0;
    char client_name[16] = "Demo";

    int index = 0;
    char buf[256] = {0};
    bool recv_flag = false;

	//注册帧
    nebulalink_pro_message::RegisterFrame reg;
    reg.set_element_type(1);
    reg.set_name(client_name,16);

	for(int i = 1; i <= 14; i++)
	{	
		//权限设置
        nebulalink_pro_message::RegisterFrame_SetPermission* per = reg.add_setpermissionvalue();
		per->set_type(i);		
		per->set_level(0x01);
		per->set_time_interval(1000000);	
	}
	
	for(int ix = 33; ix <= 36; ix++)
	{
		nebulalink_pro_message::RegisterFrame_SetPermission* per = reg.add_setpermissionvalue();
		per->set_type(ix);		
		per->set_level(0x01);
		per->set_time_interval(1000000);	
	}
	//注册帧序列化
	bool result;
   	result = reg.SerializeToArray(register_buf, 256);
	reg_len = reg.ByteSize();

	//填充帧结构
    printf("reg_len = %d, result = %s\n",reg_len,result?"true":"false");
    if(reg_len > 0)
    {
		//帧开始frame_start 4 Bytes
        buf[index++] = 0xFA;
        buf[index++] = 0xFB;
        buf[index++] = 0xFC;
        buf[index++] = 0xFD;
		//frame_tpye 2
        buf[index++] = 0x00;
        buf[index++] = 0x01;
		//数据表长度
        buf[index++] = (reg_len>>8) & 0xff;
        buf[index++] = reg_len& 0xff;
		//数据表数据值
		for(int i = 0; i < reg_len; i++)
		{
			buf[index++] = register_buf[i];
			//printf("s:%x",i);
		}
		//帧结束
        buf[index++] = 0xEA;
        buf[index++] = 0xEB;
        buf[index++] = 0xEC;
        buf[index++] = 0xED;
    }
    
    while(!recv_flag)
    {
        int send_len = 0;
        if(!(send_len = write(socket_fd_,buf,index)))
        {
            printf("send error...%d\n",send_len);
            ClientInit();
            continue;
        }
        printf("send successful...%d\n",send_len);
        recv_flag = true;
    }

    return 1;
}

void OBU_CLIENT::handle(unsigned char *buf, int len)
{
    unsigned char data_buf[C_MAX_PACK_SIZE] = {0};
    unsigned short data_len = 0;
    int index = 0;
	unsigned short FrameType;    

    if(len > 0)
    {
		//数据头
        if((buf[index] == 0xFA)&&(buf[index+1] == 0xFB)&&(buf[index+2] == 0xFC)&&(buf[index+3] == 0xFD))
        {
			//数据帧类型
        	FrameType=	(buf[4])<<8 | buf[5] ;
			//数据帧长度
			data_len =	(buf[6])<<8 | buf[7] ;
			//拷贝数据
			memcpy(data_buf,buf+8,data_len);

            printf("FrameType:0x%04x data_len:%d\n",FrameType,data_len);
			bool is_show002 = false;
			bool is_show003 = false;
			bool is_show004 = false;
			bool is_show005 = false;
			bool is_show006 = false;
			bool is_show007 = false;
			bool is_show008 = false;
			bool is_show009 = false;
			bool is_show00a = false;
			bool is_show00b = false;
			bool is_show00c = false;
			bool is_show00d = false;
			bool is_show00e = false;
			bool is_show00f = false;
			bool is_show201 = false;
			bool is_show202 = true;
			bool is_show203 = false;
			bool is_show204 = false;
	
			
            switch(FrameType)
            {

				case 0x0002://自身信息(Host Info)
				{   
					if(!is_show002)break;        
					printf("@@@@@@@@@@@@@@@@@@@@@@ HOST INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					
					nebulalink_pro_message::HostInfo host;
					host.ParseFromArray(data_buf,data_len);
					printf("device_type----------------------> %d \n",host.device_type());
					        
					if(host.has_obu())
					{
						nebulalink_pro_message::HostObu* obu = host.mutable_obu();
						printf("device_id=======================>> %s \n",obu->device_id().c_str());
						printf("expand_info=====================>> %s \n",obu->expand_info().c_str());

						if(obu->has_gnss_info())
						{
							nebulalink_pro_message::GnssInfo* gnssinfo = obu->mutable_gnss_info();
							print_fun->show_gnss_info(gnssinfo);
						}
						
						if(obu->has_veh_info())
						{
							nebulalink_pro_message::VehicleInfo* vehinfo = obu->mutable_veh_info();
							print_fun->show_veh_info(vehinfo);
						}

						if(obu->has_can_info())
						{
							nebulalink_pro_message::CanInfo* caninfo = obu->mutable_can_info();
							print_fun->show_can_info(caninfo);
						}

						for(int msize = 0; msize < obu->historypath_size(); msize++)
						{
							nebulalink_pro_message::PointPosInfo* point = obu->mutable_historypath(msize);
							printf("historypath point=====>> %d <<===========\n",msize);
							print_fun->show_PointPos_Info(point);
							printf("historypath point=====>> %d end<<========\n",msize);
						}	
						
						if(obu->has_predict_path())
						{
							nebulalink_pro_message::PathPrediction* prediction = obu->mutable_predict_path();
							print_fun->show_predic_info(prediction);
						}
					}	

					if(host.has_rsu())
					{
						nebulalink_pro_message::HostRsu* rsu = host.mutable_rsu();
						printf("version=========================>> %d \n",rsu->version());
						printf("device_id=======================>> %s \n",rsu->device_id().c_str());
						printf("longitude=======================>> %f \n",rsu->longitude());
						printf("latitude========================>> %f \n",rsu->latitude());
						printf("altitude========================>> %f \n",rsu->altitude());
					}						
					                              
					printf("@@@@@@@@@@@@@@@@@@@ HOST INFORMATION END @@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;
				
				case 0x0003://非自身OBU(Other Obu)，周边所有OBU
				{
					if(!is_show003)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ OBU INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::OtherObu obus;
					obus.ParseFromArray(data_buf,data_len);
					printf("OBU size-------------->> %d \n",obus.obus_size());
					for(int i = 0; i < obus.obus_size(); i++)
					{
						nebulalink_pro_message::OtherObu_ObuInfo *obu = obus.mutable_obus(i);
						printf("OBU num===============>> %d <<===========\n",i);
						printf("device_id=======================>> %s \n",obu->device_id().c_str());

						if(obu->has_gnss_info())
						{
							nebulalink_pro_message::GnssInfo* gnssinfo = obu->mutable_gnss_info();
							print_fun->show_gnss_info(gnssinfo);
						}
						
						if(obu->has_veh_info())
						{
							nebulalink_pro_message::VehicleInfo* vehinfo = obu->mutable_veh_info();
							print_fun->show_veh_info(vehinfo);
						}

						if(obu->has_can_info())
						{
							nebulalink_pro_message::CanInfo* caninfo = obu->mutable_can_info();
							print_fun->show_can_info(caninfo);
						}

						for(int msize = 0; msize < obu->historypath_size(); msize++)
						{
							nebulalink_pro_message::PointPosInfo* point = obu->mutable_historypath(msize);
							printf("historypath point=====>> %d <<===========\n",msize);
							print_fun->show_PointPos_Info(point);
							printf("historypath point=====>> %d end<<========\n",msize);
						}	

						if(obu->has_predict_path())
						{
							nebulalink_pro_message::PathPrediction* prediction = obu->mutable_predict_path();
							print_fun->show_predic_info(prediction);
						}
						printf("OBU num===============>> %d end<<========\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@ OBU INFORMATION END @@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x0004://非自身RSU(Other Rsu)
				{
					if(!is_show004)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ RSU INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::OtherRsu rsus;
					rsus.ParseFromArray(data_buf,data_len);
					printf("RSU size-------------->> %d \n",rsus.rsus_size());
					for(int i = 0; i < rsus.rsus_size(); i++)
					{
						nebulalink_pro_message::OtherRsu_RsuInfo* rsu = rsus.mutable_rsus(i);
						printf("RSU num===============>> %d <<===========\n",i);
						
						printf("version=========================>> %d \n",rsu->version());
						printf("device_id=======================>> %s \n",rsu->device_id().c_str());
						printf("longitude=======================>> %f \n",rsu->longitude());
						printf("latitude========================>> %f \n",rsu->latitude());
						printf("altitude========================>> %f \n",rsu->altitude());

						printf("RSU num===============>> %d end<<========\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@ RSU INFORMATION END @@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x0005://路侧交通参与者信息：机动车(PTV)
				{
					if(!is_show005)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ PTV INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::PTV ptvs;
					ptvs.ParseFromArray(data_buf,data_len);
					printf("PTV size-------------->> %d \n",ptvs.ptvs_size());
					for(int i = 0; i < ptvs.ptvs_size(); i++)
					{
						nebulalink_pro_message::PtvInfomation* ptv = ptvs.mutable_ptvs(i);
						printf("PTV num===============>> %d <<===========\n",i);
						
						printf("type============================>> %d \n",ptv->type());
						printf("detect_id=======================>> %d \n",ptv->detect_id());
						printf("source_type=====================>> %d \n",ptv->source_type());
						printf("source_id=======================>> %s \n",ptv->source_id().c_str());
						
                        if(ptv->has_gnss_info())
                        {
							nebulalink_pro_message::GnssInfo* gnssinfo = ptv->mutable_gnss_info();
							print_fun->show_gnss_info(gnssinfo);
                        }
						
						if(ptv->has_veh_info())
						{
							nebulalink_pro_message::VehicleInfo* vehinfo = ptv->mutable_veh_info();
							print_fun->show_veh_info(vehinfo);
						}

						for(int msize = 0; msize < ptv->history_points_size(); msize++)
						{
							nebulalink_pro_message::PointPosInfo* point = ptv->mutable_history_points(msize);
							printf("historypath point=====>> %d <<===========\n",msize);
							print_fun->show_PointPos_Info(point);
							printf("historypath point=====>> %d end<<========\n",msize);
						}	
						
						printf("PTV num===============>> %d end<<========\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@ PTV INFORMATION END @@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x0006://路侧交通参与者信息：非机动车(PNV)
				{
					if(!is_show006)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ PNV INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::PNV pnvs;
					pnvs.ParseFromArray(data_buf,data_len);
					printf("PNV size-------------->> %d \n",pnvs.pnvs_size());
					for(int i = 0; i < pnvs.pnvs_size(); i++)
					{
						nebulalink_pro_message::PnvInfomation* pnv = pnvs.mutable_pnvs(i);
						printf("PNV num===============>> %d <<===========\n",i);
						
						printf("type============================>> %d \n",pnv->type());
						printf("detect_id=======================>> %d \n",pnv->detect_id());
						printf("source_type=====================>> %d \n",pnv->source_type());
						printf("source_id=======================>> %s \n",pnv->source_id().c_str());
						
						if(pnv->has_gnss_info())
						{
							nebulalink_pro_message::GnssInfo* gnssinfo = pnv->mutable_gnss_info();
							print_fun->show_gnss_info(gnssinfo);
						}
							
						for(int msize = 0; msize < pnv->history_points_size(); msize++)
						{
							nebulalink_pro_message::PointPosInfo* point = pnv->mutable_history_points(msize);
							printf("historypath point=====>> %d <<===========\n",msize);
							print_fun->show_PointPos_Info(point);
							printf("historypath point=====>> %d end<<========\n",msize);
						}	

						printf("PNV num===============>> %d end<<========\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@ PNV INFORMATION END @@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x0007://逻辑地图(Logical Map)
				{
					if(!is_show007)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ LOGICAL MAP @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::LogicalMap map;
					map.ParseFromArray(data_buf,data_len);
					print_fun->show_time(map.time());
					printf("Node size------------->> %d \n",map.nodes_size());
					for(int i = 0; i < map.nodes_size(); i++)
					{
						nebulalink_pro_message::MapNode* node = map.mutable_nodes(i);
						printf("Node num-------------->> %d <<-----------\n",i);

						printf("longitude=======================>> %f \n",node->longitude());
						printf("latitude========================>> %f \n",node->latitude());
						printf("altitude========================>> %f \n",node->altitude());
						printf("name============================>> %s \n",node->name().c_str());
						printf("node_id=========================>> %" PRIu64 " \n",node->node_id());
						
						printf("Link size=============>> %d \n",node->links_size());
						for(int linksize = 0; linksize < node->links_size(); linksize++)
						{
							nebulalink_pro_message::MapLink* link = node->mutable_links(linksize);
							printf("Link num==============>> %d <<===========\n",linksize);

							printf("upstr_node_id*******************>> %" PRIu64 " \n",link->upstr_node_id());
							printf("name****************************>> %s \n",link->name().c_str());
							printf("lane_width**********************>> %f \n",link->lane_width());
							printf("limit size*************>> %d \n",link->limits_size());
							for(int limitsize = 0; limitsize < link->limits_size(); limitsize++)
							{
								nebulalink_pro_message::SpeedLimitInfo* limits = link->mutable_limits(limitsize);
								printf("limit num*************>> %d <<***********\n",limitsize);
								printf("speed_limit_type &&&&&&&&&&&&&&&>> %d \n",limits->speed_limit_type());
								printf("speed &&&&&&&&&&&&&&&&&&&&&&&&&&>> %f \n",limits->speed());
								printf("limit num*************>> %d end<<********\n",limitsize);
							}

							printf("Point size************>> %d \n",link->points_size());
							for(int pointsize = 0; pointsize < link->points_size(); pointsize++)
							{
								nebulalink_pro_message::PointSiteInfo* ponit = link->mutable_points(pointsize);
								printf("Point num*************>> %d <<***********\n",pointsize);
								printf("longitude &&&&&&&&&&&&&&&&&&&&&&>> %f \n",ponit->longitude());
								printf("latitude &&&&&&&&&&&&&&&&&&&&&&&>> %f \n",ponit->latitude());
								printf("altitude &&&&&&&&&&&&&&&&&&&&&&&>> %f \n",ponit->altitude());
								printf("Point num*************>> %d end<<********\n",pointsize);
							}

							printf("Move size*************>> %d \n",link->movements_size());
							for(int movesize = 0; movesize < link->movements_size(); movesize++)
							{
								nebulalink_pro_message::LinkMovement* move = link->mutable_movements(movesize);
								printf("Move num**************>> %d <<***********\n",movesize);
								printf("node_id &&&&&&&&&&&&&&&&&&&&&&&&>> %" PRIu64 " \n",move->node_id());
								printf("phase_id &&&&&&&&&&&&&&&&&&&&&&&>> %d \n",move->phase_id());								
								printf("Move num**************>> %d end<<********\n",movesize);
							}
							
							printf("Lane size*************>> %d \n",link->lanes_size());
							for(int lanesize = 0; lanesize < link->lanes_size(); lanesize++)
							{
								nebulalink_pro_message::MapLane* lane = link->mutable_lanes(lanesize);
								printf("Lane num**************>> %d <<***********\n",lanesize);

								printf("lane_id &&&&&&&&&&&&&&&&&&&&&&&&>> %d \n",lane->lane_id());
								printf("lane_width &&&&&&&&&&&&&&&&&&&&&>> %d \n",lane->lane_width());
								printf("allowed_maneuvers &&&&&&&&&&&&&&>> %d \n",lane->allowed_maneuvers());
								printf("lane_sharing &&&&&&&&&&&&&&&&&&&>> %d \n",lane->lane_sharing());
								printf("lane_attributes_type &&&&&&&&&&&>> %d \n",lane->lane_attributes_type());

								printf("Lane limit size &&&&&&>> %d \n",lane->limits_size());
								for(int lanelimitesize = 0; lanelimitesize < lane->limits_size(); lanelimitesize++)
								{
									nebulalink_pro_message::SpeedLimitInfo* lane_limit = lane->mutable_limits(lanelimitesize);

									printf("Lane limit num &&&&&&&>> %d <<&&&&&&&&&&&\n",lanelimitesize);
									printf("speed_limit_type $$$$$$$$$$$$$$$>> %d \n",lane_limit->speed_limit_type());
									printf("speed $$$$$$$$$$$$$$$$$$$$$$$$$$>> %f \n",lane_limit->speed());
									printf("Lane limit num &&&&&&&>> %d end<<&&&&&&&&\n",lanelimitesize);
								}
								
								printf("Lane Connect size &&&&&>> %d \n",lane->connections_size());
								for(int laneConnectsize = 0; laneConnectsize < lane->connections_size(); laneConnectsize++)
								{
									nebulalink_pro_message::LaneConnection* lane_connect = lane->mutable_connections(laneConnectsize);

									printf("Lane Connect num &&&&&>> %d <<&&&&&&&&&&&\n",laneConnectsize);
									printf("node_id $$$$$$$$$$$$$$$$$$$$$$$$>> %" PRIu64 " \n",lane_connect->node_id());
									printf("phase_id $$$$$$$$$$$$$$$$$$$$$$$>> %d \n",lane_connect->phase_id());
									printf("lane_id $$$$$$$$$$$$$$$$$$$$$$$$>> %d \n",lane_connect->lane_id());
									printf("allowed_maneuvers $$$$$$$$$$$$$$>> %d \n",lane_connect->allowed_maneuvers());
									printf("Lane Connect num &&&&&>> %d end<<&&&&&&&&\n",laneConnectsize);
								}
								
								printf("Lane point size &&&&&&&>> %d \n",lane->points_size());
								for(int lanepointsize = 0; lanepointsize < lane->points_size(); lanepointsize++)
								{
									nebulalink_pro_message::PointSiteInfo* lane_point = lane->mutable_points(lanepointsize);

									printf("Lane point num &&&&&&&>> %d <<&&&&&&&&&&&\n",lanepointsize);
									printf("longitude $$$$$$$$$$$$$$$$$$$$$$>> %f \n",lane_point->longitude());
									printf("latitude $$$$$$$$$$$$$$$$$$$$$$$>> %f \n",lane_point->latitude());
									printf("altitude $$$$$$$$$$$$$$$$$$$$$$$>> %f \n",lane_point->altitude());
									printf("Lane point num &&&&&&&>> %d end<<&&&&&&&&\n",lanepointsize);
								}

								printf("Lane num**************>> %d end<<********\n",lanesize);
							}
							printf("Link num==============>> %d end<<========\n",linksize);
						}
						printf("Node num-------------->> %d end<<--------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ LOGICAL MAP End @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;
				
				case 0x0008://道路交通标志信息(RTS)
				{
					if(!is_show008)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTS INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::RTS rts;
					rts.ParseFromArray(data_buf,data_len);
					printf("RTS size------------->> %d \n",rts.rtses_size());
					for(int i = 0; i < rts.rtses_size(); i++)
					{
						nebulalink_pro_message::RtsData* rtsData = rts.mutable_rtses(i);
						printf("Rts num-------------->> %d <<-----------\n",i);
						printf("rts_id==========================>> %d \n",rtsData->rts_id());
						printf("rts_type========================>> %d \n",rtsData->rts_type());
						printf("rts_longitude===================>> %f \n",rtsData->rts_longitude());
						printf("rts_latitude====================>> %f \n",rtsData->rts_latitude());
						printf("rts_altitude====================>> %f \n",rtsData->rts_altitude());
						printf("rts_description=================>> %s \n",rtsData->rts_description().c_str());
						printf("rts_start_");
						print_fun->show_time(rtsData->rts_start_time());
						printf("rts_end_");
						print_fun->show_time(rtsData->rts_end_time());
						printf("rts_priority====================>> %d \n",rtsData->rts_priority());
						printf("rsu_longitude===================>> %f \n",rtsData->rsu_longitude());
						printf("rsu_latitude====================>> %f \n",rtsData->rsu_latitude());
						printf("rsu_altitude====================>> %f \n",rtsData->rsu_altitude());
						printf("rsu_");
						print_fun->show_time(rtsData->rsu_time());
						printf("rsu_id==========================>> %s \n",rtsData->rsu_id().c_str());
						printf("Path size=============>> %d \n",rtsData->paths_size());
						for(int pathsize = 0; pathsize < rtsData->paths_size(); pathsize++)
						{
							nebulalink_pro_message::RelatePathInfo* path = rtsData->mutable_paths(pathsize);
							printf("Path num==============>> %d <<===========\n",pathsize);
							print_fun->show_relate_path(path);
							printf("Path num==============>> %d end<<========\n",pathsize);
						}
						
						printf("Link size=============>> %d \n",rtsData->links_size());
						for(int linksize = 0; linksize < rtsData->links_size(); linksize++)
						{
							nebulalink_pro_message::RelateLinkInfo* link = rtsData->mutable_links(linksize);
							printf("Link num==============>> %d <<===========\n",linksize);
							print_fun->show_relate_link(link);
							printf("Link num==============>> %d end<<========\n",linksize);
						}
						printf("Rts num-------------->> %d end<<--------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTS INFORMATION End @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;
				
				case 0x0009://道路交通事件信息(RTE)
				{
					if(!is_show009)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTE INFORMATION @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::RTE rte;
					rte.ParseFromArray(data_buf,data_len);
					printf("RTE size------------->> %d \n",rte.rtes_size());
					for(int i = 0; i < rte.rtes_size(); i++)
					{
						nebulalink_pro_message::RteData* rteData = rte.mutable_rtes(i);
						printf("Rte num-------------->> %d <<-----------\n",i);
						printf("rte_id==========================>> %d \n",rteData->rte_id());
						printf("rte_type========================>> %d \n",rteData->rte_type());
						printf("rte_source======================>> %d \n",rteData->rte_source());
						printf("rte_longitude===================>> %f \n",rteData->rte_longitude());
						printf("rte_latitude====================>> %f \n",rteData->rte_latitude());
						printf("rte_altitude====================>> %f \n",rteData->rte_altitude());
						printf("rte_radius======================>> %f \n",rteData->rte_radius());
						printf("rte_description=================>> %s \n",rteData->rte_description().c_str());
						printf("rts_start_");
						print_fun->show_time(rteData->rte_start_time());
						printf("rts_end_");
						print_fun->show_time(rteData->rte_end_time());
						printf("rte_priority====================>> %d \n",rteData->rte_priority());
						printf("rsu_longitude===================>> %f \n",rteData->rsu_longitude());
						printf("rsu_latitude====================>> %f \n",rteData->rsu_latitude());
						printf("rsu_altitude====================>> %f \n",rteData->rsu_altitude());
						printf("rsu_");
						print_fun->show_time(rteData->rsu_time());
						printf("rsu_id==========================>> %s \n",rteData->rsu_id().c_str());
						printf("Path size=============>> %d \n",rteData->paths_size());
						for(int pathsize = 0; pathsize < rteData->paths_size(); pathsize++)
						{
							nebulalink_pro_message::RelatePathInfo* path = rteData->mutable_paths(pathsize);
							printf("Path num==============>> %d <<===========\n",pathsize);
							print_fun->show_relate_path(path);
							printf("Path num==============>> %d end<<========\n",pathsize);
						}
						
						printf("Link size=============>> %d \n",rteData->links_size());
						for(int linksize = 0; linksize < rteData->links_size(); linksize++)
						{
							nebulalink_pro_message::RelateLinkInfo* link = rteData->mutable_links(linksize);
							printf("Link num==============>> %d <<===========\n",linksize);
							print_fun->show_relate_link(link);
							printf("Link num==============>> %d end<<========\n",linksize);
						}						
						printf("Rte num-------------->> %d end<<--------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTE INFORMATION End @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x000a://信号灯信息(Traffic Light)
				{
					if(!is_show00a)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ TRAFFIC LIGHT @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::TrafficLight tl;
					tl.ParseFromArray(data_buf,data_len);
					printf("Traffic Light size------------->> %d \n",tl.trafficlights_size());
					for(int i = 0; i < tl.trafficlights_size(); i++)
					{
						nebulalink_pro_message::TrafficLight_TrafficInformation* tlinfo = tl.mutable_trafficlights(i);
						printf("Traffic Light num-------------->> %d <<-----------\n",i);

						printf("description=====================>> %s \n",tlinfo->description().c_str());
						printf("intersection_region_node_id=====>> %" PRIu64 " \n",tlinfo->intersection_region_node_id());
						printf("status==========================>> %d \n",tlinfo->status());
						print_fun->show_time(tlinfo->time());
						printf("time_confidence=================>> %d \n",tlinfo->time_confidence());
						printf("Phase size======================>> %d \n",tlinfo->phases_size());
						for(int phasesize = 0; phasesize < tlinfo->phases_size(); phasesize++)
						{
							nebulalink_pro_message::TrafficLightPhase* phase = tlinfo->mutable_phases(phasesize);
							printf("Phase num==============>> %d <<===========\n",phasesize);
							printf("phase_id***********************>>> %d \n",phase->phase_id());
							printf("Status size********************>>> %d \n",phase->status_size());
							for(int stasize = 0; stasize < phase->status_size(); stasize++)
							{
								nebulalink_pro_message::PhaseStatus* status = phase->mutable_status(stasize);	
								printf("Phase num**************>> %d <<***********\n",stasize);
								printf("light_status &&&&&&&&&&&&&&&&&&>>> %d \n",status->light_status());
								printf("start_time &&&&&&&&&&&&&&&&&&&&>>> %f \n",status->start_time());
								printf("min_end_time &&&&&&&&&&&&&&&&&&>>> %f \n",status->min_end_time());
								printf("max_end_time &&&&&&&&&&&&&&&&&&>>> %f \n",status->max_end_time());
								printf("end_time &&&&&&&&&&&&&&&&&&&&&&>>> %f \n",status->end_time());
								printf("time_confidence &&&&&&&&&&&&&&&>>> %d \n",status->time_confidence());
								printf("next_start_time &&&&&&&&&&&&&&&>>> %f \n",status->next_start_time());
								printf("next_cycle &&&&&&&&&&&&&&&&&&&&>>> %f \n",status->next_cycle());
								printf("Phase num**************>> %d end<<********\n",stasize);
							}
							printf("Phase num==============>> %d end<<========\n",phasesize);
						}
						
						printf("Traffic Light num-------------->> %d end<<--------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ TRAFFIC LIGHT END @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x000b://车路预警(Cdas Alert)
				{
					if(!is_show00b)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ CDAS ALERT @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::CdasAlert ca;
					ca.ParseFromArray(data_buf,data_len);
					printf("Target List size------------->> %d \n",ca.target_list_size());
					for(int i = 0; i < ca.target_list_size(); i++)
					{
						nebulalink_pro_message::TargetInfo* target = ca.mutable_target_list(i);
						printf("Target List num-------------->> %d <<-----------\n",i);
						
						printf("target_type=====================>> %d \n",target->target_type());
						printf("device_id=======================>> %s \n",target->device_id().c_str());
						if(target->has_gnss())
						{
							nebulalink_pro_message::GnssInfo* gnss = target->mutable_gnss();
							print_fun->show_gnss_info(gnss);
						}
						if(target->has_veh_info())
						{
							nebulalink_pro_message::VehicleInfo* veh = target->mutable_veh_info();
							print_fun->show_veh_info(veh);
						}
						if(target->has_lm_res())
						{
							printf("================Has Logic Map Result================ \n");
							nebulalink_pro_message::LogicMapResult* lmres = target->mutable_lm_res();
							if(lmres->has_same_link())
							{
								printf("==================Has Same Link===================== \n");
								nebulalink_pro_message::SameDirectionLink* same_link = lmres->mutable_same_link();
								printf("from_node*********************>> %" PRIu64 " \n",same_link->from_node());
								printf("to_node***********************>> %" PRIu64 " \n",same_link->to_node());
								printf("lane_info*********************>> %s \n",same_link->lane_info()?"true":"false");
								printf("lane_num**********************>> %d \n",same_link->lane_num());
								printf("lon_dis***********************>> %f \n",same_link->lon_dis());
								printf("lat_dis***********************>> %f \n",same_link->lat_dis());
							}
							
							if(lmres->has_opposite())
							{
								printf("==================Has Opposite Link================== \n");
								nebulalink_pro_message::OppositeDirectionLink* opposite = lmres->mutable_opposite();
								printf("from_node*********************>> %" PRIu64 " \n",opposite->from_node());
								printf("to_node***********************>> %" PRIu64 " \n",opposite->to_node());
								printf("lane_info*********************>> %s \n",opposite->lane_info()?"true":"false");
								printf("lane_num**********************>> %d \n",opposite->lane_num());
								printf("lon_dis***********************>> %f \n",opposite->lon_dis());
								printf("lat_dis***********************>> %f \n",opposite->lat_dis());
							}
							
							if(lmres->has_intersection())
							{
								printf("===================Has Intersection ================= \n");
								nebulalink_pro_message::ApproachIntersection* intersection = lmres->mutable_intersection();
								printf("from_node*********************>> %" PRIu64 " \n",intersection->from_node());
								printf("to_node***********************>> %" PRIu64 " \n",intersection->to_node());
								printf("remote_node*******************>> %" PRIu64 " \n",intersection->remote_node());
								printf("hv_dis************************>> %f \n",intersection->hv_dis());
								printf("rv_dis************************>> %f \n",intersection->rv_dis());
							}
						}
						if(target->has_ph_res())
						{
							printf("================Has Path Histroy Result================ \n");
							nebulalink_pro_message::PathHistroyResult* phres = target->mutable_ph_res();
							printf("type**************************>> %d \n",phres->type());
							printf("lon_dis***********************>> %f \n",phres->lon_dis());
							printf("lat_dis***********************>> %f \n",phres->lat_dis());
							printf("head_diff*********************>> %f \n",phres->head_diff());
							printf("time_dis**********************>> %f \n",phres->time_dis());
						}
						if(target->has_pp_res())
						{
							printf("================Has Path Predict Result================ \n");
							nebulalink_pro_message::PathPredictResult* ppres = target->mutable_pp_res();
							printf("type**************************>> %d \n",ppres->type());
							printf("lon_dis***********************>> %f \n",ppres->lon_dis());
							printf("lat_dis***********************>> %f \n",ppres->lat_dis());
							printf("hv_node_dis*******************>> %f \n",ppres->hv_node_dis());
							printf("rv_node_dis*******************>> %f \n",ppres->rv_node_dis());
							printf("head_diff*********************>> %f \n",ppres->head_diff());
							printf("time_dis**********************>> %f \n",ppres->time_dis());
							printf("node_time_dis*****************>> %f \n",ppres->node_time_dis());
							printf("confidence********************>> %d \n",ppres->confidence());
						}
						if(target->has_pos_res())
						{
							printf("==================Has Position Result================== \n");
							nebulalink_pro_message::PositionResult* posres = target->mutable_pos_res();
							printf("angle*************************>> %f \n",posres->angle());
							printf("distance**********************>> %f \n",posres->distance());
							printf("lon_dis***********************>> %f \n",posres->lon_dis());
							printf("lat_dis***********************>> %f \n",posres->lat_dis());
							printf("dir_diff**********************>> %f \n",posres->dir_diff());
							printf("hv_node_dis*******************>> %f \n",posres->hv_node_dis());
							printf("rv_node_dis*******************>> %f \n",posres->rv_node_dis());
							printf("confidence********************>> %d \n",posres->confidence());
						}
						if(target->has_td_res())
						{
							printf("===============Has Target Direction Desc=============== \n");
							nebulalink_pro_message::TargetDirectionDesc* targetres = target->mutable_td_res();
							printf("direction*********************>> %d \n",targetres->direction());
							printf("confidence********************>> %d \n",targetres->confidence());
						}
						if(target->has_tw_res())
						{
							printf("==================Has Target Warn Desc================= \n");
							nebulalink_pro_message::TargetWarnDesc* targetwarn = target->mutable_tw_res();
							printf("Target warn size==============>> %d \n",targetwarn->tw_info_size());
							for(int ix = 0; ix < targetwarn->tw_info_size(); ix++)
							{
								nebulalink_pro_message::TargetWarnInfo* TargetInfo = targetwarn->mutable_tw_info(ix);
								printf("Target warn num==============>> %d <<===========\n",ix);

								if(TargetInfo->has_fcw())
								{
									printf("******************Has FCW Info***************** \n");
									nebulalink_pro_message::FCWInfoDesc* fcw = TargetInfo->mutable_fcw();
									printf("confidence********************>> %d \n",fcw->confidence());
									printf("c_time************************>> %f \n",fcw->c_time());
									printf("h_time************************>> %f \n",fcw->h_time());
									printf("subtype***********************>> %d \n",fcw->subtype());
									printf("level*************************>> %d \n",fcw->level());
								}
								if(TargetInfo->has_icw())
								{
									printf("******************Has ICW Info***************** \n");
									nebulalink_pro_message::ICWInfoDesc* icw = TargetInfo->mutable_icw();
									printf("confidence********************>> %d \n",icw->confidence());
									printf("c_time************************>> %f \n",icw->c_time());
									printf("d_time************************>> %f \n",icw->d_time());
									printf("subtype***********************>> %d \n",icw->subtype());
									printf("level*************************>> %d \n",icw->level());
								}
								if(TargetInfo->has_clw())
								{
									printf("******************Has CLW Info***************** \n");
									nebulalink_pro_message::CLWInfoDesc* clw = TargetInfo->mutable_clw();
									printf("confidence********************>> %d \n",clw->confidence());
									printf("c_time************************>> %f \n",clw->c_time());
									printf("h_time************************>> %f \n",clw->h_time());
									printf("subtype***********************>> %d \n",clw->subtype());
									printf("level*************************>> %d \n",clw->level());
								}
								if(TargetInfo->has_avw())
								{
									printf("******************Has AVW Info***************** \n");
									nebulalink_pro_message::AVWInfoDesc* avw = TargetInfo->mutable_avw();
									printf("confidence********************>> %d \n",avw->confidence());
									printf("c_time************************>> %f \n",avw->c_time());
									printf("h_time************************>> %f \n",avw->h_time());
									printf("subtype***********************>> %d \n",avw->subtype());
									printf("level*************************>> %d \n",avw->level());
								}
								if(TargetInfo->has_ebw())
								{
									printf("******************Has EBW Info***************** \n");
									nebulalink_pro_message::EBWInfoDesc* ebw = TargetInfo->mutable_ebw();
									printf("confidence********************>> %d \n",ebw->confidence());
									printf("c_time************************>> %f \n",ebw->c_time());
									printf("h_time************************>> %f \n",ebw->h_time());
									printf("subtype***********************>> %d \n",ebw->subtype());
									printf("level*************************>> %d \n",ebw->level());
								}
								if(TargetInfo->has_dnpw())
								{
									printf("******************Has DNPW Info***************** \n");
									nebulalink_pro_message::DNPWInfoDesc* dnpw = TargetInfo->mutable_dnpw();
									printf("confidence********************>> %d \n",dnpw->confidence());
									printf("c_time************************>> %f \n",dnpw->c_time());
									printf("subtype***********************>> %d \n",dnpw->subtype());
									printf("level*************************>> %d \n",dnpw->level());
								}
								if(TargetInfo->has_lcw())
								{
									printf("******************Has LCW Info***************** \n");
									nebulalink_pro_message::LCWInfoDesc* lcw = TargetInfo->mutable_lcw();
									printf("confidence********************>> %d \n",lcw->confidence());
									printf("c_time************************>> %f \n",lcw->c_time());
									printf("h_time************************>> %f \n",lcw->h_time());
									printf("subtype***********************>> %d \n",lcw->subtype());
									printf("level*************************>> %d \n",lcw->level());
								}
								if(TargetInfo->has_lta())
								{
									printf("******************Has LTA Info***************** \n");
									nebulalink_pro_message::LTAInfoDesc* lta = TargetInfo->mutable_lta();
									printf("confidence********************>> %d \n",lta->confidence());
									printf("d_time************************>> %f \n",lta->d_time());
									printf("subtype***********************>> %d \n",lta->subtype());
									printf("level*************************>> %d \n",lta->level());
								}
								if(TargetInfo->has_evw())
								{
									printf("******************Has EVW Info***************** \n");
									nebulalink_pro_message::EVWInfoDesc* evw = TargetInfo->mutable_evw();
									printf("confidence********************>> %d \n",evw->confidence());
									printf("c_time************************>> %f \n",evw->c_time());
									printf("h_time************************>> %f \n",evw->h_time());
									printf("subtype***********************>> %d \n",evw->subtype());
									printf("level*************************>> %d \n",evw->level());
								}
								if(TargetInfo->has_fvw())
								{
									printf("******************Has FVW Info***************** \n");
									nebulalink_pro_message::FVWInfoDesc* fvw = TargetInfo->mutable_fvw();
									printf("confidence********************>> %d \n",fvw->confidence());
									printf("h_time************************>> %f \n",fvw->h_time());
									printf("subtype***********************>> %d \n",fvw->subtype());
									printf("level*************************>> %d \n",fvw->level());
								}
								if(TargetInfo->has_bsw())
								{
									printf("******************Has BSW Info***************** \n");
									nebulalink_pro_message::BSWInfoDesc* bsw = TargetInfo->mutable_bsw();
									printf("confidence********************>> %d \n",bsw->confidence());
									printf("c_time************************>> %f \n",bsw->c_time());
									printf("h_time************************>> %f \n",bsw->h_time());
									printf("subtype***********************>> %d \n",bsw->subtype());
									printf("level*************************>> %d \n",bsw->level());
								}
								if(TargetInfo->has_slw())
								{
									printf("******************Has SLW Info***************** \n");
									nebulalink_pro_message::SLWInfoDesc* slw = TargetInfo->mutable_slw();
									printf("confidence********************>> %d \n",slw->confidence());
									printf("sl_number*********************>> %d \n",slw->sl_number());
									printf("level*************************>> %d \n",slw->level());
								}
								if(TargetInfo->has_rlvw())
								{
									printf("******************Has RLVW Info***************** \n");
									nebulalink_pro_message::RLVWInfoDesc* rlvw = TargetInfo->mutable_rlvw();
									printf("confidence********************>> %d \n",rlvw->confidence());
									printf("distance_to_end***************>> %f \n",rlvw->distance_to_end());
									printf("level*************************>> %d \n",rlvw->level());
								}
								printf("Target warn num==============>> %d end<<========\n",ix);
							}	
						}

						printf("Target List num-------------->> %d end<<--------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ CDAS ALERT END @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x000c://地图处理结果(Map Result)
				{
					if(!is_show00c)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ MAP RESULT @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::MapResult mapres;
					mapres.ParseFromArray(data_buf,data_len);
					printf("from_node_id-------------------->> %" PRIu64 " \n",mapres.from_node_id());
					printf("to_node_id---------------------->> %" PRIu64 " \n",mapres.to_node_id());
					printf("dis_to_end---------------------->> %f \n",mapres.dis_to_end());
					if(mapres.has_lane_limit())
					{
						nebulalink_pro_message::SpeedLimitResult* limit = mapres.mutable_lane_limit();
						printf("lane speed ceiling==============>> %f \n",limit->ceiling());	
					    printf("lane speed floor================>> %f \n",limit->floor()); 
					}
					printf("lane_id------------------------->> %f \n",mapres.dis_to_end());
					if(mapres.has_link_limit())
					{
						nebulalink_pro_message::SpeedLimitResult* limit = mapres.mutable_link_limit();
						printf("link speed ceiling==============>> %f\n",limit->ceiling()); 
						printf("link speed floor================>> %f\n",limit->floor()); 
					}
					printf("lane_to_end--------------------->> %f \n",mapres.lane_to_end());
					printf("lane_type----------------------->> %d \n",mapres.lane_type());
					printf("lane_attribute------------------>> %d \n",mapres.lane_attribute());

					printf("@@@@@@@@@@@@@@@@@@@@@@ MAP RESULT END @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x000d://RTS处理结果(Rts Result)
				{
					if(!is_show00d)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTS RESULT @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::RtsResult rtsres;
					rtsres.ParseFromArray(data_buf,data_len);
					printf("Rts Result size------------->> %d \n",rtsres.rtsresults_size());
					for(int i = 0; i < rtsres.rtsresults_size(); i++)
					{
						nebulalink_pro_message::RtsResult_RtsResultInformation* res = rtsres.mutable_rtsresults(i);
						printf("Rts Result num------------------>> %d <<-----------\n",i);
						printf("type============================>> %d \n",res->type());
						printf("description=====================>> %s \n",res->description().c_str());
						printf("dis_to_end======================>> %f \n",res->dis_to_end());
						printf("rdis_to_path====================>> %f \n",res->rdis_to_path());
						printf("upper_node_id===================>> %" PRIu64 " \n",res->upper_node_id());
						printf("down_node_id====================>> %" PRIu64 " \n",res->down_node_id());
						printf("rts_id==========================>> %d \n",res->rts_id());
						printf("rsu_id==========================>> %s \n",res->rsu_id().c_str());
						printf("Rts Result num------------------>> %d end<<--------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTS RESULT END @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x000e://RTE处理结果(Rte Result)
				{
					if(!is_show00e)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTE RESULT @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::RteResult rteres;
					rteres.ParseFromArray(data_buf,data_len);
					printf("Rte Result size------------->> %d \n",rteres.rteresults_size());
					for(int i = 0; i < rteres.rteresults_size(); i++)
					{
						nebulalink_pro_message::RteResult_RteResultInformation* res = rteres.mutable_rteresults(i);
						printf("Rte Result num----------------->> %d <<-----------\n",i);
						printf("type============================>> %d \n",res->type());
						printf("source==========================>> %d \n",res->source());
						printf("description=====================>> %s \n",res->description().c_str());
						printf("dis_to_end======================>> %f \n",res->dis_to_end());
						printf("rdis_to_path====================>> %f \n",res->rdis_to_path());
						printf("upper_node_id===================>> %" PRIu64 " \n",res->upper_node_id());
						printf("down_node_id====================>> %" PRIu64 " \n",res->down_node_id());
						printf("rte_id==========================>> %d \n",res->rte_id());
						printf("rsu_id==========================>> %s \n",res->rsu_id().c_str());
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ RTE RESULT END @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x000f://信号灯信息处理结果(Traffic Light Result)
				{
					if(!is_show00f)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ TRAFFIC LIGHT RESULT @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::TrafficLightResult tlres;
					tlres.ParseFromArray(data_buf,data_len);
					printf("TR Result size-------------->> %d \n",tlres.trafficlightresults_size());
					for(int i = 0; i < tlres.trafficlightresults_size(); i++)
					{
						nebulalink_pro_message::TrafficLightResult_TrafficLightResultInformation* res = tlres.mutable_trafficlightresults(i);
						printf("TR Result num------------------>> %d <<------------\n",i);
						printf("turner_angle====================>> %f \n",res->turner_angle());
						printf("direction=======================>> %d \n",res->direction());
						printf("light_state=====================>> %d \n",res->light_state());
						printf("time_remaining==================>> %d \n",res->time_remaining());
						if(res->has_lane_limit())
						{
							nebulalink_pro_message::SpeedLimitResult* limit = res->mutable_lane_limit();
							printf("lane speed ceiling==============>> %f \n",limit->ceiling());	
						    printf("lane speed floor================>> %f \n",limit->floor()); 
						}
						if(res->has_link_limit())
						{
							nebulalink_pro_message::SpeedLimitResult* limit = res->mutable_link_limit();
							printf("link speed ceiling==============>> %f\n",limit->ceiling()); 
							printf("link speed floor================>> %f\n",limit->floor()); 
						}
						printf("direction_node_id===============>> %" PRIu64 " \n",res->direction_node_id());
						printf("phase_id========================>> %d \n",res->phase_id());
						printf("decel_red_break=================>> %f \n",res->decel_red_break());
						printf("TR Result num------------------>> %d end<<---------\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ TRAFFIC LIGHT RESULT END @@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;
				
				case 0x0201://车辆意图和请求(Vir List)//02|01
				{
					if(!is_show201)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ VIR LIST  @@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::VIR virlist;
					virlist.ParseFromArray(data_buf,data_len);
					printf("Vir size-------------->> %d \n",virlist.virs_size());
					for(int i = 0; i < virlist.virs_size(); i++)
					{
						nebulalink_pro_message::VirInfo* vir = virlist.mutable_virs(i);
						printf("Vir num===============>> %d <<===========\n",i);
						printf("device_id=======================>> %s \n",vir->device_id().c_str());
						print_fun->show_time(vir->time());
						nebulalink_pro_message::DriveBehaviorInfo* behavior = vir->mutable_behavior_info();
						printf("******************DriveBehaviorInfo************** \n");
						printf("behavior************************>> %d \n",behavior->behavior());
						printf("****************DriveBehaviorInfo End************ \n");
						nebulalink_pro_message::PathPointInfo* pathpoint = vir->mutable_path_point();
						print_fun->show_path_point(pathpoint);
						printf("plan point size================>> %d \n",vir->plan_points_size());
						for(int ix = 0; ix < vir->plan_points_size(); ix++)
						{
							printf("plan point num===============>> %d <<===========\n",ix);
							nebulalink_pro_message::PathPointInfo* pathpoint = vir->mutable_plan_points(ix);
							print_fun->show_path_point(pathpoint);
							printf("plan point num===============>> %d end<<========\n",ix);
						}

						printf("request info size================>> %d \n",vir->request_info_size());
						for(int ix = 0; ix < vir->request_info_size(); ix++)
						{
							nebulalink_pro_message::RequestInfo* reqinfo = vir->mutable_request_info(ix);
							printf("request info num***************>> %d <<***********\n",ix);
							printf("request_id**********************>> %d \n",reqinfo->request_id());
							printf("request_status******************>> %d \n",reqinfo->request_status());
							printf("request_priority****************>> %d \n",reqinfo->request_priority());
							printf("obu_device**********************>> %s \n",reqinfo->obu_device().c_str());
							printf("rsu_device**********************>> %s \n",reqinfo->rsu_device().c_str());
							printf("life_");
							print_fun->show_time(reqinfo->life_time());

							if(reqinfo->has_lane_change())
							{
								nebulalink_pro_message::LaneChangeInfo* lanechange = reqinfo->mutable_lane_change();
								printf("******************LaneChangeInfo************** \n");
								printf("upper_node_id*******************>> %" PRIu64 " \n",lanechange->upper_node_id());
								printf("down_node_id********************>> %" PRIu64 " \n",lanechange->down_node_id());
								printf("change_lane*********************>> %d \n",lanechange->change_lane());
								printf("****************LaneChangeInfo End************ \n");
							}
							
							if(reqinfo->has_clear_way())
							{
								nebulalink_pro_message::ClearWayInfo* clearway = reqinfo->mutable_clear_way();
								printf("*******************ClearWayInfo*************** \n");
								printf("upper_node_id*******************>> %" PRIu64 " \n",clearway->upper_node_id());
								printf("down_node_id********************>> %" PRIu64 " \n",clearway->down_node_id());
								printf("clear_lane**********************>> %d \n",clearway->clear_lane());
								printf("start_");
								print_fun->show_time(clearway->start_time());
								printf("end_");
								print_fun->show_time(clearway->end_time());
								nebulalink_pro_message::RelatePathInfo* path = clearway->mutable_path();
								print_fun->show_relate_path(path);
								printf("*****************ClearWayInfo End************* \n");
							}
							
							if(reqinfo->has_signal_priority())
							{
								nebulalink_pro_message::SignalPriorityInfo* priority = reqinfo->mutable_signal_priority();
								printf("****************SignalPriorityInfo************ \n");
								printf("upper_node_id*******************>> %" PRIu64 " \n",priority->upper_node_id());
								printf("down_node_id********************>> %" PRIu64 " \n",priority->down_node_id());
								printf("phase_id************************>> %d \n",priority->phase_id());
								printf("maneuver************************>> %d \n",priority->maneuver());
								printf("arrive_");
								print_fun->show_time(priority->arrive_time());
								printf("distance************************>> %f \n",priority->distance());
								printf("**************SignalPriorityInfo End********** \n");
							}
							
							if(reqinfo->has_sense_share())
							{
								nebulalink_pro_message::SenseShareingInfo* senseshare = reqinfo->mutable_sense_share();
								printf("*****************SenseShareingInfo************ \n");
								printf("path size***********************>> %d \n",senseshare->paths_size());
								for(int ii = 0; ii < senseshare->paths_size(); ii++)
								{
									printf("path num&&&&&&&&&&&&&&&&&&&&&>> %d <<&&&&&&&&&&&\n",ii);
									nebulalink_pro_message::RelatePathInfo* path = senseshare->mutable_paths(ii);
									print_fun->show_relate_path(path);
									printf("plan num&&&&&&&&&&&&&&&&&&&&&>> %d end<<&&&&&&&&\n",ii);
								}								
								printf("***************SenseShareingInfo End********** \n");
							}
							
							if(reqinfo->has_parking())
							{
								nebulalink_pro_message::ParkingInfo* parking = reqinfo->mutable_parking();
								printf("********************ParkingInfo*************** \n");
								printf("vehicle_class*******************>> %d \n",parking->vehicle_class());
								printf("fuel_type***********************>> %d \n",parking->fuel_type());
								printf("parking_req*********************>> %d \n",parking->parking_req());
								printf("parking_type********************>> %d \n",parking->parking_type());
								printf("slot_id*************************>> %d \n",parking->slot_id());
								printf("*****************ParkingInfo End************** \n");
							}
							
							printf("request info num***************>> %d end<<********\n",ix);
						}
						
						printf("Vir num===============>> %d end<<========\n",i);
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ VIR LIST END @@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x0202://感知共享消息(SSM)
				{
					if(!is_show202)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ SSM @@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::SsmInfo ssm;
					ssm.ParseFromArray(data_buf,data_len);
					
					printf("polygons size--------------->> %d \n",ssm.polygons_size());//共享多边形区域
					for(int i = 0; i < ssm.polygons_size(); i++)
					{
						printf("polygons num------------------->> %d <<------------\n",i);
						nebulalink_pro_message::PolygonInfo* polygon = ssm.mutable_polygons(i);
						printf("point site size================>> %d \n",polygon->points_size());
						for(int ix = 0; ix < polygon->points_size(); ix++)
						{
							nebulalink_pro_message::PointSiteInfo* point = polygon->mutable_points(ix);
							printf("point site num=============>> %d <<===========\n",ix);
							print_fun->show_point_site(point);
							printf("point site num=============>> %d end<<========\n",ix);		
						}
						printf("polygons num------------------->> %d end<<---------\n",i);
					}

					printf("ptcs size------------------->> %d \n",ssm.detect_ptcs_size());
					for(int i = 0; i < ssm.detect_ptcs_size(); i++)
					{
						nebulalink_pro_message::DetectPtcInfo* ptc = ssm.mutable_detect_ptcs(i);
						printf("ptcs num----------------------->> %d <<------------\n",i);

						if(ptc->has_ptcs())
						{
							nebulalink_pro_message::PtcDescInfo* ptcinfo = ptc->mutable_ptcs();
							if(ptcinfo->has_ptv())
							{
								printf("=========================PtvInfomation=====================\n");
								nebulalink_pro_message::PtvInfomation* ptv = ptcinfo->mutable_ptv();
								
								printf("type============================>> %d \n",ptv->type());
								printf("detect_id=======================>> %d \n",ptv->detect_id());
								printf("source_type=====================>> %d \n",ptv->source_type());
								printf("source_id=======================>> %s \n",ptv->source_id().c_str());
								
								if(ptv->has_gnss_info())
								{
									nebulalink_pro_message::GnssInfo* gnssinfo = ptv->mutable_gnss_info();
									print_fun->show_gnss_info(gnssinfo);
								}
								
								if(ptv->has_veh_info())
								{
									nebulalink_pro_message::VehicleInfo* vehinfo = ptv->mutable_veh_info();
									print_fun->show_veh_info(vehinfo);
								}
								
								for(int msize = 0; msize < ptv->history_points_size(); msize++)
								{
									nebulalink_pro_message::PointPosInfo* point = ptv->mutable_history_points(msize);
									printf("historypath point=====>> %d <<===========\n",msize);
									print_fun->show_PointPos_Info(point);
									printf("historypath point=====>> %d end<<========\n",msize);
								}	
							}
							
							if(ptcinfo->has_pnv())
							{
								printf("=========================PnvInfomation=====================\n");
								nebulalink_pro_message::PnvInfomation* pnv = ptcinfo->mutable_pnv();
								
								printf("type============================>> %d \n",pnv->type());
								printf("detect_id=======================>> %d \n",pnv->detect_id());
								printf("source_type=====================>> %d \n",pnv->source_type());
								printf("source_id=======================>> %s \n",pnv->source_id().c_str());
								
								if(pnv->has_gnss_info())
								{
									nebulalink_pro_message::GnssInfo* gnssinfo = pnv->mutable_gnss_info();
									print_fun->show_gnss_info(gnssinfo);
								}
									
								for(int msize = 0; msize < pnv->history_points_size(); msize++)
								{
									nebulalink_pro_message::PointPosInfo* point = pnv->mutable_history_points(msize);
									printf("historypath point=====>> %d <<===========\n",msize);
									print_fun->show_PointPos_Info(point);
									printf("historypath point=====>> %d end<<========\n",msize);
								}	
								
							}
						}
						
						printf("source_type=====================>> %d \n",ptc->source_type());
						printf("status_druing===================>> %d \n",ptc->status_druing());
						printf("tracking========================>> %d \n",ptc->tracking());

						printf("point site size=================>> %d \n",ptc->polygons_size());
						for(int ix = 0; ix < ptc->polygons_size(); ix++)
						{
							nebulalink_pro_message::PointSiteInfo* point = ptc->mutable_polygons(ix);
							printf("point site num=============>> %d <<===========\n",ix);
							print_fun->show_point_site(point);
							printf("point site num=============>> %d end<<========\n",ix);		
						}

						printf("ptcs num----------------------->> %d end<<---------\n",i);
					}
					
					printf("obstacles size-------------->> %d \n",ssm.obstacles_size());
					for(int i = 0; i < ssm.obstacles_size(); i++)
					{
						printf("obstacles num------------------>> %d <<------------\n",i);
						nebulalink_pro_message::ObstacleInfo* obstacle = ssm.mutable_obstacles(i);
						printf("obstacle========================>> %d \n",obstacle->type());
						printf("id==============================>> %d \n",obstacle->id());
						printf("source_type=====================>> %d \n",obstacle->source_type());
						printf("longitude=======================>> %f \n",obstacle->longitude());
						printf("latitude========================>> %f \n",obstacle->latitude());
						printf("altitude========================>> %f \n",obstacle->altitude());
						printf("speed===========================>> %f \n",obstacle->speed());
						printf("heading=========================>> %f \n",obstacle->heading());
						printf("verspeed========================>> %f \n",obstacle->verspeed());
						printf("length==========================>> %f \n",obstacle->length());
						printf("width===========================>> %f \n",obstacle->width());
						printf("height==========================>> %f \n",obstacle->height());
						printf("acc4way_lon=====================>> %f \n",obstacle->acc4way_lon());
						printf("acc4way_lat=====================>> %f \n",obstacle->acc4way_lat());
						printf("acc4way_vert====================>> %f \n",obstacle->acc4way_vert());
						printf("acc4way_yaw=====================>> %f \n",obstacle->acc4way_yaw());
						print_fun->show_time(obstacle->time());
						printf("tracking_time===================>> %d \n",obstacle->tracking_time());
						printf("point site size=================>> %d \n",obstacle->polygons_size());
						for(int ix = 0; ix < obstacle->polygons_size(); ix++)
						{
							nebulalink_pro_message::PointSiteInfo* point = obstacle->mutable_polygons(ix);
							printf("point site num=============>> %d <<===========\n",ix);
							print_fun->show_point_site(point);
							printf("point site num=============>> %d end<<========\n",ix);		
						}
						printf("obstacles num------------------>> %d end<<---------\n",i);
					}

					printf("Rte size-------------------->> %d \n",ssm.detect_rte_size());
					for(int i = 0; i < ssm.detect_rte_size(); i++)
					{
						nebulalink_pro_message::RteData* rteData = ssm.mutable_detect_rte(i);
						printf("Rte num-------------->> %d <<-----------\n",i);
						printf("rte_id==========================>> %d \n",rteData->rte_id());
						printf("rte_type========================>> %d \n",rteData->rte_type());
						printf("rte_source======================>> %d \n",rteData->rte_source());
						printf("rte_longitude===================>> %f \n",rteData->rte_longitude());
						printf("rte_latitude====================>> %f \n",rteData->rte_latitude());
						printf("rte_altitude====================>> %f \n",rteData->rte_altitude());
						printf("rte_radius======================>> %f \n",rteData->rte_radius());
						printf("rte_description=================>> %s \n",rteData->rte_description().c_str());
						printf("rts_start_");
						print_fun->show_time(rteData->rte_start_time());
						printf("rts_end_");
						print_fun->show_time(rteData->rte_end_time());
						printf("rte_priority====================>> %d \n",rteData->rte_priority());
						printf("rsu_longitude===================>> %f \n",rteData->rsu_longitude());
						printf("rsu_latitude====================>> %f \n",rteData->rsu_latitude());
						printf("rsu_altitude====================>> %f \n",rteData->rsu_altitude());
						printf("rsu_");
						print_fun->show_time(rteData->rsu_time());
						printf("rsu_id==========================>> %s \n",rteData->rsu_id().c_str());
						printf("Path size=============>> %d \n",rteData->paths_size());
						for(int pathsize = 0; pathsize < rteData->paths_size(); pathsize++)
						{
							nebulalink_pro_message::RelatePathInfo* path = rteData->mutable_paths(pathsize);
							printf("Path num==============>> %d <<===========\n",pathsize);
							print_fun->show_relate_path(path);
							printf("Path num==============>> %d end<<========\n",pathsize);
						}
						
						printf("Link size=============>> %d \n",rteData->links_size());
						for(int linksize = 0; linksize < rteData->links_size(); linksize++)
						{
							nebulalink_pro_message::RelateLinkInfo* link = rteData->mutable_links(linksize);
							printf("Link num==============>> %d <<===========\n",linksize);
							print_fun->show_relate_link(link);
							printf("Link num==============>> %d end<<========\n",linksize);
						}
						
						printf("Rte num------------------->> %d end<<---------\n",i);
					}
					
					printf("@@@@@@@@@@@@@@@@@@@@@@ SSM END @@@@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;

				case 0x0203://路侧协调消息(RSC)
				{
					
					if(!is_show203)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ RSC @@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::RscInfo rsc;
					rsc.ParseFromArray(data_buf,data_len);

					printf("source_id--------------------->> %s \n",rsc.source_id().c_str());
					print_fun->show_time(rsc.time());
					printf("rsc_longitude----------------->> %f \n",rsc.rsc_longitude());
					printf("rsc_latitude------------------>> %f \n",rsc.rsc_latitude());
					printf("rsc_altitude------------------>> %f \n",rsc.rsc_altitude());
					printf("veh coordination size--------->> %d \n",rsc.veh_coordination_size());
					for(int i = 0; i < rsc.veh_coordination_size(); i++)
					{
						nebulalink_pro_message::VehicleCoordination* vehcoor = rsc.mutable_veh_coordination(i);
						printf("veh coordination num----------->> %d <<------------\n",i);
						printf("device_id=======================>> %s \n",vehcoor->device_id().c_str());

						if(vehcoor->has_suggest())
						{
							nebulalink_pro_message::DriveSuggestion* drivesug = vehcoor->mutable_suggest();
							printf("******************DriveSuggestion************** \n");
							printf("behavior************************>> %d \n",drivesug->behavior());
							print_fun->show_time(drivesug->time());
							nebulalink_pro_message::RelatePathInfo* prlatepath = drivesug->mutable_path();
							print_fun->show_relate_path(prlatepath);
							nebulalink_pro_message::RelateLinkInfo* prlatelink = drivesug->mutable_link();
							print_fun->show_relate_link(prlatelink);

							printf("****************DriveSuggestion End************ \n");
						}
						printf("Path Point size===============>> %d \n",vehcoor->points_size());
						for(int ix = 0; ix < vehcoor->points_size(); ix++)
						{
							nebulalink_pro_message::PathPointInfo* pathpoint = vehcoor->mutable_points(ix);
							printf("Path Point num----------------->> %d <<------------\n",ix);
							print_fun->show_path_point(pathpoint);
							printf("Path Point num----------------->> %d end<<---------\n",ix);
						}
						printf("info============================>> %d \n",vehcoor->info());
						printf("veh coordination num----------->> %d end<<---------\n",i);
					}

					printf("lane coordination size-------->> %d \n",rsc.lane_coordination_size());
					for(int i = 0; i < rsc.lane_coordination_size(); i++)
					{
						nebulalink_pro_message::LaneCoordination* lanecoor = rsc.mutable_lane_coordination(i);
						printf("lane coordination num---------->> %d <<------------\n",i);
						nebulalink_pro_message::RelatePathInfo* prlatepath = lanecoor->mutable_path();
						print_fun->show_relate_path(prlatepath);
						nebulalink_pro_message::RelateLinkInfo* prlatelink = lanecoor->mutable_link();
						print_fun->show_relate_link(prlatelink);
						printf("start_");
						print_fun->show_time(lanecoor->start_time());
						printf("end_");
						print_fun->show_time(lanecoor->end_time());
						printf("speed***************************>> %f \n",lanecoor->speed());
						printf("behavior************************>> %d \n",lanecoor->behavior());
						printf("info****************************>> %d \n",lanecoor->info());
						printf("description*********************>> %s \n",lanecoor->description().c_str());

						printf("lane coordination num---------->> %d end<<---------\n", i);
					}
										
					printf("@@@@@@@@@@@@@@@@@@@@@@ RSC END @@@@@@@@@@@@@@@@@@@@@@@@\n");
				}
                break;
				case 0x0204: // 二阶段结果集(Day2 Result)
				{
					if(!is_show204)break;
					printf("@@@@@@@@@@@@@@@@@@@@@@ DAYII RESULT @@@@@@@@@@@@@@@@@@@\n");
					nebulalink_pro_message::CoordinationResult result;
					result.ParseFromArray(data_buf,data_len);
					if(result.has_lane_change())
					{
						printf("-------------------Has Lane Change------------------ \n");
						nebulalink_pro_message::LaneChangeResult* lanechange = result.mutable_lane_change();
						printf("role-------------------------->> %d \n",lanechange->role());
						if(lanechange->has_target_lane())
						{
							printf("===================Has Target Lane================== \n");
							nebulalink_pro_message::TargetLaneInfo* targetlane = lanechange->mutable_target_lane();
							print_fun->show_target_lane(targetlane);
						}
						
						if(lanechange->has_suggestion())
						{
							printf("===================Has Drive Suggestion============= \n");
							nebulalink_pro_message::CoordinationDriveSuggestion* suggestion = lanechange->mutable_suggestion();
							print_fun->show_drive_suggestion(suggestion);
						}

						if(0 < lanechange->veh_list_size())
						{
							printf("Veh list size==============>> %d \n",lanechange->veh_list_size());
							for(int i = 0; i < lanechange->veh_list_size(); i++)
							{
								nebulalink_pro_message::TargetVehicle* vehicle = lanechange->mutable_veh_list(i);
								printf("Veh list num=================>> %d <<===========\n",i);
								print_fun->show_target_veh(vehicle);
								printf("Veh list num=================>> %d end<<========\n",i);
							}
						}
					}
					
					if(result.has_clear_the_way())
					{
						printf("-------------------Has Clear The Way---------------- \n");
						nebulalink_pro_message::ClearTheWayResult* clearway = result.mutable_clear_the_way();
						printf("role-------------------------->> %d \n",clearway->role());
						
						if(clearway->has_target_lane())
						{
							printf("===================Has Target Lane================== \n");
							nebulalink_pro_message::TargetLaneInfo* targetlane = clearway->mutable_target_lane();
							print_fun->show_target_lane(targetlane);
						}
						
						if(clearway->has_suggestion())
						{
							printf("===================Has Drive Suggestion============= \n");
							nebulalink_pro_message::CoordinationDriveSuggestion* suggestion = clearway->mutable_suggestion();
							print_fun->show_drive_suggestion(suggestion);
						}
					}
					
					if(result.has_sensor_sharing())
					{
						printf("-------------------Has Sensor Sharing--------------- \n");
						nebulalink_pro_message::SensorSharingResult* sensorshare = result.mutable_sensor_sharing();
						printf("role-------------------------->> %d \n",sensorshare->role());
						if(0 < sensorshare->veh_list_size())
						{
							printf("Veh list size==============>> %d \n",sensorshare->veh_list_size());
							for(int i = 0; i < sensorshare->veh_list_size(); i++)
							{
								nebulalink_pro_message::TargetVehicle* vehicle = sensorshare->mutable_veh_list(i);
								printf("Veh list num=================>> %d <<===========\n",i);
								print_fun->show_target_veh(vehicle);
								printf("Veh list num=================>> %d end<<========\n",i);
							}
						}
					}
					
					if(result.has_intersection_pass())
					{
						printf("-------------------Has Intersection Pass------------ \n");
						nebulalink_pro_message::InterSectionPassResult* intersectionpass = result.mutable_intersection_pass();
						printf("type-------------------------->> %d \n",intersectionpass->type());
						if(intersectionpass->has_target_lane())
						{
							printf("===================Has Target Lane================== \n");
							nebulalink_pro_message::TargetLaneInfo* targetlane = intersectionpass->mutable_target_lane();
							print_fun->show_target_lane(targetlane);
						}
						
						if(intersectionpass->has_suggestion())
						{
							printf("===================Has Drive Suggestion============= \n");
							nebulalink_pro_message::CoordinationDriveSuggestion* suggestion = intersectionpass->mutable_suggestion();
							print_fun->show_drive_suggestion(suggestion);
						}
						
						if(0 < intersectionpass->current_lane_size())
						{
							printf("current lane size==============>> %d \n",intersectionpass->current_lane_size());
							for(int i = 0; i < intersectionpass->current_lane_size(); i++)
							{
								nebulalink_pro_message::TargetLaneInfo* targetlane = intersectionpass->mutable_current_lane(i);
								printf("current lane num=================>> %d <<===========\n",i);
								print_fun->show_target_lane(targetlane);
								printf("current lane num=================>> %d end<<========\n",i);
							}
						}
					}
					
					if(result.has_ramp_in())
					{
						printf("-----------------------Has Ramp In------------------ \n");
						nebulalink_pro_message::RampInResult* rampin = result.mutable_ramp_in();
						printf("role-------------------------->> %d \n",rampin->role());
						if(rampin->has_suggestion())
						{
							printf("===================Has Drive Suggestion============= \n");
							nebulalink_pro_message::CoordinationDriveSuggestion* suggestion = rampin->mutable_suggestion();
							print_fun->show_drive_suggestion(suggestion);
						}
					}
					printf("@@@@@@@@@@@@@@@@@@@@@@ DAYII RESULT END @@@@@@@@@@@@@@@\n");
				}
                break;

                default:
                    break;
            }

        }
    }

    return;
}



