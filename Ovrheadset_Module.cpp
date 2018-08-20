#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "Ovrheadset_Module.h"
#include <yarp/dev/IJoypadController.h>

#define DEBUG 0
//If debug is set to true , you"ll see the cameras as OpenCV images 



bool MyModule::configure(yarp::os::ResourceFinder &rf)
{
	string moduleName = rf.find("name").asString();
	string robotName = rf.find("robot").asString();
	Network yarp; 
	inputPortL = "/" + robotName + "/cam/left"; // icub or icubsim
	inputPortR = "/" + robotName + "/cam/right";
	
	
	if (DEBUG)
	{
		cvNamedWindow("rightview", 1);
		cvNamedWindow("leftview", 1);
		
	}
	else
	{
		Property options;
		options.fromConfigFile("ovr.ini");
		if (!headset.open(options))
		{
			printf("ovrheadset not ready\n");
			return false;
		}
		// headset.view(joyPad);
		// unsigned int n;
		// joyPad->getButtonCount(n);
		// std::cout << "HELEEEEEEEEEEEELOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << n << std::endl;
	}
		//------------------Opening Cameras driver----------------------//

	Property driverOptL;
	driverOptL.put("device", "remote_grabber");
	driverOptL.put("remote", inputPortL);
	driverOptL.put("local", "/oculus/display/left:i");
	if (!camL.open(driverOptL))
	{
		LOG("Can't connect to LEFT cameras");
		return false;
	}
	if (!camL.view(IframL))
	{
		LOG("unable to dynamic cast remote_grabber to IFrameGrabber interface ");
		return false;
	}

	Property driverOptR;
	driverOptR.put("device", "remote_grabber");
	driverOptR.put("remote", inputPortR);
	driverOptR.put("local", "/oculus/display/right:i");
	if (!camR.open(driverOptR))
	{
		LOG("Can't connect to RIGHT cameras");
		return false;
	}
	if (!camR.view(IframR))
	{
		LOG("unable to dynamic cast remote_grabber to IFrameGrabber interface ");
		return false;
	}

	
	return true;
}

bool MyModule::interruptModule()
{
	return true;
}
bool MyModule::close()
{	
	if (!DEBUG)
	headset.close();
	camL.close();
	camR.close();
	
	delete IframR;
	delete IframL;
	return true;
}

//-------Called periodically every getPeriod() seconds----------//

bool MyModule::updateModule()
{

	if (DEBUG)
	{
		ImageOf<PixelRgb> imgL;
		if (IframL == nullptr)
		{
			printf("NULLPTR_ERROR");
			return false;
		}
		if (IframL->getImage(imgL))
		{

			std::cout << " imgsize is: width  " << imgL.width() << " height : " << imgL.height() << std::endl;
			IplImage *cvImage = cvCreateImage(cvSize(imgL.width(),
				imgL.height()),
				IPL_DEPTH_8U, 3);
			cvCvtColor((IplImage*)imgL.getIplImage(), cvImage, CV_RGB2BGR);

			cvShowImage("leftview", cvImage);
			cvWaitKey(5);
		}
		ImageOf<PixelRgb> imgR;
		if (IframR == nullptr)
		{
			printf("NULLPTR_ERROR");
			return false;
		}
		if (IframR->getImage(imgR))
		{

			std::cout << " imgsize is: width  " << imgR.width() << " height : " << imgR.height() << std::endl;
			IplImage *cvImage = cvCreateImage(cvSize(imgR.width(),
				imgR.height()),
				IPL_DEPTH_8U, 3);
			cvCvtColor((IplImage*)imgR.getIplImage(), cvImage, CV_RGB2BGR);

			cvShowImage("rightview", cvImage);
			cvWaitKey(5);
		}
	}
	return true;
}

double MyModule::getPeriod()
{
	return 0.05; // a smaller value  may causes issues since openCV need some time to render properly
}


bool MyModule::respond(const Bottle& command, Bottle& reply)
{
	return true;
}



int main(int argc, char * argv[])
{

	Network yarp;

	if (!yarp.checkNetwork())

	{

		printf("YARP server not available!\n");

		return 1;

	}
	
	ResourceFinder rf;
	rf.setVerbose(true);
	rf.setDefault("name", "Grabber");
	rf.setDefault("robot", "icubSim");
	//rf.setDefault("robot", "iCubNancy01");
	rf.setDefaultConfigFile("myModule.ini");
	rf.setDefaultContext("myModule/conf");
	rf.configure(argc, argv);

	MyModule myModule;
	return myModule.runModule(rf);
}
