/*
*	ObjRec.cpp
*
*	Created on March 15th 2016
*	Last update on April 14th 2016
*	Author: Iason Batzianoulis
*
*/


#include "objrec.h"


//#include <opencv2/core/core.hpp>

#include "preprocf.h"

//#include <opencv2/imgcodecs.hpp>
int count = 0;													// image counter
int cmnd = -1;													// variable to handle the communication with the user
bool flag, mannual_id;											// a boolian variable to handle the choises of the user,
                                                                // flag: handles the choice of taking pictures or not
                                                                // mannual_id: handles the choice which the user can insert mannually







void CommandThread(void){
    /*	This function is a thread handler. It controls the console interface (the communication with the user).
        It provides an asynchronous communication with the user

        flag=true  -> the mode is "take a picture from the camera"
        flag=false -> the mode is "not take a picture"

        mannual_id=true  -> the id of the picture will be set by the program (increase the variable 'count' at each time)
        mannual_id=false -> the user will insert the id of the picture (don't increase the variable 'count')

        the options of the variable 'cmd' are:
        1: take a picture from the camera
        2: don't take a picture
        3: save the picture (after taking a picture from the camera)
        4: don't save the picture

    */
    //usleep(200000);
    std::string l_in(" ");
    while (1){
        std::cout << "\nDo you want to want to take a picture? (y/n) ";
        std::cin >> l_in;

        if (l_in == "y"){
            if (mannual_id){
                std::cout << "Type the id of the picture to take: ";
                std::cin >> count;
            }

            cmnd = 1;
            flag = true;
            usleep(200000);
            //std::this_thread::sleep_for(std::chrono::seconds(1));		// sleep for 1 second
        }
        else if (l_in == "n"){
            cmnd = 2;
            flag = false;
            break;
        }
        else{
            std::cout << "Wrong choice\ntry again\n";
        }
        if (flag){
            std::cout << "Do you want to save the picture (y/n) ";
            std::cin >> l_in;
            if (l_in == "y"){
                cmnd = 3;
                usleep(200000);
                //std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            else if (l_in == "n"){
                std::cout << "The image wasn't saved\n";
                cmnd = 4;
            }
            else{
                std::cout << "Wrong choice\ntry again\n";
            }
        }
    }
}



int dirExists(std::string tpath){
    /*	This function checks if the directory 'tpath' exists
        if it exists the function returns 0
    */

    struct stat info;

    if (stat(tpath.c_str(), &info) != 0){
        // could not have access in the folder
        return 0;
    }else if (info.st_mode & S_IFDIR){
        // the folder exists
        return 1;
    }
    else{
        // the folder doesn't exist
        return 1;
    }
}


ObjRec::ObjRec(){
    std::cout<<"ObjectRecognition object created\n";
}


ObjRec::ObjRec(int c_id){

    camera_id = c_id;
    std::cout<<"I'm in\n";
    /*	an empty constructor mainly for the demo
    */

    //boost::filesystem::path current_ph= boost::filesystem::current_path();				// the current path
    //current_ph.string();
    //path = current_ph.string() +"\\bin\\demo";											// the path of the data


    take_p = false;


}



ObjRec::ObjRec(std::string ph,bool do_train, int c_id){
    /*	Basic constructor for training and testing
    */
    camera_id = c_id;


    std::cout << "**********************************************************\n";
    std::cout << "*|------------------------------------------------------|*\n";
    std::cout << "*|      Welcome to the object recognition program       |*\n";
    std::cout << "*|------------------------------------------------------|*\n";
    std::cout << "**********************************************************\n\n";


    path = ph;


    // creating the necessary folders

    std::string dir_path = path + "/pictures";//\\raw";
    if (!dirExists(dir_path)){
        if(mkdir(dir_path.c_str(),0777)==0){
            dir_path+="//raw";
            if(mkdir(dir_path.c_str(),0777)==0){
                std::cout << "a folder created to store the raw pictures" << "\n";
            }
        }else{
            std::cout<<"Unable to create the folder:\n"<<dir_path<<"\n";
        }

        dir_path.clear();
    }


    dir_path = path + "//pictures//processed";
    if (!dirExists(dir_path)){
        if(mkdir(dir_path.c_str(),0700)==0){
             std::cout << "a folder created to store the raw pictures" << "\n";
        }else{
            std::cout<<"Unable to create the folder:\n"<<dir_path<<"\n";
        }
        dir_path.clear();
    }


    dir_path = path + "//pictures//training";
    if (!dirExists(dir_path)){
        if(mkdir(dir_path.c_str(),0700)==0){
             std::cout << "a folder created to store the raw pictures" << "\n";
        }else{
            std::cout<<"Unable to create the folder:\n"<<dir_path<<"\n";
        }
        dir_path.clear();
    }


    dir_path = path + "//pictures//testing";
    if (!dirExists(dir_path)){
        if(mkdir(dir_path.c_str(),0700)==0){
             std::cout << "a folder created to store the raw pictures" << "\n";
        }else{
            std::cout<<"Unable to create the folder:\n"<<dir_path<<"\n";
        }
        dir_path.clear();
    }

    dir_path = path + "//pca_model";
    if (!dirExists(dir_path)){
        if(mkdir(dir_path.c_str(),0700)==0){
             std::cout << "a folder created to store the raw pictures" << "\n";
        }else{
            std::cout<<"Unable to create the folder:\n"<<dir_path<<"\n";
        }
        dir_path.clear();
    }

    if (do_train){
        std::string choice, ch_tmp;
        std::cout << "\nDo you want to take pictures of the objects? (y/n)\n";


        while (1){
            std::cin >> choice;
            if (choice == "y"){

                take_p = true;
                std::cout << "How many different objects (classes) will you use? ";
                std::cin >> nbObjs;
                nbImgs=std::vector<int>(nbObjs,0);
                std::cout << "Do you want to enter manually the id of each picture? (y/n) ";
                std::cin >> ch_tmp;
                if (ch_tmp == "y") mannual_id = true;
                break;
            }
            else if (choice == "n"){
                take_p = false;
                std::cout << "How many different objects do you have? ";
                std::cin >> nbObjs;
                nbImgs=std::vector<int>(nbObjs,0);
                set_nbImjs();
                break;
            }
            else{
                std::cout << "Wrong choice!! Type 'y' for yes or 'n' for no.\n";
            }
        }

        int ch;

        std::cout << "Set the rows for the processed picture\nchoices:\n1. 640x480\n2. 256x192\n3. 128x96\n4. 64x48\nType the number of your choice: ";
        std::cin >> ch;

        switch (ch)
        {
        case 1:
            im_cols = 640;
            im_rows = 480;
            break;
        case 2:
            im_cols = 256;
            im_rows = 192;
            break;
        case 3:
            im_cols = 128;
            im_rows = 96;
            break;
        case 4:
            im_cols = 64;
            im_rows = 48;
            break;
        default:
            std::cout << "\nWrong choice. Using the default (128x96)\n";
            im_cols = 128;
            im_rows = 96;
            break;
        }
    }
    else{

        std::cout<<"Loading trained system ...\n";

        if (loadParamff("param")){
            std::cout << "Error when loading the parameters\nCheck if the file exists\n";
        }

        std::cout<<"number of objects: "<<nbObjs<<"\nnumbers of pictures:\n";

        for(int k=0;k<nbObjs;k++){
            std::cout<<"for the class "<<k+1<<" you have "<< nbImgs[k] <<"pics\n";
        }

        std::cout<<"You have kept "<<nbComp<<" principal components\n";



        if (loadWff("W")){
            std::cout << "Error when loading the transformation matrix\n";
        }
    }



}



ObjRec::~ObjRec(){


    std::cout << "\nthe object deleted\n";

}



int ObjRec::set_nbObjs(int nb){
    /* setting the number of different objects
    */
    nbObjs = nb;
    return 1;
}



int ObjRec::set_nbImjs(){
    /* setting the number of images per object
    */
    for(int i=0;i<nbObjs;i++){
        std::cout << "How many pictures do you have of the object number " << i+1 << " ? ";
        std::cin >> nbImgs[i];
    }
    return 1;
}



