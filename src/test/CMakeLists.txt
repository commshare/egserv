cmake_minimum_required(VERSION 2.6)
PROJECT(mytest)
SET(_ ${PROJECT_SOURCE_DIR})
AUX_SOURCE_DIRECTORY(${_} SRC_LIST)
#MESSAGE(STATUS "1....${SRC_LIST}")
#MESSAGE(STATUS "2....${_}")

#SET(LIBRARY_OUTPUT_PATH ${_})
SET(EXECUTABLE_OUTPUT_PATH  ${_})
if(APPLE)
    SET(PB_LIB_DIR ${_}/../base/pb/lib/mac)
    #list(REMOVE_ITEM SRC_LIST ${_}/playsound.c)
elseif(UNIX)
    SET(PB_LIB_DIR ${_}/../base/pb/lib/linux)
    list(REMOVE_ITEM SRC_LIST ${_}/playsound.mm)
endif()

ADD_DEFINITIONS( -g -W -Wall -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

INCLUDE_DIRECTORIES(${_} ${_}/../base ${_}/../base/pb/protocol ${_}/../base/jsoncpp/ ${_}/../base/pb/)
LINK_DIRECTORIES(${_} ${_}/../base ${PB_LIB_DIR} ${_}/../base/slog/lib)

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面，否则会报错
#ADD_LIBRARY(${PROJECTNAME} SHARED/STATIC ${SRC_LIST})
ADD_EXECUTABLE(mytest ${SRC_LIST})

TARGET_LINK_LIBRARIES(mytest base protobuf-lite slog pthread curl) 
