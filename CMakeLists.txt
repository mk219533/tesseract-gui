project ( tesseract-gui )
 
cmake_minimum_required(VERSION 2.6.0)

add_definitions ( -Wall )
add_definitions ( -g -ggdb )

find_package(Qt4 REQUIRED)
include ( ${QT_USE_FILE} )
 
add_subdirectory(src)
