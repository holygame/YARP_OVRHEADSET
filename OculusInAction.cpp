#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <thread>
#include <iostream>
#include <yarp/dev/IJoypadController.h>
#include "OculusInAction.h"


#define DEBUG 0
//If debug is set to true , you"ll see the cameras as OpenCV images 

using namespace std::literals::chrono_literals;


 void OculusModule::AddAxis(Bottle& b, int n, double out)
 {
	 joyPad->getAxis(n, out);
	 b.addDouble(out);
 }
 void OculusModule::AddButton(Bottle& b, int n, float out)
 {
	 joyPad->getButton(n, out);
	 b.addDouble(out);
 }

 void OculusModule::AddStick(Bottle& b, int n, Vector& out, IJoypadController::JoypadCtrl_coordinateMode type)
 {
	 joyPad->getStick(n, out, type);
	 for (int i = 0; i < out.length(); i++)
	 {
		 b.addDouble(out[i]);
	 }
 }

bool OculusModule::configure(yarp::os::ResourceFinder &rf)
{
	leftport.open("/oculus/joysticks/left:o");
	rightport.open("/oculus/joysticks/right:o");
	leftStickPort.open("/oculus/joysticks/left/stick:o");
	rightStickPort.open("/oculus/joysticks/right/stick:o");
	/*leftport.open("/oculus/local/display/right:i");
	rightport.open("/oculus/local/display/left:i");
	Network::connect("/oculus/local/display/right:i", "/oculus/display/right:i");
	Network::connect("/oculus/local/display/left:i", "/oculus/display/left:i");*/
	
	string moduleName = rf.find("name").asString();
	string robotName = rf.find("robot").asString();
	inputPortL = "/" + robotName + "/cam/left"; 
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
			LOG("ovrheadset not ready");
			return false;
		}
		if (!headset.view(joyPad))
		{
			LOG("cant acces joysticks");
			return false;
		}
		

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
	
}
bool OculusModule::interruptModule()
{
	return true;
}
bool OculusModule::close()
{

	if (!DEBUG)
		headset.close();
	camL.close();
	camR.close();
	leftport.close();
	rightport.close();
	leftStickPort.close();
	rightStickPort.close();
	delete joyPad;
	delete IframR;
	delete IframL;
	return true;
}

bool OculusModule::respond(const Bottle& command, Bottle& reply)
{
	return true;
}

double OculusModule::getPeriod()
{
	return 0.05;
}

bool OculusModule::updateModule()
{
	//std::this_thread::sleep_for(100ms);
	Bottle& bLeft = leftport.prepare();
	Bottle& bRight = rightport.prepare();
	Bottle& bRightStick = rightStickPort.prepare();
	Bottle& bLeftStick = leftStickPort.prepare();
	bRight.clear();
	bLeft.clear();
	bLeftStick.clear();
	bRightStick.clear();

	// ADD BUTTON //
	for (int i = 0; i < 3; i++)
	{
		AddButton(bRight, i, outFloat);
	}
	for (int i = 4; i < 7; i++)
	{
		AddButton(bLeft, i, outFloat);
	}
	// ADD AXIS //
	AddAxis(bRight, 1, outDouble);
	AddAxis(bRight, 3, outDouble);
	AddAxis(bLeft, 0, outDouble);
	AddAxis(bLeft, 2, outDouble);
	// ADD STICKS //
	AddStick(bLeftStick, 0, leftVector);
	AddStick(bRightStick, 1, rightVector);
	leftStickPort.write();
	rightStickPort.write();
	leftport.write();
	rightport.write();
	if (DEBUG) // stream images as openCV images , 
            	//might be usefull to get higher resolution images and compressing 
            	//them before sending them to the oculus as to no saturate the network
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

int main(int argc, char** argv)
{
	Network yarp;
	if (!yarp.checkNetwork())

	{
		LOG("YARP server not available!");
		return false;
	}
	ResourceFinder rf;
	rf.setVerbose(true);
	rf.setDefault("name", "Grabber");
	rf.setDefault("robot", "icubSim");
	//rf.setDefault("robot", "iCubNancy01");
	rf.setDefaultConfigFile("oculusModule.ini");
	rf.setDefaultContext("oculusModule/conf");
	rf.configure(argc, argv);
	OculusModule oculusModule;
	return oculusModule.runModule(rf);
}
