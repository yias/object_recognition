/*
*	preprocf.cpp
*
*	Created on March 15th 2016
*	Author: Iason Batzianoulis
*
*/


#include "preprocf.h"



int* center_point(std::vector<int> xV, std::vector<int> yV){
    /* This funtion finds the center point of a rectangular by calculating the mean of the x-coordinate and y-coordinate.
    *  It returns a vector of integers which the first element is the x-coordinate of the center point and the second element
    *  is the y-coordinate of the center point.
    */
    int *center;
    center = new int[2]; center[0] = 0; center[1] = 0;

    for (int i = 0; i < (int)xV.size(); i++){
        center[0] = center[0] + xV[i];
    }
    center[0] = center[0] / xV.size();

    for (int i = 0; i < (int)yV.size(); i++){
        center[1] = center[1] + yV[i];
    }
    center[1] = center[1] / yV.size();

    return center;

}

cv::Mat cropImage(int center[2], cv::Mat img,int co, int ro){
    /*This function crops an image, keeping the area around the center point. The area is a rectangular
    * with dimensions 128x96. The funtion returns the croped image*/

    cv::Mat cropedImg(cv::Size(co, ro), CV_8UC3);

    int corner1 = ro/2, corner2 = co/2;

    if (center[0] - corner1 < 0){
        corner1 = 0;
    }

    if (center[0] + corner1 - 1 > img.rows){
        corner1 = cropedImg.rows;
    }

    if (center[1] - corner2 < 0){
        corner2 = 0;
    }

    if (center[1] + corner2 - 1 > img.cols){
        corner2 = cropedImg.cols;
    }


    for (int j = 0; j < cropedImg.cols; j++){
        for (int i = 0; i < cropedImg.rows; i++){
            cropedImg.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(center[0] - corner1 + i, center[1] - corner2 + j);
        }

        //propic3.at<uchar>(center[0] + 47, j) = 255;
    }



    return cropedImg;

}


cv::Mat idObj(cv::Mat img){
    /*  This funtion identifies the object by placing a red rectangular around it.
    It returns the original image with the rectangular */


    cv::Mat propic(img.size(), CV_8UC3);									// the object to store the edges of the object

    double edgeThresh = 25;													// first threshold of the hysteresis procedure

    cv::Canny(img, propic, edgeThresh, edgeThresh * 3, 3);					// find the edges



    std::vector<int> xVec;														// vector to contain all the x-positions of the white pixles
    std::vector<int> yVec;														// vector to contain all the y-positions of the white pixles

    // find the white pixels (high intensity)
    for (int j = 0; j < propic.rows; j++){
        for (int i = 0; i < propic.cols; i++){
            cv::Scalar intensity = propic.at<uchar>(j, i);

            if (intensity.val[0] > 200){
                xVec.push_back(j);
                yVec.push_back(i);
            }
        }
    }

    // find the minimum and the maximum of the white pixels
    std::vector<int>::iterator result = std::min_element(std::begin(xVec), std::end(xVec));
    int xmin = xVec[std::distance(std::begin(xVec), result)];

    result = std::max_element(xVec.begin(), xVec.end());
    int xmax = xVec[std::distance(xVec.begin(), result)];


    result = std::min_element(std::begin(yVec), std::end(yVec));
    int ymin = yVec[std::distance(std::begin(yVec), result)];

    result = std::max_element(yVec.begin(), yVec.end());
    int ymax = yVec[std::distance(yVec.begin(), result)];



    // draw the rectangular
    cv::Vec3b intensity = img.at<cv::Vec3b>(1, 1);
    intensity.val[0] = 0;
    intensity.val[1] = 0;
    intensity.val[2] = 255;

    for (int j = xmin; j < xmax; j++){

        img.at<cv::Vec3b>(j, ymin) = intensity;
    }

    for (int j = xmin; j < xmax; j++){

        img.at<cv::Vec3b>(j, ymax) = intensity;
    }

    for (int i = ymin; i < ymax; i++){

        img.at<cv::Vec3b>(xmin, i) = intensity;
    }

    for (int i = ymin; i < ymax; i++){

        img.at<cv::Vec3b>(xmax, i) = intensity;
    }


    return img;
}


cv::Mat process_pic2gray(cv::Mat img,int co, int ro){
    /*	This funtion returns a cropped gray-scale of the object.
    The dimensions of the returned image are 128x96
    */

    //std::cout << "initial dim: " << img.cols << "x" << img.rows << "\n";

    cv::Mat propic(img.size(), CV_8UC3);										// the object to store the processed image,CV_8UC3

    cv::resize(img, propic, cv::Size(0, 0), 0.5, 0.5);							// resize image in the half size

    //std::cout << "new dim: " << propic.cols << "x" << propic.rows << "\n";

    //cv::imshow("resized", propic);



    // finding the edges of the object to identify it

    cv::Mat propic2(propic.size(), CV_8UC3);									// the object to store the edges of the object

    double edgeThresh = 25;														// first threshold of the hysteresis procedure

    cv::Canny(propic, propic2, edgeThresh, edgeThresh * 3, 3);					// find the edges


    //cv::imshow("edges", propic2);


    //std::cout << "channels: " << propic2.channels() << " rows: " << propic2.rows << " collumns: " << propic2.cols << "\n";

    std::vector<int> xVec;														// vector to contain all the x-positions of the white pixles
    std::vector<int> yVec;														// vector to contain all the y-positions of the white pixles

    // find the white pixels (high intensity)
    for (int j = 0; j < propic2.rows; j++){
        for (int i = 0; i < propic2.cols; i++){
            cv::Scalar intensity = propic2.at<uchar>(j, i);

            if (intensity.val[0] > 200){
                xVec.push_back(j);
                yVec.push_back(i);
            }
        }
    }


    int *center = center_point(xVec, yVec);										// the center point of the oblect


    // croping the image, keeping the area around the object
    cv::Mat propic3(img.size(), CV_8UC3);

    propic3 = cropImage(center, propic, co, ro);


    // transform the image to gray scale
    cv::Mat gray_pic(propic3.size(), CV_8UC3);

    cvtColor(propic3, gray_pic, CV_BGR2GRAY);											// changing the image to gray scale

    //std::cout << gray_pic.rows << "x" << gray_pic.cols << "\n";

    cv::resize(gray_pic, gray_pic, cv::Size(co, ro), 0, 0);//(0,0) 0.5 0.5
    //std::cout << gray_pic.rows << "x" << gray_pic.cols << "\n";

    //cv::imshow("gray pic",gray_pic);

    //cv::waitKey(0);

    //cv::Mat propic4 = idObj(img);

    //cv::imshow("gray pic", propic4);
    //cv::waitKey(0);


    return gray_pic;

}


int playContPic(){
    cv::VideoCapture capture(0);									// open a camera to stream frames (0 for default)

    // check if the camera is open

    if (!capture.isOpened()){
        std::cout << "Cannot open the camera, try again\n";
        return -1;
    }

    //capture.set(CV_CAP_PROP_BUFFERSIZE, 0);

    // set the resolution of the image to 640x480
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    cv::Mat camera_frame(cv::Size(640, 480), CV_8UC3);				// object to store and manipulate the image (catch the frame), CV_8UC3






    while (1){




        capture.read(camera_frame);
        cv::imshow("frame", camera_frame);
        cv::waitKey(20);

        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 1;


}




