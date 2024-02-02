



#include "udp_receive_main.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "udp_receive");
	ros::NodeHandle nh("~");


	UDP_RECEIVE *udp_receive = new UDP_RECEIVE();

	ros::Rate loop_rate(1);
	while (ros::ok())
	{
		udp_receive->recvData();
		loop_rate.sleep();
	}


	return 0;
}