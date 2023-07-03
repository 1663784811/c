cmake_minimum_required(VERSION 3.20)
project(test01)

###############################
set(CMAKE_CXX_STANDARD 14)
set(OpenCV_DIR "D:\\c\\opencv\\mingw_build")


###############################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})



###############################
add_executable(
        test01
        main.cpp
        quickDemo.cpp quickDemo.h
        window/LogWindow.cpp window/LogWindow.h
)



###############################
target_link_libraries(test01 ${OpenCV_LIBS})







