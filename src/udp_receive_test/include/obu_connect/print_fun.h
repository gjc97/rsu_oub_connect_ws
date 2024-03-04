#ifndef PRINT_FUN_H_
#define PRINT_FUN_H_
#include <nebulalink.serverdayii.pb.h>





class PRINT_FUN
{
public:
PRINT_FUN();
void show_acc4way_Info(nebulalink_pro_message::Acc4wayInfo* acc);


void show_time(int64_t time);

void show_gnss_info(nebulalink_pro_message::GnssInfo* gnssinfo);


void show_veh_info(nebulalink_pro_message::VehicleInfo* vehinfo);


void show_can_info(nebulalink_pro_message::CanInfo* caninfo);


void show_PointPos_Info(nebulalink_pro_message::PointPosInfo* point);


void show_point_site(nebulalink_pro_message::PointSiteInfo* point);

void show_path_point(nebulalink_pro_message::PathPointInfo* point);

void show_relate_path(nebulalink_pro_message::RelatePathInfo* path);


void show_relate_link(nebulalink_pro_message::RelateLinkInfo* link);


void show_target_lane(nebulalink_pro_message::TargetLaneInfo* target);


void show_drive_suggestion(nebulalink_pro_message::CoordinationDriveSuggestion* suggestion);


void show_target_veh(nebulalink_pro_message::TargetVehicle* vehicle);


void show_predic_info(nebulalink_pro_message::PathPrediction* prediction);



};

#endif
