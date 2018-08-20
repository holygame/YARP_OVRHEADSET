#include <iostream>
#include <yarp/dev/IJoypadController.h>
#include <thread>
#include "JoySticksStream.h"


using namespace std::literals::chrono_literals;



int main(int argc, char * argv[])
{
	Property options;
	options.fromConfigFile("ovr.ini");
	if (!headset.open(options))
	{
		LOG("ovrheadset not ready\n");
		return false;
	}
	if (!headset.view(joyPad))
	{
		LOG("cant acces joysticks");
		return false;
	}
	
	Network yarp;
	leftport.open("/oculus/joysticks/left:o");
	rightport.open("/oculus/joysticks/right:o");
	leftStickPort.open("/oculus/joysticks/left/stick:o");
	rightStickPort.open("/oculus/joysticks/right/stick:o");
	
	while (true)
	{
		std::this_thread::sleep_for(10ms);
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
		// ADD triggers //
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
	}
	
}
void AddAxis(Bottle& b, int n, double out )
{
	joyPad->getAxis(n, out);
	b.addDouble(out);
}
void AddButton(Bottle& b, int n, float out)
{
	joyPad->getButton(n, out);
	b.addDouble(out);
}

void AddStick(Bottle& b, int n, Vector& out ,IJoypadController::JoypadCtrl_coordinateMode type)
{
	joyPad->getStick(n, out, type);
	for (int i = 0; i < out.length(); i++)
	{
		b.addDouble(out[i]);
	}
}
