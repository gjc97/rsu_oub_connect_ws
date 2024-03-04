#include <stdlib.h>

#include <iostream>
#include <unistd.h>


#include <stdlib.h>
#include <string>
#include <inttypes.h>

#include "obu_connect/print_fun.h"


PRINT_FUN::PRINT_FUN()
{
	
}


void PRINT_FUN::show_acc4way_Info(nebulalink_pro_message::Acc4wayInfo* acc)
{
	printf("*********************Acc4wayInfo********************\n");
	printf("acc4way_lon********************>>> %f \n",acc->acc4way_lon());
	printf("acc4way_lat********************>>> %f \n",acc->acc4way_lat());
	printf("acc4way_vert*******************>>> %f \n",acc->acc4way_vert());
	printf("acc4way_yaw********************>>> %f \n",acc->acc4way_yaw());
	printf("*******************Acc4wayInfo End******************\n");
}

void PRINT_FUN::show_time(int64_t time)
{
	int miilsecond = time%1000;
	int64_t second = time/1000;
	struct tm *info;
	info = localtime((time_t*)&second);
	printf("time***************************>>> %s",asctime(info));
	printf("millisecond********************>>> %d \n",miilsecond);
}

void PRINT_FUN::show_gnss_info(nebulalink_pro_message::GnssInfo* gnssinfo)
{
	printf("\n***********************GnssDesc*********************\n");
	printf("longitude**********************>>> %f \n",gnssinfo->longitude());
	printf("latitude***********************>>> %f \n",gnssinfo->latitude());
	printf("altitude***********************>>> %f \n",gnssinfo->altitude());
	printf("speed**************************>>> %f \n",gnssinfo->speed());
	show_time(gnssinfo->time());
	printf("heading************************>>> %f \n",gnssinfo->heading());
	printf("accel**************************>>> %f \n",gnssinfo->accel());
	printf("accel_angle********************>>> %f \n",gnssinfo->accel_angle());
	printf("satellite_num******************>>> %d \n",gnssinfo->satellite_num());
	printf("precision**********************>>> %f \n",gnssinfo->precision());
	nebulalink_pro_message::Acc4wayInfo* acc = gnssinfo->mutable_acc();
	show_acc4way_Info(acc);
	printf("*********************GnssDesc End*******************\n");
}

void PRINT_FUN::show_veh_info(nebulalink_pro_message::VehicleInfo* vehinfo)
{
	printf("*******************VehicleInfoDesc******************\n");
	printf("plate_num**********************>>> %s \n",vehinfo->plate_num().c_str());
	printf("vehicle_class******************>>> %d \n",vehinfo->vehicle_class());
	printf("fuel_type**********************>>> %d \n",vehinfo->fuel_type());
	printf("brand_and_type*****************>>> %s \n",vehinfo->brand_and_type().c_str());
	printf("length*************************>>> %f \n",vehinfo->length());
	printf("width**************************>>> %f \n",vehinfo->width());
	printf("height*************************>>> %f \n",vehinfo->height());
	printf("*****************VehicleInfoDesc End****************\n");
}

