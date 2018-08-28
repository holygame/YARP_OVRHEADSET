# Installing the dependencies on Windows

**Note : all the following runtime libraries have to be built as Multi-threaded DLL (/MD) for release build and Multi-threaded
Debug DLL (/MDd) for debug [example](https://imgur.com/Rw1K7TT) .If you ever run into linker problemes , this is very likely the cause**


 

## 1. GLFW3

 * Download [GLFW3](http://www.glfw.org)

 * to create the solution file you can either use CMAKE-gui  or use cmake commands , for the later :

 * open terminal and type in :
	
		 cd <path to GLFW3_DIR>  
		 mkdir build  
		 cd build  
		 cmake -G "Visual Studio 15 2017 Win64" ..  
  
 * launch the .sln created in the build folder and build the libraries

 * add the path to GLFW3.dll to your PATH env variable ( if ovrheadset device isn't ready when typing yarpdev --list , this is very likely the cause )
 
## 2. GLEW

* Download [GLEW](http://glew.sourceforge.net/), make sure to take the source version.
 
* Go to  "GLEW_DIR"\build\vc12 and launch glew.sln to build the libraries ( make sure you have administrator rights)

* add the path to GLEW.dll to your PATH env variable ( if ovrheadset device isn't ready when typing yarpdev --list , this is very likely the cause )

## 3. OculusSDK

* Dowload Oculus SDK version 1.16 because higher version are not supported yet.

* Go to <Oculus_DIR>\Samples\Projects\Windows\VS2015 and open Samples.sln 

* build the debug and release version for both LibOVR and LibOVRKernel 


## 4. Yarp

 * You need YARP version 2.3.72 for this project to work - download the Win64 version of yarp[Yarp 2.3.72](https://github.com/robotology/yarp/releases/download/v2.3.72/yarp_2.3.72_v14_x86_amd64_1.exe)
 
 * Follow the installation from sources guide at http://yarp.it/install_yarp_windows.html
 
 * This flags should be enabled :

		YARP_USE_EIGEN3
		YARP_USE_QT5
		YARP_USE_GLEW 
		YARP_USE_GLFW3
		YARP_USE_LIBOVR
		CREATE_DEVICE_LIBRARY_MODULES
		ENABLE_yarpmod_ovrheadset
		CREATE_OPTIONAL_CARRIERS	
		ENABLE_yarpcar_bayer_carrier

**CMake may not find the libraries installed depending on where you've installed them , if so, enable the advanced options on the 
cmake-gui and set their parametres manually**

**for example:** 

**set GLFW3_OPENGL_LIBRARY to C:/Program Files (x86)/Microsoft SDKs/Windows/YOUR VERSION/Lib/x64/OpenGL32.Lib)**

**or update the FindLibName.cmake accordingly**
 
## 5. Icub


 * Follow the instruction to Install IcubSim 1.10.0 from [Icub Wiki](http://wiki.icub.org/wiki/Windows:_installation_from_sources)

 * make sure the flag ICUB_USE_IPOPT is set to ON
 
# OculusInAction

Streaming images from icub cameras to the oculus rift and getting joystick data from the oculus joysticks:

   * place the ovr.ini in the same repertory as your release file (<source_dir>/build/release) 
   * launch Yarpserver
   * launch icub_sim
   * launch frameTransform device : yarpdev --device transformServer --ROS::enable_ros_publisher 1 
   --ROS::enable_ros_subscriber 1 --transforms_lifetime 0.5
   * launch Application
	

## ports opened by the ovrheadset driver:

Port /oculus/headpose/orientation:o 

Port /oculus/headpose/position:o 

Port /oculus/headpose/angularVelocity:o 

Port /oculus/headpose/linearVelocity:o 

Port /oculus/headpose/angularAcceleration:o 

Port /oculus/headpose/linearAcceleration:o 

Port /oculus/predicted/headpose/predictedOrientation:o 

Port /oculus/predicted/headpose/predictedPosition:o

Port /oculus/predicted/headpose/predictedAngularVelocity:o 

Port /oculus/predicted/headpose/predictedLinearVelocity:o 

Port /oculus/predicted/headpose/predictedAngularAcceleration:o 

Port /oculus/predicted/headpose/predictedLinearAcceleration:o

Port /oculus/display/left:i

Port /oculus/display/right:i 

 * the orientation and predicted orientation data is streamed in degree as ( pitch , roll , yaw )

		pitch increases up , roll increase on the right, yaw increases on the right of the oculus operator 

 * the angular and predicted velocity and acceleration data is streamed in degree as (pitch, yaw , roll)

		pitch increase up , yaw  increases to the right , roll increases to the right of the oculus operator 

 * the position data is streamed as ( y , z , x) :

		y increases to the right , z increases upward, x backward of the oculus operator 
 
 * the velocity , the acceleration and the predicted velocity and acceleration data are stramed as (y , z , x )

		y increases to the right , z increases upward, x backward of the oculus operator 
 
more information about the predicted values : [Oculus](https://developer.oculus.com/blog/the-latent-power-of-prediction/)

## ports opened by OculusInAction :

Port /oculus/joysticks/right/stick:o

Port /oculus/joysticks/left/stick:o

Port /oculus/joysticks/right:o

Port /oculus/joysticks/left:o

 * the buttons are either fully pressed (1) or not pressed (0) 
 * the triggers can takes different values from 0 (not pressed) to 1 ( fully pressed )
 * the stick positon on the x and y axes goes from -1 to 1 
 
the Button are streamed in that order
 
 * For the right joystick : A , B , stick , right forward trigger , right grip trigger
 * For the left joystick : X , Y , stick , left forward trigger ,  left grip trigger
 * the sticks position is tracked using caretesian coordinates x , y


TODO :

* Use Rate_thread class in combination with the RFmodule class