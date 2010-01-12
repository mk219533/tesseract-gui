LINUX

In order to build tesseract-gui on Linux, first you should install:
- c++ compiler (g++ for example)
- Qt4 development libraries (http://qt.nokia.com)
- cmake (http://www.cmake.org)

Installing cmake

You can install cmake either from binary packages you can find on project home page (http://www.cmake.org/cmake/resources/software.html)
or from packages prepared to your Linux distribution.
On Ubuntu/Debian: 
sudo apt-get install cmake
On Fedora:
sudo yum install cmake

Installing Qt4

Similarly to cmake, you can install Qt4 in two ways:
- Download source package from product home page and follow installation instructions.
- Install suitable package using your package manager.
On Ubuntu/Debian:
sudo apt-get install libqt4-dev
On Fedora:
sudo yum install qt-devel


Building tesseract-gui

Enter root directory of sources and invoke:
mkdir build
cd build
cmake ..
make

You can run tesseract-gui typing:
./tesseract-gui


WINDOWS

The same as on Linux, download and install:
- c++ compiler (you can use MinGW or Visual Studio for example)
- Qt4 libraries (for MinGW or Visual Studio)
- cmake

Ensure you have all of them added to you PATH environment variable.
Open command line and enter root directory of tesseract-gui project.

If you are using MinGW type:
mkdir build
cd build
cmake -G"MinGW Makefiles" ..
make

If you want to use Visual Studio you cen generate project files by invoking
cmake -G"<Suitable cmake generator>" 

To see all generators that cmake provide run cmake without any parameters