int ObjRec::capture_pics(int nb_class){
    /*	This funtion captures pictures, process them and stores both the captured picture and processed picture to the "raw" and "processed" folders
        respectively. The variable nb_class corresponds to the id of the class of the pictures to be taken.
    */



    // converting the number of class to string

    std::string Result;															// string which will contain the result of the number-to-string convertion

    std::ostringstream convert;													// stream used for the conversion

    // insert the textual representation of 'nb_class' in the characters in the stream

    if (nb_class < 10){
        convert << 0 <<nb_class;
    }
    else{
        convert << nb_class;
    }


    Result = convert.str();														// set 'Result' to the contents of the stream

    std::string fname_raw = path + "//pictures//raw//captured" + Result;		// the path of the raw picture

    std::string fname_pr = path + "//pictures//processed//captured" + Result;	// the path of the processed picture;


    cv::VideoCapture capture(camera_id);										// open a camera to stream frames (0 for the default camera)

    // check if the camera is open

    if (!capture.isOpened()){
        std::cout << "Cannot open the camera number "<< camera_id <<", try again\n";
        return -1;
    }





    // activate this if you want to set the buffer size of the camera (by default is 3 pics)
    //capture.set(CV_CAP_PROP_BUFFERSIZE, 0);


    // set the resolution of the image to 640x480
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);



    cv::Mat camera_frame(cv::Size(640, 480), CV_8UC3);                      // object to store and manipulate the image (catch the frame), CV_8UC3
    cv::Mat camera_roll(cv::Size(640, 480), CV_8UC3);                       // object to throw the pictures for live streaming of the camera


    cv::Mat pic_r(cv::Size(im_cols, im_rows), CV_8UC3);                     // object to store the processed image

    cv::Mat tmp_pic;                                                        // object for checking if the images still exists


    boost::thread *command_thread = new boost::thread(&CommandThread);		// creating a thread to handle the asynchronous communication with the user

    /* the loop bellow checks the value of cmnd and captures,saves or closes the program depending on the choice of the user
       the interface with the console is controlled by the thread*/


    while (1){

        std::string tmp_nb;											// a string to store the number of the image
        std::ostringstream convert2;								// a converter from numbers to strings
        if (count < 9){
            convert2 << "0" << count+1;
        }
        else{ convert2 << count+1; }

        std::string f_raw = fname_raw + convert2.str() + ".png";	// the path and file to save the raw image

        std::string f_pr = fname_pr + convert2.str() + ".png";		//the path and file to save the processed image

        //capturing a frame from the camera and display it in a window
        capture.read(camera_roll);
        cv::imshow("Camera Live", camera_roll);

        // wait for the camera to catch the next frame
        cv::waitKey(20);



        // if the command is to capture an image, catch a frame and display it on another window
        if (cmnd == 1){
            capture.read(camera_frame);
            cv::imshow("Image", camera_frame);
            cv::waitKey(10);
            // process the image (find the object, crop the image around the object and convert it to gray-scale)
            pic_r = process_pic2gray(camera_frame, im_cols, im_rows);
            cv::imshow("Processed image", pic_r);
            cv::waitKey(10);
            cmnd = -1;
        }

        // if the command is to stop taking pictures, break the loop
        if (cmnd == 2){
            capture.release();

            cv::destroyWindow("Camera Live");
            cv::destroyAllWindows();
            break;
        }

        // if the command is to save the picture, save the raw image and also process the image and save it seperetaly
        if (cmnd == 3){

            // check if the image exists
            tmp_pic=cv::imread(f_raw.c_str(), CV_LOAD_IMAGE_COLOR);

            if(!tmp_pic.data){                  // if the image doesn't exist save the image
                // save the image in the folder "raw"
                cv::imwrite(f_raw.c_str(), camera_frame);


                // save the processed image in the folder "processed"
                cv::imwrite(f_pr.c_str(), pic_r);
                nbImgs[nb_class-1]++;
                std::cout << "the image number " <<  nbImgs[nb_class-1] << " of the class " << nb_class << " has been saved\n";
                count++;

            }else{                          // if the image exists ask for replacing it
                char tmp_c;
                std::cout<<"The image already exists!\nDo you want to replace it? (y/n) ";
                std::cin>>tmp_c;
                if(tmp_c=='y'){
                    // save the image in the folder "raw"
                    cv::imwrite(f_raw.c_str(), camera_frame);


                    // save the processed image in the folder "processed"
                    cv::imwrite(f_pr.c_str(), pic_r);

                    std::cout << "the image number " << count+1 << " of the class " << nb_class << " has been saved\n";

                }else{
                    std::cout << "The image has not been saved\n";
                }
            }


            cv::waitKey(100);
            cv::destroyWindow("Image");						// close the window of the image
            cv::destroyWindow("Processed image");
                                                // increase the number of image to be taken
            cmnd = -1;
        }

        // if the command is not to save the picture, close the window of the image
        if (cmnd == 4){
            cv::destroyWindow("Image");
            cv::destroyWindow("Processed image");

            cmnd = -1;
        }

    }



    //nbImgs = count;					// setting count as the number of images for the class
    //std::cout << "You took "<< nbImgs <<" pictures of the object number " << nb_class << "\n";
    count = 0;
    cmnd = -1;


    return 0;
}



int ObjRec::classification_train(){
    /*	This function handles the iteraction with the user and trains the PCA model. Moreover, it stores the data to a txt file and
        returns 0 if no error occured or 1 in case of an error.
        More particularly, the funtion does the following actions:
        1. Captures pictures of objects by the choice of the user
        2. Seperates the data to a training and a testing set by the choice of the user
        3. Applies PCA to the data
        4. Provides the extra choices of:
                a. display components
                b. display compontents of the pictures
                c. display the pictures keeping some components
        5. Handles the communitation with the user on definig the number of components to keep.
           At this step, the programm classifies the training set and the testing set printing the results to the console
    */

    std::cout << "\n\n";
    std::cout << "--------------------------------------\n";
    std::cout << "     Training procedure\n";
    std::cout << "---------------------------------------\n\n";

    if (take_p){
        std::cout << "Prepare to take pictures of the objects\n";
        std::cout << "(Press enter to continue)\n";
        std::cin.get();
    }



    std::string choice("y");											// if the the input from the user is string it will be stores in "choice"




    // The process of taking pictures of different objects
    // The user is asked if wants to take pictures.
    // if yes, the user is asked each time about the id of the object.
    // Then the function capture_pics is called in order to capture all that the user wants of an object

    if (take_p){
    int tmp;
    while (choice != "n"){

        std::cout << "Type the id of the object (1 to " << nbObjs << ")\n";
        std::cin >> tmp;
        std::cout << "\n====================================================\n";
        std::cout << "   Taking pictures of the object number: " << tmp;
        std::cout << "\n====================================================\n";
        if (capture_pics(tmp)){
            std::cout << "Something went wrong\nRepeat the process for the same class\n";
        }
        cv::destroyAllWindows();
        std::cout << "Do you want to continue to another object? (y/n)\n";
        std::cin >> choice;
    }
    }
    // check if the data are stored properly and if the number of the taken pictures corresponds
    // to the expected number as declared in the constructor. In the case the number doesn't match
    // the function returns 1


    if (check_data()){
        std::cout << "Please repeat the process\n";
        return 1;
    }

    // setting the number of pictures for training
    int sum=0;
    for(int k=0;k<nbObjs;k++){
        sum=nbImgs[k]+sum;
    }
    std::cout << "You took " << sum << " pictures of " << nbObjs << " objects (classes)\n";
    std::cout << "How many pictures do you want to use for training? (for each class)\n";
    std::cin >> nbIm_train;
    //nbIm_test = nbImgs - nbIm_train;

    // seperating the data to training and testin sets
    // At this point the user is asked if wants the programm to seperate the data or do it manually.
    // In the first choice the function sep_data is cold, which seperates the data to training and
    // testing sets. The seperated data are stored in the 'training' and 'testing' folders respectively.
    // In the second case, the user is asked to seperate the images manually and place them in to the
    // aformentioned folders. The program waits for this process to be completed, asking the user to
    // declare that by pressing enter.

    std::cout << "Do you want to randomly seperate the dataset to training and testing set? (y/n)\n";
    std::cin >> choice;
    if (choice == "y"){
        if (sep_data()){
            std::cout << "Sorry but something went wrong and the seperation of the data failed. Seperate the data manually or repeat the proccess\n";
            std::cout << "Press enter if you wish to continue\n";
            std::cin.get();

        }
        else{
            nbIm_test=sum-nbObjs*nbIm_train;

            std::cout << "The data are seperated to " << nbObjs*nbIm_train << " pictures for training and " << nbIm_test << " pictures for testing for every class\n";
        }

    }
    else{
        std::cout << "Ok!\nSeperate the pictures to the training folder and the testing folder and press enter\n";
        //std::cin.get();
        std::cin.get();
        while (!load_labels()){
            std::cout << "Check the data and press enter to continue\n";
            std::cin.get();
        }
    }


    // read the pictures and make 2D matrix of all the pictures
    std::vector<std::vector<double>> imageMatrix = pics2matrix("training");				// the trainind set
    std::vector<std::vector<double>> pcs_test = pics2matrix("testing");					// the testing set

    // do PCA to the dataset
    // the function small_PCA applies the PCA to the training set and returns the transformation matrix
    // after the PCA the transformation matrix is saves to a txt file

    std::cout << "Apply PCA to the training data ...\n";

    Wmatrix = small_PCA(imageMatrix);

    std::cout << "PCA is completed!\n";

    std::vector<int> classresults_train;                    // vectors to store the results
    std::vector<int> classresults_test;

    // decide about the number of components
    // the following loop asks the user if wants to enter the extra choices regarding the display
    // of the components of the PCA and the power table. Afterwards, asks the user about the number
    // of components to keep and proceeds to the classification of the training-set and the testing-set
    // when the user is satisfied with the results, the loop is terminated.

    while (1){

        std::cout << "Do you want to enter the extra menu? (y/n)\n";
        std::cin >> choice;
        if (choice == "y"){
                if (extra_menu(imageMatrix)){
                    std::cout << "Someting went wrong with the extra choices.\nTry again later!!\n\n";
                }
        }else if (choice != "n"){
            std::cout << "Wrong choice. Please type 'y' for yes or 'n' for no.\n\n";
            continue;
        }


        std::cout << "How many components do you want to use\n";
        std::cin >> nbComp;

        std::cout << "\nTraining the classification model ...\n";

        // classification of the training set
        classresults_train = classify_dataset(imageMatrix, "train");


        std::cout << "\nThe classification success rate in the training set is: " << successrate(classresults_train, train_grndtruth) * 100 << "%\n";


        // validation with the testing set
        std::cout << "\nClassifying testing set:\n";

        classresults_test = classify_dataset(pcs_test, "test");

        std::cout << "\nThe classification success rate in the testing set is: " << successrate(classresults_test, test_grndtruth) * 100 << "%\n";


        std::cout << "Are you satisfied with the result? (yes/no)\n";
        std::cin >> choice;
        if (choice == "yes"){
            break;
        }
        else if (choice=="no"){
            std::cout << "Ok\nRepeat the process\n";
            continue;
        }
        else{ std::cout << "Wrong choice!\nRepeat the process"; }
    }

    int a=saveM2file(Wmatrix, "W");
    int b=saveParam2file("param");
    if (!a || !b){
        std::cout << "The data were successfully saved!!!\n";
    }


    return 0;
}



