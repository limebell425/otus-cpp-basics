cmake_minimum_required(VERSION 3.5)

project(containers)

add_executable(containers 
    main.cpp
    include/MyVector.h
    include/MyForwardList.h
    include/MyList.h
)
set_target_properties(containers PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

include_directories(include)
add_subdirectory(test)