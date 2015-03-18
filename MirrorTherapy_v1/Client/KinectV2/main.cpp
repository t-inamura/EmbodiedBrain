﻿//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "KinectApp.h"
//consider to use  "CComPtr" if you use Visual Studio Professional.
//create instance of "SIGKINECT_Linkage"
SIGKINECT_Linkage* srv;

std::string agent_name;

/*
 *main loop
 */
void mainLoop(std::string saddr, KinectApp kinectapp, SIGKINECT_Linkage* srv)
{
	bool m_start;
	m_start = true;
	static clock_t start;
	clock_t now;
	start = clock();
	bool m_onLoop;
	m_onLoop = true;
	double ltime;
	ltime = -1.0;
	while (true) {
		kinectapp.update();
		kinectapp.draw();
		auto key = cv::waitKey(10);
		if (key == 'q'){
			printf("quit mainloop\n");
			kinectapp.br = false;
			break;
		}
		now = clock();
		kinectapp.sendMessage(srv, agent_name, now);
		Sleep(100); /* wait 0.1 seconds */
		if (m_onLoop){
			if (ltime > 0){
				now = clock();
				double tmp = (double)(now - start) / CLOCKS_PER_SEC;
				if (tmp > ltime) {
					m_onLoop = false;
					break;
				}
			}
			if (!srv->checkRecvData(100)) {
				m_onLoop = false;
				break;
			}
		}
		else
		{
			break;
		}
	}
}

int main(int argc, char **argv)
{
	agent_name = "Linkage-Robot";
	std::string sname = "SIGKINECT_Linkage";
	TCHAR spd[64] = "";
	GetPrivateProfileString(_T("SETTING"), _T("SERVICE_NAME"), '\0', spd, 256, _T("./SIGNiUserTracker.ini"));
	printf("sname=%s\n",sname.c_str());
	printf("SERVICENAME=%s\n",sname.c_str());
	SIGKINECT_Linkage srv(sname);
	char saddr[128];
	int portnumber;

	if (argc == 1) {
		std::cout << "Cannot find Server host name and port number.\n";
		std::cout << "Enter SIGVerse server host name (or IP address)\n>>";
		std::cin >> saddr;
		std::cout << "Enter port number\n>>";
		std::cin >> portnumber;
	}

	if (argc > 1) {
		sprintf_s(saddr, 128, "%s", argv[1]);
		portnumber = atoi(argv[2]);
		std::cout << "Connect to server [" << saddr << "]\n";
		std::cout << "Portnumber [" << portnumber << "]\n\n";
		std::cout << "[ESC]: Quit service.\n";
	}

	//connect to server
	if (srv.connect(saddr, portnumber)){
		printf("success to connect to SIGServer.\n");
	}else
	{
		printf("fail to connect to SIGServer.\n");
	}

	// connect to viewer
	if (srv.connectToViewer()){
		printf("success to connect to SIGViewer.\n");
		//automatically exit loop when the viewer disconnect
		srv.setAutoExitLoop(true);
	}
	else{
		printf("fail to connect to SIGViewer.\n");
	}

    try {
		KinectApp kinectapp;
		kinectapp.initialize();
		mainLoop(saddr, kinectapp, &srv);

    }
    catch ( std::exception& ex ){
        std::cout << ex.what() << std::endl;
    }
}