int *random_numbers(int nbr,int maxnbr){
    /*	This function generates a vector of random integers, different with each other.
        The number of elements of the vector is nbr and the elements are smaller than maxnbr.
    */
    int *vect = new int[nbr];								// a vector that will contain the random integers and the output of the function
    bool check = false;										// boolian variable to handle the check of the already existance of the integer
    for (int i = 0; i < nbr; i++){
        do{
            vect[i] = rand() % maxnbr + 1;					// generate a random number from one to maxnbr

            // check if this number has already been taken
            for (int j = 0; j < i; j++){
                if (vect[j] == vect[i]){
                    check = true;
                    break;
                }
                check = false;
            }

        } while (check);
        check = true;
    }


    return vect;
}



int ObjRec::sep_data(){
    /*	This function seperate the pictures to the training and testing set by copying the data to the corresponding folders
        The function returns 0 if no error occured or the number of the images that don't exist
    */



    int *train_nums;                                    							// a vector of integers that contains the ids od the pictures that will go for training


    std::cout << "\n";

    std::string path_from = path + "//pictures//processed//captured";				// string which contains the path of the pictures
    std::string path_training = path + "//pictures//training//captured";			// string which contains the path that the pictures for training will be copied
    std::string path_testing = path + "//pictures//testing//captured";				// string which contains the path that the pictures for testing will be copied

    cv::Mat pic(cv::Size(im_cols, im_rows), CV_8UC3);								// the object that will contain the picture

    bool check;																		// variable to check if the id of a picture belongs to the ids for training

    int er_cnt=0;																	// variable for counting the images that don't exist

    // copy-paste the pictures to the training forlder and the testing folder with respect to their ids

    for (int i = 1; i < nbObjs+1; i++){
        train_nums = random_numbers(nbIm_train, nbImgs[i-1]);
        for (int j = 1; j < nbImgs[i-1] + 1; j++){
            std::ostringstream convert;
            if (i < 10){
                if (j < 10){
                    convert << "0" << i << "0" << j;
                }
                else{
                    convert << "0" << i << j;
                }
            }
            else {
                if (j < 10){
                    convert << i << "0" << j;
                }
                else{
                    convert << i << j;
                }
            }
            path_from = path + "//pictures//processed//captured" + convert.str() + ".png";
            path_training = path + "//pictures//training//captured" + convert.str() + ".png";
            path_testing = path + "//pictures//testing//captured" + convert.str() + ".png";

            pic = cv::imread(path_from.c_str(), CV_LOAD_IMAGE_COLOR);

            if (!pic.data)                              // Check for invalid input
            {
                std::cout << "The picture captured" << convert.str() << ".png doesn't seem to exist\n";
                return 1;
            }

            for (int k = 0; k < nbIm_train; k++){
                if (train_nums[k] == j){
                    check = true;
                    break;
                }
                else check = false;
            }
            if (check){
                train_grndtruth.push_back(i);
                cv::imwrite(path_training.c_str(), pic);
            }
            else{
                cv::imwrite(path_testing.c_str(), pic);
                test_grndtruth.push_back(i);
            }


        }
    }


    return er_cnt;
}



bool ObjRec::load_labels(){
    /*	This function reads the class id of each picture in the training and testing folders
        and stores them in the train_grndtruth and test_grndtruth respectively. It returns "true"
        if the number of pictures of the training and testing set match the ones declared from
        the user.
    */
    std::string path_training;// = path + "//pictures//training//captured";			// string which contains the path that the pictures for training will be copied
    std::string path_testing;// = path + "//pictures//testing//captured";				// string which contains the path that the pictures for testing will be copied

    cv::Mat pic(cv::Size(im_cols, im_rows), CV_8UC3);								// the object that will contain the picture



    int er_cnt = 0;																	// variable for counting the images that don't exist



    for (int i = 1; i < nbObjs + 1; i++){

        for (int j = 1; j < nbImgs[i-1] + 1; j++){
            std::ostringstream convert;
            if (i < 10){
                if (j < 10){
                    convert << "0" << i << "0" << j;
                }
                else{
                    convert << "0" << i << j;
                }
            }
            else {
                if (j < 10){
                    convert << i << "0" << j;
                }
                else{
                    convert << i << j;
                }
            }

            path_training = path + "//pictures//training//captured" + convert.str() + ".png";
            path_testing = path + "//pictures//testing//captured" + convert.str() + ".png";

            pic = cv::imread(path_training.c_str(), CV_LOAD_IMAGE_COLOR);

            if (!pic.data){                            // Check for invalid input

                pic = cv::imread(path_testing.c_str(), CV_LOAD_IMAGE_COLOR);
                if (!pic.data){
                    std::cout << "The picture captured" << convert.str() << ".png doesn't exist\n";
                    er_cnt++;
                }else{
                    test_grndtruth.push_back(i);
                }
            }
            else {
                train_grndtruth.push_back(i);
            }
        }
    }


    if (er_cnt > 0){
        std::cout << "\n" << er_cnt << " pictures don't exist in total.\nPlease check the data\n";
        return false;
    }
    else{
        return true;
    }

}



int ObjRec::check_data(){
    /*	This function checks if there are nbImgs pictures for each of the nbObjs classes.
        It prints the pictures that are missing and in this case it returns the number of pctures that missing.
        If all the pictures exist, the function returns 0.
    */

    int count_absences = 0;													// a variable that contains the number of pictures missing. also the output of the function

    std::string path_from = path + "//pictures//processed//captured";		// the path of the pictures

    cv::Mat pic(cv::Size(im_cols, im_rows), CV_8UC3);						// the object that will contain the picture

    std::string pic_file;

    // for every class (different object)
    for (int i = 1; i < nbObjs + 1; i++){
        // for every picture of the class
        for (int j = 1; j < nbImgs[i-1] + 1; j++){

            // converting number to string
            std::ostringstream convert;
            if (i < 10){
                if (j < 10){
                    convert << "0" << i << "0" << j;
                }
                else{
                    convert << "0" << i << j;
                }
            }
            else {
                if (j < 10){
                    convert << i << "0" << j;
                }
                else{
                    convert << i << j;
                }
            }
            pic_file = path_from + convert.str() + ".png";							// the path of the picture

            pic = cv::imread(pic_file.c_str(), 0);									// load the picture from the folder

            if (!pic.data)															// Check for invalid input
            {
                std::cout << "The picture captured" << convert.str() << ".png doesn't seem to exist\n";
                count_absences++;
            }
        }
    }



    if (count_absences > 0){
        std::cout << count_absences << " pictures don't seem to exist\n";
    }
    return count_absences;

}



