#pragma once
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/IJoypadController.h>


#define LOG(x) std::cout << x << std::endl;
using std::string;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;



class OculusModule :public RFModule
{
	//CAMERAS
	string robotName;
	string inputPortL;
	string inputPortR;
	string localPortL;
	string localPortR;
	PolyDriver camL;
	PolyDriver camR;
	PolyDriver headset;
	ImageOf<PixelRgb> img;
	IFrameGrabberImage *IframR;
	IFrameGrabberImage *IframL;
	//JOYSTICKS
	float outFloat = .0f;
	double outDouble = 0;
	Vector leftVector;
	Vector rightVector;
	BufferedPort<Bottle> leftport;
	BufferedPort<Bottle> rightport;
	BufferedPort<Bottle> leftStickPort;
	BufferedPort<Bottle> rightStickPort;
	IJoypadController *joyPad;
	
	
public:

	bool configure(yarp::os::ResourceFinder &rf);
	bool interruptModule();                   
	bool close();                                
	bool respond(const Bottle& command, Bottle& reply);
	double getPeriod();
	bool updateModule();
	//JOYSTICKS
	//Function to write the button pressed on a Bottle
	void AddButton(Bottle& b, int n, float out);
	//Function to write the trigger pressur  on a Bottle
	void AddAxis(Bottle& b, int n, double out);
	//Function to write the position of the axis on a Bottle
	//CARTESIAN coordinates are used by default but can be changed to POLAR
	void AddStick(Bottle& b, int n, Vector& out, IJoypadController::JoypadCtrl_coordinateMode type = IJoypadController::JypCtrlcoord_CARTESIAN);
};
