#include "obu_connect/obu_client.h"
#include "udp_receive_main.h"
#include "udp_receive_from_rsu_server/udp_receive_test.h"
#include <ros/ros.h>
#include <ros/package.h>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "udp_receive");
	ros::NodeHandle nh("~");
	ros::Rate loop_rate(10);

	OBU_CLIENT* obu_client;

	std::string cwd = ros::package::getPath("udp_receive_test");
	std::string inifile_path = cwd + "/config/receive_config.ini";

	obu_client->SetConfig(inifile_path);

	if(obu_client->ClientInit()<0)
	{
		printf("ClientInit failed...\n");
		return -1;
	}
	
	// if(NewThread(CreateRecvThread,NULL) == -1)
	// {
	// 	printf("CreateRecvThread pthread_create failed...\n");
	// 	return -1;
	// }

	while(ros::ok())



	
 

	// if (RegisterFrame() < 0)
		// return 0;
	printf("Register Frame Send Suc!\r\n");

   int recv_len = 0;
	unsigned char recv_buf[C_MAX_PACK_SIZE] = {0};
	memset(recv_buf, 0, C_MAX_PACK_SIZE);
	recv_len = read(obu_client->socket_fd_,recv_buf, C_MAX_PACK_SIZE);
	if (recv_len <= 0)
	{
		printf("sockfd = %d\n",obu_client->socket_fd_);
		obu_client->RegisterFrame();
	}
	else
	{
		obu_client->handle(recv_buf, recv_len);
	}

		loop_rate.sleep();
		return 0;
}






