/*
# Software License Agreement (MIT License)
#
# Copyright (c) 2019, UFACTORY, Inc.
# All rights reserved.
#
# Author: Vinman <vinman.wen@ufactory.cc> <vinman.cub@gmail.com>
*/

#include "xarm/wrapper/xarm_api.h"
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Please enter IP address\n");
		return 0;
	}
	std::string port(argv[1]);

	XArmAPI *arm = new XArmAPI(port);
	sleep_milliseconds(500);
	if (arm->error_code != 0) arm->clean_error();
	if (arm->warn_code != 0) arm->clean_warn();
	arm->motion_enable(true);
	arm->set_mode(0);
	arm->set_state(0);
	sleep_milliseconds(500);

	printf("=========================================\n");

	//#####READ FILE#####//
	std::fstream myfile("/home/guoqing/data.txt", std::ios_base::in);
	fp32 a, b, c, d, e, f;
   	myfile >> a >> b >> c;
   	printf("%f\t%f\t%f\t\n", a, b, c);

	// go to the first pose
	fp32 firstPose[6] = { 250, 0, 250, 180, 0, 0 };
	int ret;
	arm->reset(true);
	arm->set_position(firstPose,true);
	print_nvect("firstPose:",firstPose,6);
	// switch mode
	arm->set_mode(1);
	arm->set_state(0);
	sleep_milliseconds(100);
	// get current pose 
	fp32 desiredPose[6];
	fp32 currentPose[6];
	arm->get_position(currentPose);
	
	// while (arm->is_connected() && arm->state != 4)
	while(true)
	{
		std::fstream myfile("teleop_cmd.txt", std::ios_base::in);
    	float a, b, c;
    		myfile >> a >> b >> c;
		float velocity[3]={a,b,c};
		float dt=0.01;
		for (int i = 0; i <3; i++){
			desiredPose[i] = currentPose[i] + dt*velocity[i];
		}
		desiredPose[3] = 180;
		desiredPose[4] = 0;
		desiredPose[5] = 0;
		print_nvect("desiredPose:",desiredPose,6);
    		printf("%f\t%f\t%f\t\n", a, b, c);
		ret = arm->set_servo_cartesian(desiredPose);
		sleep_milliseconds(10);
		arm->get_position(currentPose);
	}
	return 0;
}
