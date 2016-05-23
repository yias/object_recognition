/*
*	ObjRec.h
*
*	Created on March 15th 2016
*	Author: Iason Batzianoulis
*/

#ifndef OBJREC_H
#define OBJREC_H



#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

//#include "opencv2/imgproc/imgproc.hpp"

//#include "opencv2/objdetect/objdetect.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
//#include "preprocf.h"
#include <boost/thread.hpp>
//#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
//#include <conio.h>

#include <ncurses.h>





class ObjRec{
    std::string path;								// the path of the practicals
    int im_rows, im_cols;							// dimensions of the processed image
    int nbObjs;                     				// number of different objects
    int nbIm_train, nbIm_test;						// number of images for trianing, number of images in total
    int nbComp;										// the number of components
    int camera_id;									// the id of the camera
    bool take_p;									// a boolian variable to handle if to take new pictures

    std::vector<int> nbImgs;                        // number of images for each object
    std::vector< std::vector<double> > classMeans;	// the means of the classes
    std::vector<double> globalMeans;				// the means of the dataset for each dimension
    std::vector< std::vector<double> > Wmatrix;		// the transformation matrix and the eigenvalues
    std::vector<double> eigen_values;				// the eigenvalues after PCA

    std::vector<int> train_grndtruth;				// ids of the pictures in the training set
    std::vector<int> test_grndtruth;				// ids of the pictures in the testing set




public:
    ObjRec();
    ObjRec(int c_id=0);                                                 // empty constructor mainly for the demo
    ObjRec(std::string, bool do_train = true, int camera_id = 0);		// the main contructor for training, validating the model and online classification
    ~ObjRec();															// destrurctor (saves the transformation matrix and the parameters of the object)
    int set_nbObjs(int);												// set the number of different objects (classes)
    int set_nbImjs();												// set the number of pictures for each objects
    int capture_pics(int);												// capture pictures for one class
    int sep_data();														// seperate data to training and testing set
    int check_data();													// check if the data match the expeted numbers

    std::vector< std::vector<double> > pics2matrix(std::string);													// reads the images and creates a matrix with all the images
    std::vector< std::vector<double> > findClassMeans(std::vector< std::vector<double> >);							// finds the means of each class
    std::vector<double> findGlobalMeans(std::vector< std::vector<double> >);										// finds the means of the whole data set for each dimension

    // functions related to the classification
    int classification_train();																					// train the classifier
    int classification_test(std::vector<double>);																// classify one picture
    std::vector<int> classify_dataset(std::vector< std::vector<double> >, std::string);							// classify a number of pictures
    int classify_online(int c_id = 0);																			// capture a picture from a camera and classify it to one of the classes
    float successrate(std::vector<int>, std::vector<int>);														// calculates the classification success rate
    bool load_labels();

    // functions related to math
    std::vector< std::vector<double> > small_PCA(std::vector< std::vector<double> >);											// pca with the pca of opencv
    std::vector< std::vector<double> > dotProduct(std::vector< std::vector<double> >, std::vector< std::vector<double> >);		// the product of two matrices
    std::vector<double> dotProduct(std::vector<double>, std::vector<std::vector<double> >);									// the product of a vector multiplied by a matrix
    std::vector<double> dotProduct(std::vector< std::vector<double> >, std::vector<double>);									// the product of a matrix multiplied by a vector (from the right side)
    std::vector< std::vector<double> > transp(std::vector< std::vector<double> >);												// the transposed matrix
    cv::Mat normal(cv::Mat, int lower = 0, int higher = 1, bool integ = true, int tpe = 0);									// normalize a matrix
    cv::Mat normal(std::vector<double>, int lower = 0, int higher = 1, bool integ = true, int tpe = 0);						// normalize a vector
    std::string  round_number(double nb, int acq);

    // saving and loading functions
    int saveM2file(std::vector< std::vector<double> >, std::string);															// save a matrix to a file
    int saveParam2file(std::string);																						// save the parameters of the object
    int loadWff(std::string);																								// load the transformation matrix from a text file
    int loadParamff(std::string);																							// load the parameters of the object from a text file





    int extra_menu(std::vector< std::vector<double> >);													// extra menu
    int displayCopmponent(int nbC);																		// display one eigenimage
    int dispProjImgComp(std::vector< std::vector<double> >, int, int);									// display one picture using one component
    int dispImg(std::vector< std::vector<double> >, int, int);											// display one picture by keeping a number of components
    int displayOneComponent();																			// the menu for displaying eigenimages
    int displayIgmComponent(std::vector< std::vector<double> >);											// the menu for displaying components of images
    int displayWithComponents(std::vector< std::vector<double> >);										// the menu for displaying projected image
    int printPowerTable(void);																			// print the power table of eigenvalues (after PCA)

    int demo();        																					// demo of the function
};







#endif // OBJREC_H
