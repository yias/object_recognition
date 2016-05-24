# object_recognition
This is the object recognition project for the Robotic Practicals course
The "Object_Recognition" program reads captures pictures from a camera and classifies the pictures according to the user's preferencies. 
It has been created by the Learning Algorithms and Systems Laboratory (LASA) of EPFL for the purposes of the "Robotic Practicals" course.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%       Copyright (c) 2016 Iason Batzianoulis LASA Lab, EPFL,   	%%%
%%%        CH-1015 Lausanne, Switzerland, http://lasa.epfl.ch 	        %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

The program is free for non-commercial academic use!! :D:D:D

It has been created in Microsoft Visual Studio 2013.
It has been tested on Microsoft Visual Studio 2012 and Qtcreator 3.0.1
running on Windows 7, Windows 8.1 and on Ubuntu 14.04


The program uses OpenCV and boost libraries. See below for instructions on how to install OpenCV and boost in Windows and Ubuntu


Please send your feedback or questions to:      iason.batzianoulis@epfl.ch


----------------------------------
  OpenCV installation in Windows
----------------------------------

Follow the steps bellow in order to install the opencv library to a windows machine (using Visual Studio):

1) download the precompiled version for windows from:
 
http://opencv.org/downloads.html

2) run the executable to extract the dowloaded file to a folder 
 let's say that the folder path is  C:\opencv\

3) open a command prompt as administrator and type 

	setx -m OPENCV_DIR C:\OpenCV\Build\x86\vc10     (suggested for Visual Studio 2010 - 32 bit Windows)
or	setx -m OPENCV_DIR C:\OpenCV\Build\x64\vc10     (suggested for Visual Studio 2010 - 64 bit Windows)

or	setx -m OPENCV_DIR C:\OpenCV\Build\x86\vc11     (suggested for Visual Studio 2012 - 32 bit Windows)
or	setx -m OPENCV_DIR C:\OpenCV\Build\x64\vc11     (suggested for Visual Studio 2012 - 64 bit Windows)

or	setx -m OPENCV_DIR C:\OpenCV\Build\x86\vc12     (suggested for Visual Studio 2013 - 32 bit Windows)
or	setx -m OPENCV_DIR C:\OpenCV\Build\x64\vc12     (suggested for Visual Studio 2013 - 64 bit Windows)

4) set up the visual studio

a) open Visual Studio and create an empty project by the name "opencv_test"

b) go to the proct properties by clicking PROJECT->opencv_test Properties

c) click on the Configuration Properties

d) on the field Configuration (top-left of the window) select Release

e) expand the Configuration Properties menu, after that expand the C/C++ menu and go to the General entry
   Under the Additional Include Directories add the path to your OpenCV include:
	$(OPENCV_DIR)\..\..\include

f) next go to the Linker->General and under the Additional Library Directories add the libs directory:
	$(OPENCV_DIR)\lib

g) then go to the Linker->Input and under the Additional Dependencies entry add:
	opencv_world310.lib (for the version 3.1 of later)
or	the libraries you want to use in the project. The names of the libraries are as follow:
	opencv_(The Name of the module)(The version Number of the library you use)d.lib

h) change the option on the Configuration field from Release to Debug and repeat the steps e,f and g. 
   *** spicification for the step g:
   If you will use the version 3.2 or later, instead of the library opencv_world310.lib add the library opencv_world310d.lib. 
   In earlier versions, add the libraries that have d to the end of their name, eg opencv_core249d.lib

i) click Apply to save the changes and close the window

j) to make sure that everythig are set up properly, create a new .cpp file in the Source folder and copy-paste the following simple code:


	#include <iostream>
	#include "opencv2\opencv.hpp"
	#include <conio.h>


	int main(){

		std::cout<<"OpenCV version: "<< CV_VERSION <<std::endl; 

		std::cout<<"\nPress a key to terminate the program"<<std::endl; 

		_getch();

		return 0;
	}


build and run the project. The program will print the version of OpenCV that you are using. 

if the program compiles and runs then yhou are ready to use OpenCV.

More instructions and specification for the OpenCV instalation visit the following websites:


http://docs.opencv.org/2.4/doc/tutorials/introduction/windows_install/windows_install.html#windows-installation
http://docs.opencv.org/2.4/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html#windows-visual-studio-how-to

you can also watch the following videos:

https://www.youtube.com/watch?v=EcFtefHEEII
https://www.youtube.com/watch?v=tHX3MLzwF6Q



----------------------------------
  OpenCV installation in Ubuntu
----------------------------------

$ chmod +x opencv.sh
$ ./opencv.sh


---------------------------------
  Boost Installation in Windows
---------------------------------


to install boost libraries do the steps bellow:

1. download the proper version of boost libs from here:

http://www.boost.org/users/download/

2. unpack it to a folder eg.

	C:\Boost\

3. open a command prompt as administrator and change your current directory to the Boost root directory eg.

	C:\Boost\boost_1_60_0

4. type the following two commands

	bootstrap
	.\b2 

or if you want static library output

	.\b2 link=static runtime-link=static 

have patience!!

In order to run it in Visual Studio do the following steps:

1. Go to Project->"Name_of_the_project" Properties->Configuration Properties->C++->General and in the field Additional Include Direcotries add the boost root directory eg.

		C:\Boost\boost_1_60_0

2. Go to Project->"Name_of_the_project" Properties->Configuration Properties->C++->Precompiled Headers, change Use Precompiled Header (/Yu) to Not Using Precompiled Headers


3. Go to Project->"Name_of_the_project" Properties->Configuration Properties->Linker->General and the in field Additional Include Direcotries add the folder stage\lib eg.

		C:\Boost\boost_1_60_0\stage\lib


Then you are ready to use the boost library


For more information about the installation procedure of the boost library visit the following webpage:

http://www.boost.org/doc/libs/1_60_0/more/getting_started/windows.html


--------------------------------
  Boost installation in Ubuntu
--------------------------------

Although most of the Linux distributions include pre-built Boost packages, if you want to be sure that you
have the boost libraries in your Ubuntu system, open a terminal and type

sudo apt-get install libboost-all-dev

Enjoy!



***problem: 	missing OperCV dll in runtime when using visual studio
***solustion:	add the directoty of the dll ($(OPENCV_DIR)\bin) to the path of the system variables
		right-click to the "Copmuter" or "This PC" and click Properties, then click on "Andanced System Settings", go to "Advanced" tab and 
		click on Enviorment Variables. In the System Variables in the field Path add $(OPENCV_DIR)\bin