std::vector<std::vector<double>> ObjRec::pics2matrix(std::string mode){
    /*	This function reads the image from the folder mode and returns a matrix that contains
    */

    std::string path_from = path +"//pictures//"+ mode +"//captured";	// the path of the pictures

    std::vector<std::vector<double>> p2m;								// the output matrix of the function

    cv::Mat pic;														// the object that will contain the picture

    int ct_images = 0,cl = 0;											// variables for counting the images found and the pixels respectively


    // the following loop checks one by one for all the pictures of all the classes if the picture exists in the folder
    // if it exists, the matrix of pixels of the picture reshapes to a vector row-wise, meaning that the elements of the
    // second row will be concatinated to the elements of the first row etc.

    for (int i = 1; i < nbObjs + 1; i++){

        for (int j = 1; j < nbImgs[i-1] + 1; j++){

            // convert a number to string
            std::ostringstream convert;
            if (i < 10){
                if (j < 10){
                    convert << "0" << i << "0" << j;
                }
                else{
                    convert << "0" << i << j;
                }
            }
            else {
                if (j < 10){
                    convert << i << "0" << j;
                }
                else{
                    convert << i << j;
                }
            }
            std::string imgp=path_from + convert.str() + ".png";				// the path of the picture


            pic = cv::imread(imgp.c_str(), 0);									// load the picture from the folder

            cv::waitKey(20);													// wait for the picture to be loaded


            cv::Scalar intent;													// the object to contain the values of the pixel


            if (!pic.empty()){													// if the picture exists

                p2m.push_back(std::vector<double>(pic.rows * pic.cols, 0));		// create another row in the output matrix and initialize the elements to zero
                cl = 0;															// initialize the pixel counter to zero

                // for every row of the picture
                for (int l = 0; l < pic.rows; l++){
                    // for every element (pixel) of the row of the picture
                    for (int k = 0; k < pic.cols; k++){


                        intent = pic.at<uchar>(l, k);							// store the intensity of the pixel to a scalar value, this is a necessary convertion

                        p2m[ct_images][cl] = intent.val[0];						// store the value of the pixel to the output matrix

                        cl++;													// increase the counter of pixels
                    }
                }

                ct_images++;													// increase the picture counter
            }

        }
    }


    return p2m;
}



std::vector<std::vector<double>> ObjRec::findClassMeans(std::vector<std::vector<double>> imageMatrix){
    /*	This funtion returns the mean of each class.
        The input matrix 'imageMatrix' should have each picture as row while the collumns corespond to
        the dimensions of the picture (=number of pixels).
        The output matrix has the means of each class as a rows. Moreover, the dimensions of the output
        matrix are (number of classes)x(number of dimensions of the picture).
        For each class, the function groups the pictures that belong to each class (that corresponds to
        the value of 'nbIm_train') and calculates the mean for each group.
    */


    std::vector<std::vector<double>> means;										// the output of the function

    // for each class
    for (int i = 0; i < nbObjs; i++){
        // create a new row in the output matrix, initializing with zero
        means.push_back(std::vector<double>(imageMatrix[0].size(), 0));
        // for each column of the input matrix
        for (int k = 0; k < (int)imageMatrix[0].size(); k++){
            // group the rows acording to nbIm_train
            for (int j = i*nbIm_train; j < nbIm_train + i*nbIm_train; j++){
                // summation of the collumn
                means[i][k] = means[i][k] + imageMatrix[j][k];
            }
            // mean of that collumn
            means[i][k] = means[i][k] / nbIm_train;
        }
    }



    return means;

}



std::vector<double> ObjRec::findGlobalMeans(std::vector<std::vector<double>> imageMatrix){
    /*	This funtion returns the means of all the pictures for each dimension.
        The input matrix 'imageMatrix' should have each picture as row while the collumns corespond to
        the dimensions of the picture (=number of pixels).
        The output vector contains the means for each dimension and its length is (number of dimensions
        of the picture).
    */


    std::vector<double> gmeans(imageMatrix[0].size(),0);

    for (int i = 0; i < (int)imageMatrix[0].size(); i++){
        for (int j = 0; j < (int)imageMatrix.size(); j++){
            gmeans[i] = gmeans[i] + imageMatrix[j][i];
        }
        gmeans[i] = gmeans[i] / (double)imageMatrix.size();
    }


    return gmeans;

}



std::vector<int> ObjRec::classify_dataset(std::vector<std::vector<double>> dataset, std::string mode){
    /*	This function classifies each row of the input 'matrix'. The string 'mode' indicate if
        the dataset is the training set or the testing set by having the values 'train' or 'tesst'
        respectively.
        The function returns the classification success rate according to the matrices 'train_grndtruth'
        and 'test_grndtruth'
    */


    // substract the means

    std::vector<std::vector<double>> ctrd_data(dataset.size(), std::vector<double>(dataset[0].size(), 0));

    for (int i = 0; i < (int)dataset.size(); i++){
        for (int j = 0; j < (int)dataset[i].size(); j++){
            ctrd_data[i][j] = dataset[i][j] - globalMeans[j];
        }
    }



    // project the data to the new hyperspace
    std::vector<std::vector<double>> proj_data = dotProduct(ctrd_data, Wmatrix);


    if (mode == "train"){
        // find the means of each class for the projected data
        classMeans = findClassMeans(proj_data);
    }



    std::vector<int> class_results (dataset.size(),0);					// a vector to contain the classification result for each row

    for (int i = 0; i < (int)dataset.size(); i++){
        class_results[i] = classification_test(proj_data[i]);
        std::cout << "The image number " << i+1 << " classified as class number " << class_results[i] << "\n";


    }

    return class_results;

}



int ObjRec::classification_test(std::vector<double> imgd){
    /*	This function classifies the vector 'imgd' to one of the classes.
        It does the following process:
        1. find the distances from the mean of each class
        2. returns the class that corresponds to the shortest distance

    */

    // calculate the distances from the means of every class
    std::vector<double> distances(nbObjs, 0);

    // use the number of components that are defined by the user in the function 'classification_train()'
    for (int i = 0; i < nbObjs; i++){
        for (int j = 0; j < nbComp; j++){
            distances[i] = distances[i] + (imgd[j] - classMeans[i][j])*(imgd[j] - classMeans[i][j]);
        }
        distances[i] = std::sqrt(distances[i]);
    }

    // find which element of the vector 'distances' is the smallest and its order in the vector 'distances'
    std::vector<double>::iterator result;
    result = std::min_element(distances.begin(), distances.end());


    return std::distance(distances.begin(), result) + 1;

}



int ObjRec::classify_online(int c_id){
    /*	This function captures a picture from a camera (given by the variable 'c_id') and classifies that
        picture to one of the classes. It returns the winner class of 0 in the case of error

    */



    cv::VideoCapture capture(c_id);												// open a camera to stream frames (0 for the default camera)

    // check if the camera is open

    if (!capture.isOpened()){
        std::cout << "Cannot open the camera number " << camera_id << ", try again\n";
        return 0;
    }


    // activate this if you want to set the buffer size of the camera (by default is 3 pics)
    //capture.set(CV_CAP_PROP_BUFFERSIZE, 0);


    // set the resolution of the image to 640x480
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    cv::Mat camera_frame(cv::Size(640, 480), CV_8UC3);					// object to store and manipulate the image (catch the frame), CV_8UC3

    cv::Mat pic_r(cv::Size(im_cols, im_rows), CV_8UC3);					// object to store the processed image


    //capturing a frame from the camera and display it in a window
    capture.read(camera_frame);
    cv::imshow("Camera Live", camera_frame);

    // wait for the camera to catch the next frame
    cv::waitKey(20);

    // process the image (find the object, crop the image around the object and convert it to gray-scale)
    pic_r = process_pic2gray(camera_frame, im_cols, im_rows);
    cv::imshow("Processed image", pic_r);
    cv::waitKey(20);


    std::vector<double> vec_frame(pic_r.rows * pic_r.cols, 0);				// a vector to contain the reshaped picture

    cv::Scalar intent;														// the object to contain the values of the pixel

    int cl = 0;																// initialize the pixel counter to zero

    if (!pic_r.empty()){													// if the picture exists
        // for every row of the picture
        for (int l = 0; l < pic_r.rows; l++){
            // for every element (pixel) of the row of the picture
            for (int k = 0; k < pic_r.cols; k++){
                intent = pic_r.at<uchar>(l, k);								// store the intensity of the pixel to a scalar value, this is a necessary convertion
                vec_frame[cl] = intent.val[0];								// store the value of the pixel to the output matrix
                cl++;														// increase the counter of pixels
            }
        }
    }
    else{
        std::cout << "The camera didn't capture anything\n";
        return 0;
    }


    // substract the means
    for (int i = 0; i < (int)vec_frame.size(); i++){
        vec_frame[i] = vec_frame[i] - globalMeans[i];
    }

    // project the data to the new hyperspace
    std::vector<double> proj_data = dotProduct(vec_frame, Wmatrix);


    return classification_test(proj_data);

}



float ObjRec::successrate(std::vector<int> results, std::vector<int> groundTruth){
    /*	This function returns the success rate by comparing each element of the vector 'results'
        with the corresponeding element of the vector 'groundtrouth'
    */

    int count = 0;

    for (int i = 0; i < (int)results.size(); i++){
        if (results[i] == groundTruth[i]){
            count++;
        }
    }


    return count / (float)results.size();
}



