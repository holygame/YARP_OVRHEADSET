
Project(Ovrheadset)

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

include_directories(D:/Documents/Visual Studio 2017/Projects/CMake/Nouveau dossier
		    D:/opencv-3.4.2/build/install/include
		    D:/glew-2.1.0/include
		    C:/Program\ Files/GLFW/include
		    D:/OculusSDK/LibOVR/Include
		  
		   )


find_package(YARP)



add_executable(display Ovrheadset_Module.cpp)

add_executable(joystick JoySticksStream.cpp)



target_link_libraries(display ${YARP_LIBRARIES} 
"D:/opencv-3.4.2/build/lib/Release/opencv_core342.lib"
"D:/opencv-3.4.2/build/lib/Release/opencv_highgui342.lib"
"D:/opencv-3.4.2/build/lib/Release/opencv_imgproc342.lib"
"D:/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2015/libOVR.lib"
)
target_link_libraries(joystick ${YARP_LIBRARIES} 
"D:/OculusSDK/LibOVR/Lib/Windows/x64/Release/VS2015/libOVR.lib"
)
