project ( tesseract-gui )
 
cmake_minimum_required(VERSION 2.4.0)

add_definitions ( -Wall )
add_definitions ( -g -ggdb )

find_package(Qt4 REQUIRED)
include ( ${QT_USE_FILE} )
message(STATUS "includy: ${QT_USE_FILE}")

file(GLOB SRCS *.cpp)

file(GLOB RCS *.qrc)
qt4_add_resources ( RC_SRCS ${RCS} )

file(GLOB UIS *.ui)
qt4_wrap_ui ( UI_HDRS ${UIS} )

file(GLOB MOC_HDRS *.h)
qt4_wrap_cpp ( MOC_SRCS ${MOC_HDRS} )
 
add_executable(tesseract-gui ${SRCS} ${RC_SRCS} ${UI_HDRS} ${MOC_SRCS})
target_link_libraries(tesseract-gui ${QT_LIBRARIES})