void PRINT_FUN::show_can_info(nebulalink_pro_message::CanInfo* caninfo)
{
	printf("***********************CanInfo**********************\n");
	printf("transmission_state_source******>>> %d \n",caninfo->transmission_state_source());
	printf("transmission_state*************>>> %d \n",caninfo->transmission_state());
	printf("speed_can_valid****************>>> %d \n",caninfo->speed_can_valid());
	printf("speed_can**********************>>> %f \n",caninfo->speed_can());
	printf("heading_can_speed**************>>> %d \n",caninfo->heading_can_speed());
	printf("heading_can********************>>> %f \n",caninfo->heading_can());
	nebulalink_pro_message::Acc4wayInfo* acc = caninfo->mutable_acc_can();
	show_acc4way_Info(acc);
	printf("steering_wheel_angle_valid*****>>> %d \n",caninfo->steering_wheel_angle_valid());
	printf("steering_wheel_angle***********>>> %f \n",caninfo->steering_wheel_angle());
	printf("brake_state_valid**************>>> %d \n",caninfo->brake_state_valid());
	printf("brake_pedal_state**************>>> %d \n",caninfo->brake_pedal_state());
	printf("brake_applied_state************>>> %d \n",caninfo->brake_applied_state());
	printf("traction_control_state*********>>> %d \n",caninfo->traction_control_state());
	printf("abs_state**********************>>> %d \n",caninfo->abs_state());
	printf("stability_control_state********>>> %d \n",caninfo->stability_control_state());
	printf("brake_boost_applied************>>> %d \n",caninfo->brake_boost_applied());
	printf("auxiliary_brake_state**********>>> %d \n",caninfo->auxiliary_brake_state());
	printf("traction_control_state*********>>> %d \n",caninfo->traction_control_state());
	printf("vehicle_event_flag_source******>>> %d \n",caninfo->vehicle_event_flag_source());
	printf("vehicle_event_flag*************>>> %d \n",caninfo->vehicle_event_flag());
	printf("vehicle_lights_state_source****>>> %d \n",caninfo->vehicle_lights_state_source());
	printf("vehicle_lights_state***********>>> %d \n",caninfo->vehicle_lights_state());
	printf("brake_pedal_depth_source*******>>> %d \n",caninfo->brake_pedal_depth_source());
	printf("brake_pedal_depth**************>>> %f \n",caninfo->brake_pedal_depth());
	printf("*********************CanInfo End********************\n");
}

void PRINT_FUN::show_PointPos_Info(nebulalink_pro_message::PointPosInfo* point)
{
	printf("***********************PointPos*********************\n");
	printf("longitude**********************>>> %f \n",point->point_longitude());
	printf("latitude***********************>>> %f \n",point->point_latitude());
	printf("altitude***********************>>> %f \n",point->point_altitude());
	printf("speed**************************>>> %f \n",point->point_speed());
	show_time(point->point_time());
	printf("heading************************>>> %f \n",point->point_heading());
	printf("*********************PointPos End*******************\n");
}

void  PRINT_FUN::show_point_site(nebulalink_pro_message::PointSiteInfo* point)
{
	printf("*************************PointSite*********************\n");
	printf("longitude**********************>>> %f \n",point->longitude());
	printf("latitude***********************>>> %f \n",point->latitude());
	printf("altitude***********************>>> %f \n",point->altitude());
	printf("***********************PointSite End*******************\n");
}

void  PRINT_FUN::show_path_point(nebulalink_pro_message::PathPointInfo* point)
{
	printf("*************************PathPoint*********************\n");
	show_time(point->time());
	printf("longitude**********************>>> %f \n",point->longitude());
	printf("longitude**********************>>> %f \n",point->latitude());
	printf("longitude**********************>>> %f \n",point->altitude());
	printf("speed**************************>>> %f \n",point->speed());
	printf("heading************************>>> %f \n",point->heading());
	printf("acc4way_lon********************>>> %f \n",point->acc4way_lon());
	printf("acc4way_lat********************>>> %f \n",point->acc4way_lat());
	printf("acc4way_vert*******************>>> %f \n",point->acc4way_vert());
	printf("acc4way_yaw********************>>> %f \n",point->acc4way_yaw());
	printf("distance***********************>>> %f \n",point->distance());
	printf("upper_node_id******************>>> %" PRIu64 " \n",point->upper_node_id());
	printf("down_node_id*******************>>> %" PRIu64 " \n",point->down_node_id());
	printf("relate_lane********************>>> %d \n",point->relate_lane());

	printf("***********************PathPoint End*******************\n");
}

void  PRINT_FUN::show_relate_path(nebulalink_pro_message::RelatePathInfo* path)
{
	printf("*************************RelatePath*********************\n");
	printf("radius*************************>>> %f \n",path->path_radius());
	printf("Path point size****************>>> %d \n",path->pathpoints_size());
	for(int i = 0; i < path->pathpoints_size(); i++)
	{
		nebulalink_pro_message::PointSiteInfo* point = path->mutable_pathpoints(i);
		printf("Path point num*************>> %d <<***********\n",i);
		show_point_site(point);
		printf("Path point num*************>> %d end<<********\n",i);		
	}
	printf("***********************RelatePath End*******************\n");
}

