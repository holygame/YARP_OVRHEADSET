
Project(Ovrheadset) 


 


 CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

 include_directories(C:/Users/AnDy/Yarp_Projects/ovrHeadset)

 set(OpenCV_DIR C:/Users/AnDy/opencv-3.4.2/build)

 find_package(YARP)

 find_package(OpenCV REQUIRED)

 add_executable(ovr ICub_Cameras.cpp)

 target_link_libraries(ovr ${YARP_LIBRARIES} ${OpenCV_LIBS} )