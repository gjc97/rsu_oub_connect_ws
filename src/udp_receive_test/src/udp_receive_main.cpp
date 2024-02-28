#include "obu_connect/obu_client.h"
#include "udp_receive_main.h"
#include "udp_receive_from_rsu_server/udp_receive_test.h"

	   
int main(int argc, char **argv)
{
	ros::init(argc, argv, "udp_receive");
	ros::NodeHandle nh("~");
	ros::Rate loop_rate(10);

	


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






