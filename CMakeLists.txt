CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ejson)
SET(CMAKE_C_FLAGS "-O -Wall -g")
SET(INCLUDE_PATH ${PROJECT_SOURCE_DIR})
SET(SOURCE_PATH ${PROJECT_SOURCE_DIR})
set(TARGET "ejson")

 
AUX_SOURCE_DIRECTORY(${SOURCE_PATH} SRC_LIST)
ADD_LIBRARY(${TARGET} STATIC 
    ${SRC_LIST}
)

target_include_directories(${TARGET} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR} 
     ./
     /data/comm_code/install/include     
    /data/comm_code/install/include/jsoncpp/ 
    ${INCLUDE_PATH}
)

target_link_libraries(${TARGET} PUBLIC 
     jsoncpp
	 boost_thread
)
