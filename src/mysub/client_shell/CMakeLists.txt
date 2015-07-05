cmake_minimum_required(VERSION 2.6)
PROJECT(client_shell)
SET(_ ${PROJECT_SOURCE_DIR})
AUX_SOURCE_DIRECTORY(${_} SRC_LIST1)
AUX_SOURCE_DIRECTORY(${_}/../util SRC_LIST2)
AUX_SOURCE_DIRECTORY(${_}/../net/ SRC_LIST3)
list(REMOVE_ITEM SRC_LIST2 ${_}/../util/test.cpp)
#MESSAGE(STATUS "1....${SRC_LIST1}")
#MESSAGE(STATUS "2....${SRC_LIST2}")
#MESSAGE(STATUS "3....${SRC_LIST3}")

#SET(LIBRARY_OUTPUT_PATH ${_})
SET(EXECUTABLE_OUTPUT_PATH  ${_})
if(APPLE)
    SET(PB_LIB_DIR ${_}/../../base/pb/lib/mac)
    #list(REMOVE_ITEM SRC_LIST ${_}/playsound.c)
elseif(UNIX)
    SET(PB_LIB_DIR ${_}/../../base/pb/lib/linux)
    #list(REMOVE_ITEM SRC_LIST ${_}/playsound.mm)
endif()

ADD_DEFINITIONS( -g -Wall -Wno-deprecated -Wno-unused-function -Wno-unused-variable 
-fdiagnostics-color=always -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

#INCLUDE_DIRECTORIES(${_} ${_}/../base ${_}/../base/pb/protocol ${_}/../base/jsoncpp/ ${_}/../base/pb/)
#LINK_DIRECTORIES(${_} ${_}/../base ${PB_LIB_DIR} ${_}/../base/slog/lib)

INCLUDE_DIRECTORIES(${_} ${_}/../util/ ${_}/../net/ ${_}/../../base/pb/protocol/ ${_}/../../base/pb/)
LINK_DIRECTORIES(${_} ${_}/../lib ${PB_LIB_DIR})

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面，否则会报错
#ADD_LIBRARY(${PROJECTNAME} SHARED/STATIC ${SRC_LIST})
ADD_EXECUTABLE(client_shell ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3})

TARGET_LINK_LIBRARIES(client_shell protobuf-lite slog pthread curl ssl)
