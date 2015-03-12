#include <iostream>
#include "KinectV2Device.h"
#include <boost\thread.hpp>

int main(int argc, char **argv) {
	//Device *device = &Device(argc, argv);
	KinectV2Device kinectV2Device(argc, argv);

	kinectV2Device.run();

	return 0;
}