void  PRINT_FUN::show_relate_link(nebulalink_pro_message::RelateLinkInfo* link)
{
	printf("*************************RelateLink*********************\n");
	printf("up_node_id*********************>>> %" PRIu64 " \n",link->up_node_id());
	printf("down_node_id*******************>>> %" PRIu64 " \n",link->down_node_id());
	printf("Lane size*************>> %d \n",link->lanes_size());
	for(int lanesize = 0; lanesize < link->lanes_size(); lanesize++)
	{
		nebulalink_pro_message::RelateLinkInfo_RelateLane* lane = link->mutable_lanes(lanesize);
		printf("Lane num**************>> %d <<***********\n",lanesize);
		printf("lane_id &&&&&&&&&&&&&&&&&&&&&&&>>> %d \n",lane->lane_id());
		printf("Lane num**************>> %d end<<********\n",lanesize);
	}
	printf("***********************RelateLink End*******************\n");
}

void PRINT_FUN::show_target_lane(nebulalink_pro_message::TargetLaneInfo* target)
{
	printf("===================TargetLaneInfo==================\n");
	printf("upper_node_id===================>> %" PRIu64 " \n",target->upper_node_id());
	printf("down_node_id====================>> %" PRIu64 " \n",target->down_node_id());
	printf("lane_id=========================>> %d \n",target->lane_id());
	printf("time range size=================>> %d \n",target->time_range_size());
	for(int i = 0; i < target->time_range_size(); i++)
	{
		nebulalink_pro_message::TimeRange* range = target->mutable_time_range(i);
		printf("time range num******************>> %d <<************\n",i);
		printf("start_");
		show_time(range->start_time());
		printf("end_");
		show_time(range->end_time());
		printf("time range num******************>> %d end<<*********\n",i);		
	}	
	printf("===================TargetLaneInfo End==============\n");
}

void PRINT_FUN::show_drive_suggestion(nebulalink_pro_message::CoordinationDriveSuggestion* suggestion)
{
	printf("=============CoordinationDriveSuggestion===========\n");
	nebulalink_pro_message::SpeedLimitResult* speed_limit = suggestion->mutable_speed_guide();	
	printf("******************SpeedLimitResult************** \n");
	printf("ceiling*************************>> %f \n",speed_limit->ceiling());
	printf("floor***************************>> %f \n",speed_limit->floor());
	printf("****************SpeedLimitResult End************ \n");
	nebulalink_pro_message::DriveBehaviorInfo* behavior = suggestion->mutable_behavior_suggest();
	printf("******************DriveBehaviorInfo************** \n");
	printf("behavior************************>> %d \n",behavior->behavior());
	printf("****************DriveBehaviorInfo End************ \n");
	printf("time_line ");
	show_time(suggestion->time_line());
	printf("===========CoordinationDriveSuggestion End=========\n");
}

void PRINT_FUN::show_target_veh(nebulalink_pro_message::TargetVehicle* vehicle)
{
	printf("device_id***********************>> %s \n",vehicle->device_id().c_str());
	printf("relative_pos********************>> %d \n",vehicle->relative_pos());
	nebulalink_pro_message::DriveBehaviorInfo* behavior = vehicle->mutable_behavior_info();
	printf("******************DriveBehaviorInfo************** \n");
	printf("behavior************************>> %d \n",behavior->behavior());
	printf("****************DriveBehaviorInfo End************ \n");
}

void PRINT_FUN::show_predic_info(nebulalink_pro_message::PathPrediction* prediction)
{
	printf("***********************PredicInfo*********************\n");
	printf("radius*************************>>> %d \n",prediction->radius());
	printf("confidence*********************>>> %d \n",prediction->confidence());	
	printf("*********************PredicInfo End*******************\n");
}

