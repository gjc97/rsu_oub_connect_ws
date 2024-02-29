#include "obu_connect/obu_client.h"
#include "udp_receive_main.h"
#include "udp_receive_from_rsu_server/udp_receive_test.h"
#include <ros/ros.h>
#include <ros/package.h>


// int main(int argc, char **argv)
// {
// 	ros::init(argc, argv, "udp_receive");
// 	ros::NodeHandle nh("~");
// 	ros::Rate loop_rate(100);


// 	OBU_CLIENT *obu_client =  new OBU_CLIENT();
	
// 	obu_client->CreateThread();



// 	while(ros::ok())
// 		loop_rate.sleep();
// 		return 0;
// }



int main(int argc, char **argv)
{
	ros::init(argc, argv, "udp_receive");
	ros::NodeHandle nh("~");
	ros::Rate loop_rate(10);

	std::string cwd = ros::package::getPath("udp_receive_test");
	std::string inifile_path = cwd + "/config/receive_config.ini";

	SetConfig(inifile_path);

	if(ClientInit()<0)
	{
		printf("ClientInit failed...\n");
		return -1;
	}
	
	if(NewThread(CreateRecvThread,NULL) == -1)
	{
		printf("CreateRecvThread pthread_create failed...\n");
		return -1;
	}

	while(ros::ok())
		loop_rate.sleep();
		return 0;
}






