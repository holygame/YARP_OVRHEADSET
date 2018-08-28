Project(Ovrheadset) 


 


 CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

 include_directories(C:/Users/AnDy/Yarp_Projects/ovrHeadset) # path to the headerfile directory

 set(OpenCV_DIR C:/Users/AnDy/opencv-3.4.2/build) #path the opencv build directory

 find_package(YARP)

 find_package(OpenCV REQUIRED)

 add_executable(OVR ICub_Cameras.cpp)

 target_link_libraries(OVR ${YARP_LIBRARIES} ${OpenCV_LIBS} )