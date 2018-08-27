#pragma once
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/dev/IJoypadController.h>
#include <string>

#define LOG(x) std::cout << x << std::endl;
using std::string;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;





float outFloat = .0f;
double outDouble = 0;
Vector leftVector;
Vector rightVector;
BufferedPort<Bottle> leftport;
BufferedPort<Bottle> rightport;
BufferedPort<Bottle> leftStickPort;
BufferedPort<Bottle> rightStickPort;
IJoypadController *joyPad;
	

//Function to write the button pressed on a Bottle
void AddButton(Bottle& b, int n, float out);
//Function to write the trigger pressur  on a Bottle
void AddAxis(Bottle& b, int n, double out);
//Function to write the position of the axis on a Bottle
//CARTESIAN coordinates are used by default but can be changed to POLAR
void AddStick(Bottle& b, int n, Vector& out, IJoypadController::JoypadCtrl_coordinateMode type = IJoypadController::JypCtrlcoord_CARTESIAN);