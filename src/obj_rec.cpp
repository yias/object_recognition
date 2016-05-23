//#include "ros/ros.h"
//#include "std_msgs/String.h"

#include <objrec.h>
//#include "preprocf.h"
//#include <boost/filesystem.hpp>
#include <iostream>


int main (int argc, char** argv){//()



    //boost::filesystem::path current_ph= boost::filesystem::current_path();
    //path = current_ph.string();

    //std::cout<<current_ph<<"\n";

    std::string curent_path=get_current_dir_name();
    std::cout<<"The current path is:\n";
    std::cout<<curent_path<<"\n";


    curent_path+="/src/object_recognition/data";

    ObjRec ink(curent_path);

    ink.classification_train();



    std::cout<<"Press enter to terminate the programm\n";
    std::cin.get();

	return 0;
}