std::vector<std::vector<double>> ObjRec::small_PCA(std::vector<std::vector<double>> imgMa){
    /*  This function applies PCA to the input data matrix imgMa columnwise. It returns the coefficient matrix
        having each column as one eigenvector. The eigenvectors are sorted with respect to the descending order
        of eigenvalues (the first column is the eigenvector that corresponds to the largest eigenvalue, the second
        column to the second largest eigenvalue etc.)
    */


    //create a Mat object to contain the data
    cv::Mat mpics((int)imgMa.size(), (int)imgMa[0].size(), CV_64FC1);

    // calculate the means of the dataset
    globalMeans = findGlobalMeans(imgMa);

    // transfer the data from a vector of vectors to the Mat object
    for (int i = 0; i < (int)imgMa.size(); i++){
        for (int j = 0; j < (int)imgMa[i].size(); j++){
            mpics.at<double>(i, j) = imgMa[i][j]-globalMeans[j];
        }
    }


    // apply PCA with using the PCA class of opencv
    cv::PCA pca_obj(mpics, cv::Mat(), CV_PCA_DATA_AS_ROW, 0);


    // create the object to contain the eigenvectors to return
    std::vector<std::vector<double>> w_matrix;

    // store the eigenvectors to a vector of vectors
    for(int i = 0; i < pca_obj.eigenvectors.cols; i++){
        w_matrix.push_back(std::vector<double>(pca_obj.eigenvectors.rows, 0));
        for (int j = 0; j < pca_obj.eigenvectors.rows; j++){
            w_matrix[i][j] = pca_obj.eigenvectors.at<double>(j, i);
        }
    }


    // store the eigenvalues to a vector
    eigen_values = std::vector<double>(pca_obj.eigenvalues.rows, 0);

    for(int i = 0; i < pca_obj.eigenvalues.rows; i++){
        eigen_values[i] = pca_obj.eigenvalues.at<double>(i);
    }


    return w_matrix;

}



