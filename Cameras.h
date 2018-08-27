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

	