std::vector<std::vector<double>> ObjRec::dotProduct(std::vector<std::vector<double>> M1, std::vector<std::vector<double>> M2){
    /*	This function returns the product of two matrices
    */

    // initial checks in the dimensions of the input patrices
    for (int i = 0; i < (int)M1.size(); i++){
        if (M1[0].size() != M1[i].size()){
            std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
            std::cout << "the rows of the first matrix are not of the same length\n";
            std::cout << "press enter to terminate the programm\n";
            std::cin.get();
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < (int)M2.size(); i++){
        if (M2[0].size() != M2[i].size()){
            std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
            std::cout << "the rows of the second matrix are not of the same length\n";
            std::cout << "press enter to terminate the programm\n";
            std::cin.get();
            exit(EXIT_FAILURE);
        }
    }
    if (M1[0].size() != M2.size()){
        std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
        std::cout << "the dimensions of the matrices don't macth\n";
        std::cout << "press enter to terminate the programm\n";
        std::cin.get();
        exit(EXIT_SUCCESS);
    }

    std::vector<std::vector<double>> outMatrix;

    for (int i = 0; i < (int)M1.size(); i++){
        outMatrix.push_back(std::vector<double>(M2[0].size(), 0));
        for (int j = 0; j < (int)M2[0].size(); j++){
            for (int k = 0; k < (int)M2.size(); k++){
                outMatrix[i][j] = outMatrix[i][j] + M1[i][k] * M2[k][j];
            }
        }
    }





    return outMatrix;


}



std::vector<double> ObjRec::dotProduct(std::vector<double> M1, std::vector<std::vector<double>> M2){
    /*	This function returns the product between a vector and a matrix
    */

    // initial checks in the dimensions of the input patrices

    for (int i = 0; i < (int)M2.size(); i++){
        if (M2[0].size() != M2[i].size()){
            std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
            std::cout << "the rows of the second matrix are not of the same length\n";
            std::cout << "press enter to terminate the programm\n";
            std::cin.get();
            exit(EXIT_FAILURE);
        }
    }
    if (M1.size() != M2.size()){
        std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
        std::cout << "the dimensions of the matrices don't macth\n";
        std::cout << "press enter to terminate the programm\n";
        std::cin.get();
        exit(EXIT_SUCCESS);
    }

    std::vector<double> outVector(M2[0].size(),0);

    for (int i = 0; i < (int)M2[0].size(); i++){
        for (int j = 0; j < (int)M2.size(); j++){
            outVector[i] = outVector[i] + M1[j] * M2[j][i];
        }
    }





    return outVector;


}



std::vector<double> ObjRec::dotProduct(std::vector<std::vector<double>> M1, std::vector<double> M2){
    /*	This function returns the product between a vector and a matrix
    */

    // initial checks in the dimensions of the input patrices

    for (int i = 0; i < (int)M1.size(); i++){
        if (M1[0].size() != M1[i].size()){
            std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
            std::cout << "the rows of the first matrix are not of the same length\n";
            std::cout << "press enter to terminate the programm\n";
            std::cin.get();
            exit(EXIT_FAILURE);
        }
    }
    if (M1[0].size() != M2.size()){
        std::cout << "error in Pca::dotProduct(std::vector<std::vector<double>>, std::vector<std::vector<double>>)\n";
        std::cout << "the dimensions of the matrices don't macth\n";
        std::cout << "press enter to terminate the programm\n";
        std::cin.get();
        exit(EXIT_SUCCESS);
    }

    std::vector<double> outVector(M1.size(), 0);

    for (int i = 0; i < (int)M1.size(); i++){
        for (int j = 0; j < (int)M2.size(); j++){
            outVector[i] = outVector[i] + M1[i][j] * M2[j];
        }
    }





    return outVector;


}



std::vector<std::vector<double>> ObjRec::transp(std::vector<std::vector<double>> mattrix){
    /*	This function transposes the input matrix
    */

    std::vector<std::vector<double>> matrixT;
    for (int i = 0; i < (int)mattrix[0].size(); i++){
        matrixT.push_back(std::vector<double>((int)mattrix.size(), 0));
        for (int j = 0; j < (int)mattrix.size(); j++){
            matrixT[i][j] = mattrix[j][i];
        }
    }

    return matrixT;
}



int ObjRec::saveM2file(std::vector<std::vector<double>> tm, std::string name_){
    /*	This function saves the matrix tm to a txt file with 'name_' as the name of the file.
        It returns 0 if no error occured, and 1 if cannot open the file. The elements of the
        matrix are saved row-wise, meaning that the function begin to save the matrix starting
        from the first row, then the second etc. The elements are saved one next to the other,
        meaning the next element is stored after a space ' ' in the same line. In order to recover
        the matrix, the dimentions of the matrix should be known.
    */


    std::string pt=path+ "//pca_model//" + name_ + ".txt";					// the path that the matrix will be saved

    std::ofstream myfile(pt.c_str());										// create and open the file

    if (myfile.is_open()){													// if the file is open
        for (int i = 0; i < (int)tm.size(); i++){
            for (int j = 0; j < (int)tm[i].size(); j++){
                myfile << tm[i][j] << " ";									// store the element to the file
            }
        }
        std::cout << "the matrix has been saved to:\n" << pt << "\n";
    }
    else{
        std::cout << "Unable to create the file: " << pt << "\n";
        return 1;
    }


    myfile.close();															// close the file
    return 0;
}



int ObjRec::saveParam2file(std::string pw){
    /*	This function saves the parameters of the object to a txt file with 'pw' as the name of the file.
        It returns 0 if no error occured, and 1 if cannot open the file. The variable that have only one
        element (like integers) are stored in the format below:

                    name_of_variable: its_value

        These variables are sotred one line after another. The elements of the vectors or matrixes, are stored
        in the format below:
                    name_of_variable:
                    element(1,1)
                    element(1,2)
                         .
                         .
                         .
                    element(n,n)

        The elements of the matrix are stored row-wise, meaning that the function begin to save the matrix starting
        from the first row, then stores the second etc. In order to recover the matrix, one must first search for the
        name of the variable and the values following the name belong to the variable.
    */

    std::string path_write = path + "//pca_model//" + pw + ".txt";				// the file to save the parameters

    std::ostringstream convert;													// object to convert numbers to strings


    std::ofstream myfile(path_write.c_str());									// create and open the txt file
    if (myfile.is_open()){														// if the file is open

        // store the number of rows of the processed picture
        convert << im_rows;
        myfile << "im_rows: " << convert.str() << "\n";
        convert.str("");
        convert.clear();

        // store the number of columns of the processed picture
        convert << im_cols;
        myfile << "im_cols: " << convert.str() << "\n";
        convert.str("");
        convert.clear();

        // store the number of different objects (classes)
        convert << nbObjs;
        myfile << "nbObjs: " << convert.str() << "\n";
        convert.str("");
        convert.clear();



        // store the number of pictures to be used for training
        convert << nbIm_train;
        myfile << "nbIm_train: " << convert.str() << "\n";
        convert.str("");
        convert.clear();

        // store the number of pictures to be used for testing
        convert << nbIm_test;
        myfile << "nbIm_test: " << convert.str() << "\n";
        convert.str("");
        convert.clear();

        // store the number of components to keep
        convert << nbComp;
        myfile << "nbComp: " << convert.str() << "\n";
        convert.str("");
        convert.clear();

        // store the number of pictures for each object
        myfile << "nbImgs:\n";
        for(int i=0;i<nbObjs;i++){
            myfile << nbImgs[i]<<"\n";
        }


        // store the means of each class
        myfile << "classMeans:\n";
        for (int i = 0; i < (int)classMeans.size(); i++){
            for (int j = 0; j < (int)classMeans[i].size(); j++){
                myfile << classMeans[i][j] << "\n";
            }
        }

        // store the means of of the pictures
        myfile << "globalMeans:\n";
        for (int i = 0; i < (int)globalMeans.size(); i++){

            myfile << globalMeans[i] << "\n";

        }

        // store the eigen_values
        myfile << "eigen_values:\n";
        for (int i = 0; i < (int)eigen_values.size(); i++){

            myfile << eigen_values[i] << "\n";

        }

        myfile.close();														// close file
    }
    else{
        std::cout << "Unable to open the file: " << path_write << "\n";
        return 1;
    }


    return 0;
}



int ObjRec::loadParamff(std::string pw){



    std::string path_read = path + "//pca_model//" + pw + ".txt";



    std::ifstream myfile(path_read.c_str());

    if (myfile.is_open()){
        std::string tmp_str;

        getline(myfile, tmp_str);
        std::size_t fnd = tmp_str.find(" ");
        im_rows = std::atoi(tmp_str.substr(fnd).c_str());


        getline(myfile, tmp_str);
        fnd = tmp_str.find(" ");
        im_cols = std::atoi(tmp_str.substr(fnd).c_str());

        getline(myfile, tmp_str);
        fnd = tmp_str.find(" ");
        nbObjs = std::atoi(tmp_str.substr(fnd).c_str());

//        getline(myfile, tmp_str);
//        fnd = tmp_str.find(" ");
//        nbImgs = std::atoi(tmp_str.substr(fnd).c_str());

        getline(myfile, tmp_str);
        fnd = tmp_str.find(" ");
        nbIm_train = std::atoi(tmp_str.substr(fnd).c_str());

        getline(myfile, tmp_str);
        fnd = tmp_str.find(" ");
        nbIm_test = std::atoi(tmp_str.substr(fnd).c_str());

        getline(myfile, tmp_str);
        fnd = tmp_str.find(" ");
        nbComp = std::atoi(tmp_str.substr(fnd).c_str());

        nbImgs=std::vector<int>(nbObjs,0);
        eigen_values = std::vector<double>(nbIm_train*nbObjs, 0);
        globalMeans = std::vector<double>(im_rows*im_cols, 0);
        classMeans = std::vector<std::vector<double>>(nbObjs, std::vector<double>(nbIm_train*nbObjs, 0));

        //getline(myfile, tmp_str);
        getline(myfile, tmp_str);
        for (int i = 0; i < nbObjs; i++){

            myfile >> nbImgs[i];

        }


        getline(myfile, tmp_str);
        getline(myfile, tmp_str);



        for (int i = 0; i < nbObjs; i++){
            for (int j = 0; j < nbIm_train*nbObjs; j++){
                myfile >> classMeans[i][j];
            }
        }

        getline(myfile, tmp_str);
        getline(myfile, tmp_str);

        for (int i = 0; i < im_rows*im_cols; i++){

            myfile >> globalMeans[i];

        }

        getline(myfile, tmp_str);
        getline(myfile, tmp_str);
        for (int i = 0; i < nbIm_train*nbObjs; i++){

            myfile >> eigen_values[i];

        }



        myfile.close();

    }
    else{
        std::cout << "Unable to open the file: \n" << path_read << "\n";
        return 1;
    }

    return 0;
}



int ObjRec::loadWff(std::string pw){



    std::string path_read = path + "//pca_model//" + pw +".txt";
    Wmatrix = std::vector<std::vector<double>>(im_rows*im_cols, std::vector<double>(nbIm_train*nbObjs, 0));

    std::ifstream myfile(path_read.c_str());

    if (myfile.is_open()){
        for (int i = 0; i < im_rows*im_cols; i++){
            for (int j = 0; j < nbIm_train*nbObjs; j++){

                myfile >> Wmatrix[i][j];
            }
        }
        myfile.close();
    }
    else{
        std::cout << "Unable to open the file: " << path_read << "\n";
        return 1;
    }



    return 0;
}



cv::Mat ObjRec::normal(cv::Mat matrix, int lower, int higher, bool integ, int tpe){
    /*	This function normalizes the input matrix. It returns a matrix which elements are between the "lower" and "higher" values
    */

    double mini = matrix.at<double>(0, 0), maxi = matrix.at<double>(0, 0);

    for (int i = 0; i < matrix.rows; i++){
        for (int j = 0; j < matrix.cols; j++){
            if (matrix.at<double>(i, j) < mini){
                mini = matrix.at<double>(i, j);
            }
            if (matrix.at<double>(i, j) > maxi){
                maxi = matrix.at<double>(i, j);
            }
        }
    }


    double diff = maxi - mini;
    cv::Mat rf(matrix.rows, matrix.cols, tpe);
    if (integ){
        for (int i = 0; i < matrix.rows; i++){
            for (int j = 0; j < matrix.cols; j++){
                if (tpe == 0 || tpe == 8 || tpe == 16 || tpe == 24){
                    rf.at<uchar>(i, j) = (uchar)floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 1 || tpe == 9 || tpe == 17 || tpe == 25){
                    rf.at<char>(i, j) = (char)floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 2 || tpe == 10 || tpe == 18 || tpe == 26){
                    rf.at<ushort>(i, j) = (ushort)floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 3 || tpe == 11 || tpe == 19 || tpe == 27){
                    rf.at<short>(i, j) = (short)floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 4 || tpe == 12 || tpe == 20 || tpe == 28){
                    rf.at<int>(i, j) = (int)floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 5 || tpe == 13 || tpe == 21 || tpe == 29){
                    rf.at<float>(i, j) = (float)floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 6 || tpe == 14 || tpe == 22 || tpe == 30){
                    rf.at<double>(i, j) = floor(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
            }
        }
    }
    else{
        for (int i = 0; i < matrix.rows; i++){
            for (int j = 0; j < matrix.cols; j++){
                if (tpe == 0 || tpe == 8 || tpe == 16 || tpe == 24){
                    rf.at<uchar>(i, j) = (uchar)(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 1 || tpe == 9 || tpe == 17 || tpe == 25){
                    rf.at<char>(i, j) = (char)(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 2 || tpe == 10 || tpe == 18 || tpe == 26){
                    rf.at<ushort>(i, j) = (ushort)(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 3 || tpe == 11 || tpe == 19 || tpe == 27){
                    rf.at<short>(i, j) = (short)(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 4 || tpe == 12 || tpe == 20 || tpe == 28){
                    rf.at<int>(i, j) = (int)(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 5 || tpe == 13 || tpe == 21 || tpe == 29){
                    rf.at<float>(i, j) = (float)(lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }
                if (tpe == 6 || tpe == 14 || tpe == 22 || tpe == 30){
                    rf.at<double>(i, j) = (lower + ((matrix.at<double>(i, j) - mini) / diff)*(lower + higher));
                }

            }
        }
    }

    return rf;
}



cv::Mat ObjRec::normal(std::vector<double> matrix, int lower, int higher, bool integ, int tpe){
    /*	This function normalizes the input matrix. It returns a matrix which elements are between the "lower" and "higher" values
    */

    double mini = matrix[0], maxi = matrix[0];

    for (int i = 0; i < (int)matrix.size(); i++){
        if (matrix[i] < mini){
            mini = matrix[i];
        }
        if (matrix[i]> maxi){
            maxi = matrix[i];
        }
    }


    double diff = maxi - mini;
    cv::Mat rf(1, (int)matrix.size(), tpe);

    if (integ){
        for (int i = 0; i < (int)matrix.size(); i++){
            if (tpe == 0 || tpe == 8 || tpe == 16 || tpe == 24){
                rf.at<uchar>(0, i) = (uchar)floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
            if (tpe == 1 || tpe == 9 || tpe == 17 || tpe == 25){
                rf.at<char>(0, i) = (char)floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
            if (tpe == 2 || tpe == 10 || tpe == 18 || tpe == 26){
                rf.at<ushort>(0, i) = (ushort)floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
            if (tpe == 3 || tpe == 11 || tpe == 19 || tpe == 27){
                rf.at<short>(0, i) = (short)floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
            if (tpe == 4 || tpe == 12 || tpe == 20 || tpe == 28){
                rf.at<int>(0, i) = (int)floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
            if (tpe == 5 || tpe == 13 || tpe == 21 || tpe == 29){
                rf.at<float>(0, i) = (float)floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
            if (tpe == 6 || tpe == 14 || tpe == 22 || tpe == 30){
                rf.at<double>(0, i) = floor(lower + ((matrix[i] - mini) / diff)*(lower + higher));
            }
        }
    }
    else{
        for (int j = 0; j < (int)matrix.size(); j++){
            if (tpe == 0 || tpe == 8 || tpe == 16 || tpe == 24){
                rf.at<uchar>(0, j) = (uchar)(lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
            if (tpe == 1 || tpe == 9 || tpe == 17 || tpe == 25){
                rf.at<char>(0, j) = (char)(lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
            if (tpe == 2 || tpe == 10 || tpe == 18 || tpe == 26){
                rf.at<ushort>(0, j) = (ushort)(lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
            if (tpe == 3 || tpe == 11 || tpe == 19 || tpe == 27){
                rf.at<short>(0, j) = (short)(lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
            if (tpe == 4 || tpe == 12 || tpe == 20 || tpe == 28){
                rf.at<int>(0, j) = (int)(lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
            if (tpe == 5 || tpe == 13 || tpe == 21 || tpe == 29){
                rf.at<float>(0, j) = (float)(lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
            if (tpe == 6 || tpe == 14 || tpe == 22 || tpe == 30){
                rf.at<double>(0, j) = (lower + ((matrix[j] - mini) / diff)*(lower + higher));
            }
        }
    }

    return rf;
}



int ObjRec::displayCopmponent(int nbC){
    // display one component
    if (nbC < 1){
        std::cout << "\nThe number of component cannot be zero or negative\n";
        return 1;
    }

    if (nbC >(int)eigen_values.size() - 1){
        std::cout << "\nComponent number " << nbC << " does not exist\n";
        std::cout << "There are  " << (int)eigen_values.size() << " components of PCA\n";
        std::cout << "Try again\n";
        return 1;
    }


    cv::Mat tmpImg(im_rows, im_cols, CV_8UC1); //CV_64FC1

    cv::Mat tmpImg2;//(1, pca_obj.eigenvectors.cols, CV_8UC1, cv::Scalar(0));


    tmpImg2 = normal(transp(Wmatrix)[nbC-1], 0, 255);


    std::string winname("Component ");
    std::ostringstream convert;
    convert << nbC;
    winname += convert.str();

    //tmpImg = tmpImg2.reshape(0, im_rows);
    int count = 0;
    for (int i = 0; i < im_rows; i++){
        for (int j = 0; j < im_cols; j++){
            tmpImg.at<uchar>(i, j) = tmpImg2.at<uchar>(0, count);
            count++;
        }
    }

    cv::namedWindow(winname, CV_WINDOW_FREERATIO);
    imshow(winname, tmpImg);
    std::cout << "Click on the image to continue\n";
    cv::waitKey(0);


    return 0;
}



int ObjRec::dispProjImgComp(std::vector<std::vector<double>> imgMat,int nbPic, int nbC){
    /* This function diplays a component of one image to a window
    */

    if (nbPic>(int)imgMat.size()){
        std::cout << "\nThe image number "<< nbPic <<" does not exist\n";
        std::cout << "The images number could be from one to" << (int)imgMat.size() << "\n";
        return 1;
    }

    if (nbPic<0){
        std::cout << "\nThe number of picture cannot be zero or negative\n";
        return 1;
    }

    if (nbC < 1){
        std::cout << "\nThe number of component cannot be zero or negative\n";
        return 1;
    }

    if (nbC >(int)eigen_values.size() - 1){
        std::cout << "\nComponent number " << nbC << " does not exist\n";
        std::cout << "There are  " << (int)eigen_values.size() << " components of PCA\n";
        std::cout << "Try again\n";
        return 1;
    }

    // seperating the image and substract the means
    std::vector<double> img_vec((int)imgMat[nbPic - 1].size(), 0);
    for (int i = 0; i < (int)img_vec.size(); i++){
        img_vec[i] = imgMat[nbPic - 1][i] - globalMeans[i];
    }




    // project the data to the new hyperspace
    std::vector<double> proj_data = dotProduct(img_vec, Wmatrix);


    std::vector<double> proj_back(Wmatrix.size(), 0);


    for (int i = 0; i < (int)Wmatrix.size(); i++){
        proj_back[i] = proj_data[nbC - 1] * Wmatrix[i][nbC - 1];
    }



    cv::Mat tmpImg(im_rows, im_cols, CV_8UC1); //CV_64FC1

    cv::Mat tmpImg2;//(1, pca_obj.eigenvectors.cols, CV_8UC1, cv::Scalar(0));


    tmpImg2 = normal(proj_back, 0, 255);


    std::string winname("Image ");
    std::ostringstream num_pic;
    num_pic<< nbPic;
    std::string cComp(" Components ");
    std::ostringstream num_comp;
    num_comp << nbC;
    winname += num_pic.str();
    winname += cComp;
    winname += num_comp.str();



    //tmpImg = tmpImg2.reshape(0, im_rows);
    int count = 0;
    for (int i = 0; i < im_rows; i++){
        for (int j = 0; j < im_cols; j++){
            tmpImg.at<uchar>(i, j) = tmpImg2.at<uchar>(0, count);
            count++;
        }
    }

    cv::namedWindow(winname, CV_WINDOW_FREERATIO);
    imshow(winname, tmpImg);
    std::cout << "Click on the image to continue\n";
    cv::waitKey(0);


    return 0;
}



int ObjRec::dispImg(std::vector<std::vector<double>> imgMat, int nbPic, int nbC){
    /* This function diplays a component of one image to a window
    */

    if (nbPic>(int)imgMat.size()){
        std::cout << "\nThe image number " << nbPic << " does not exist\n";
        std::cout << "The images number could be from one to" << (int)imgMat.size() << "\n";
        return 1;
    }

    if (nbPic<0){
        std::cout << "\nThe number of picture cannot be zero or negative\n";
        return 1;
    }

    if (nbC < 1){
        std::cout << "\nThe number of component cannot be zero or negative\n";
        return 1;
    }

    if (nbC >(int)eigen_values.size()){
        std::cout << "\nComponent number " << nbC << " does not exist\n";
        std::cout << "There are  " << (int)eigen_values.size() << " components of PCA\n";
        std::cout << "Try again\n";
        return 1;
    }

    // seperating the image and substract the means
    std::vector<double> img_vec((int)imgMat[nbPic - 1].size(), 0);
    for (int i = 0; i < (int)img_vec.size(); i++){
        img_vec[i] = imgMat[nbPic - 1][i] - globalMeans[i];
    }



    // project the data to the new hyperspace
    std::vector<double> proj_data = dotProduct(img_vec, Wmatrix);


    // create a matrix by keeping only nbC components
    std::vector<std::vector<double>> smallW(Wmatrix.size(), std::vector<double>(nbC, 0));
    for (int i = 0; i < (int)Wmatrix.size(); i++){
        for (int j = 0; j < nbC - 1; j++){
            smallW[i][j] = Wmatrix[i][j];
        }
    }

    std::cout << "dims of small W: " << smallW.size() << "x" << smallW[0].size() << "\n";

    // create a vector by keeping only nbC components
    std::vector<double> fcomp(nbC, 0);
    for (int i = 0; i < nbC; i++){
        fcomp[i] = proj_data[i];
    }



    // projecting back the image to the initial space
    std::vector<double> proj_back = dotProduct(smallW, fcomp);



    cv::Mat tmpImg(im_rows, im_cols, CV_8UC1); //CV_64FC1

    cv::Mat tmpImg2;//(1, pca_obj.eigenvectors.cols, CV_8UC1, cv::Scalar(0));


    tmpImg2 = normal(proj_back, 0, 255);


    std::string winname("Image ");
    std::ostringstream num_pic;
    num_pic << nbPic;
    std::string cComp(" Components ");
    std::ostringstream num_comp;
    num_comp << nbC;
    winname += num_pic.str();
    winname += cComp;
    winname += num_comp.str();



    //tmpImg = tmpImg2.reshape(0, im_rows);
    int count = 0;
    for (int i = 0; i < im_rows; i++){
        for (int j = 0; j < im_cols; j++){
            tmpImg.at<uchar>(i, j) = tmpImg2.at<uchar>(0, count);
            count++;
        }
    }

    cv::namedWindow(winname, CV_WINDOW_FREERATIO);
    imshow(winname, tmpImg);
    std::cout << "Click on the image to continue\n";
    cv::waitKey(0);


    return 0;
}



int ObjRec::printPowerTable(){

    double sum = 0;
    for (int i = 0; i < (int)eigen_values.size(); i++){
        sum = sum + eigen_values[i];
    }


    // find the smaller number between 20 and the number of eigenvalues

    std::vector<int> minnb;
    minnb.push_back(20);
    minnb.push_back((int)eigen_values.size());
    std::vector<int>::iterator result;
    result = std::min_element(minnb.begin(), minnb.end());
    int mini = minnb[std::distance(minnb.begin(), result)];



    std::cout << "\n            PCA Power Table:\n";
    std::cout << "number of components  |   information\n";
    std::cout << "-------------------------------------\n";


    std::string percent;
    std::size_t str_length;
    double lsum = 0;
    for (int i = 0; i < mini; i++){
        lsum = lsum + eigen_values[i];
        if (i < 9) std::cout << "          " << i + 1 << "           |";
        else std::cout << "         " << i + 1 << "           |";
        percent=round_number((lsum / sum) * 100, 2);
        str_length = percent.length();
        if (str_length < 4) std::cout << "       " << percent << "\n";
        if ((str_length >= 4) && (str_length < 6)) std::cout << "      " << percent << "\n";
        if (str_length >= 6) std::cout << "     " << percent << "\n";
    }
    std::cout << "\n\n";

    if (eigen_values.size() > 20){
        for (int i = 20; i < (int)eigen_values.size(); i = i + 10){
            for (int j = mini; j < i; j++){
                lsum = lsum + eigen_values[i];
            }
            std::cout << "         " << i + 1 << "           |";
            percent = round_number((lsum / sum) * 100, 2);
            str_length = percent.length();
            if (str_length < 4) std::cout << "       " << percent << "\n";
            if ((str_length >= 4) && (str_length < 6)) std::cout << "      " << percent << "\n";
            if (str_length >= 6) std::cout << "     " << percent << "\n";
            mini = i;
        }
        std::cout << "         " << eigen_values.size() << "           |     100.00 \n";
    }


    return 0;
}



std::string ObjRec::round_number(double nb, int acq){

    std::ostringstream convert;

    convert << nb;

    std::string nbstr(convert.str());

    std::size_t fnd = nbstr.find(".");


    if (fnd > nbstr.length()){
        return nbstr+".00";
    }
    else{
        return  nbstr.substr(0, fnd + acq + 1);
    }


}



int ObjRec::displayOneComponent(){

    char input='y';
    int nbC;

    while (1){


        if (input == 'y'){
            std::cout << "Which component you want to display?\n";
            std::cin >> nbC;
            if (displayCopmponent(nbC)){
                std::cout << "Try again\n";
            }
        }
        std::cout << "Do you want to display another component?(y/n)\n";
        std::cin >> input;
        if (input == 'n'){
            break;
        }

    }

    return 0;
}



int ObjRec::displayIgmComponent(std::vector<std::vector<double>> pcs){

    char input='y';
    int nbC, nbpic;

    while (1){

        if (input == 'y'){
            std::cout << "Which image you want to display?\n";
            std::cin >> nbpic;
            std::cout << "Which component you want to display?\n";
            std::cin >> nbC;
            if (dispProjImgComp(pcs, nbpic, nbC)){
                std::cout << "Try again\n";
            }
        }
        std::cout << "Do you want to display another component of an image?(y/n)\n";
        std::cin >> input;
        if (input == 'n'){
            break;
        }

    }

    return 0;
}



int ObjRec::displayWithComponents(std::vector<std::vector<double>> pcs){

    char input='y';
    int nbC, nbpic;

    while (1){

        if (input == 'y'){
            std::cout << "Which image you want to display?\n";
            std::cin >> nbpic;
            std::cout << "How mmany components do you want to keep?\n";
            std::cin >> nbC;
            if (dispImg(pcs, nbpic, nbC)){
                std::cout << "Try again\n";
            }
        }
        std::cout << "Do you want to display another image keeping a number of components?(y/n)\n";
        std::cin >> input;
        if (input == 'n'){
            break;
        }

    }

    return 0;
}



int ObjRec::extra_menu(std::vector<std::vector<double>> imageMatrix){


    int choice;
    std::cout << "***********************************\n";
    std::cout << "          Extra menu\n";
    std::cout << "***********************************\n\n";

    std::cout << "     The extra choices are:\n";
    std::cout << "-------------------------------------\n";
    std::cout << "   1  | Print the power table\n";
    std::cout << "   2  | Display an eigenimage\n";
    std::cout << "   3  | Display a component froma a picture\n";
    std::cout << "   4  | Display a picture keeping a number of components\n";
    std::cout << "   5  | Exit the extra menu\n";


    while (1){
        std::cout << "Type your choice: ";
        std::cin >> choice;
        switch (choice){
        case 1:
            if (printPowerTable()) {
                std::cout << "Unable to print the power table\n";
                std::cout << "There was a problem with the function\n int ObjRec::printPowerTable()\n";
                return 1;
            }
            break;
        case 2:
            if (displayOneComponent()) {
                std::cout << "There was a problem with the function\n int ObjRec::displayOneComponent()\n";
                return 1;
            }
            break;
        case 3:
            if (displayIgmComponent(imageMatrix)){
                std::cout << "There was a problem with the function\n int ObjRec::displayIgmComponent(std::vector<std::vector<double>>)\n";
                return 1;
            }
            break;
        case 4:
            if (displayWithComponents(imageMatrix)){
                std::cout << "There was a problem with the function\n int ObjRec::displayWithComponents(std::vector<std::vector<double>>)\n";
                return 1;
            }
            break;
        case 5:
            break;
        default:
            std::cout << "Wrong choice. Try again \n";
            break;
        }
        if (choice == 5){
            break;
        }

    }


    return 0;
}



int ObjRec::demo(){


    std::cout << "-------------------------------------------------------------\n";
    std::cout << "-------------------------------------------------------------\n\n";
    std::cout << "          Welcome to the Object Recognition demo\n\n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "-------------------------------------------------------------\n\n";

    std::string pic_path2(path+"//pics//raw//captured0");
    std::string pic_path3(path+"//pics//processed/captured0");


    std::cout << "This demo classifies 30 pictures to 3 objects (classes). \nThe PCA model is created using the 60% of the data, while the rest 40% of the dataset is used for validation\n";
    std::cout << "In this demo, you can choose either to retrain the model or not. You can also use different number of components to be used in the classification procedure\n";
    std::cout << "The default dimensions of the processed image is 64x48\n";
    std::cout << "If you are ready to proceed press enter\n";
    std::cin.get();





    std::cout << "\n\nLoading training dataset ...\n";

    std::vector<std::vector<double>> pcs = pics2matrix("training");

    std::cout << "Loading testing dataset ...\n";

    std::vector<std::vector<double>> pcs_test = pics2matrix("testing");

    char inl;

    std::cout << "Do you want to train a new model? (y/n) ";
    std::cin >> inl;
    switch (inl){
    case 'y':
        set_nbObjs(3);
        set_nbImjs();
        nbIm_train = 6;
        nbIm_test = 4;
        im_rows = 48;
        im_cols = 64;
        std::cout << "Train PCA model ...\n";
        Wmatrix = small_PCA(pcs);
        std::cout << "The PCA model is trained\n";
        std::cout << "Save PCA model ...\n";
        saveM2file(Wmatrix, 0);
        break;
    case 'n':
        std::cout << "Loading befault transformation matrix\n";
        if (loadParamff("modelParameters")){
            std::cout << "Error when loading the parameters\n";
        }
        if (loadWff("W0")){
            std::cout << "Error when loading the transformation matrix\n";
        }
        break;
    default:
        std::cout << "Wrong choice.\n";
        std::cout << "Loading befault transformation matrix\n";
        if (loadParamff("modelParameters")){
            std::cout << "Error when loading the parameters\n";
        }
        if (loadWff("W0")){
            std::cout << "Error when loading the transformation matrix\n";
        }
        break;
    }








    printPowerTable();

    if (displayOneComponent()){
        std::cout << "There was a problem with the function\n int ObjRec::displayOneComponent()\n";
    }

    if (displayIgmComponent(pcs)){
        std::cout << "There was a problem with the function\n int ObjRec::displayIgmComponent(std::vector<std::vector<double>>)\n";
    }

    if (displayWithComponents(pcs)){
        std::cout << "There was a problem with the function\n int ObjRec::displayWithComponents(std::vector<std::vector<double>>)\n";
    }


    std::cout << "Type the number of components you want to use: ";
    std::cin >> nbComp;
    while (nbComp > (int)eigen_values.size() || (nbComp<1)){
        std::cout << "Wrong number.\n";
        std::cout << "The number of components to use shoulb be between 1 and " << eigen_values.size() << "\n";
        std::cout << "Select another number: ";
        std::cin >> nbComp;
    }




    std::cout << "\n\n  Classifying training set:\n";
    std::cout << "----------------------------\n";

    std::vector<int> classresults = classify_dataset(pcs, "train");

    std::cout << "\nThe classification success rate in the training set is: " << successrate(classresults, train_grndtruth) * 100 << "%\n";



    std::cout << "\n\n  Classifying testing set:\n";
    std::cout << "----------------------------\n";

    std::vector<int> classresults_test = classify_dataset(pcs_test, "test");

    std::cout << "The classification success rate in the training set is: " << successrate(classresults_test, test_grndtruth) * 100 << "%\n";

    return 1;
}





