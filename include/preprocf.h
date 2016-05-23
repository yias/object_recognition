/*
*	preprocf.h
*
*	Created on March 15th 2016
*	Author: Iason Batzianoulis
*
*/



#ifndef PREPROCF_H
#define PREPROCF_H

#endif // PREPROCF_H



#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


int* center_point(std::vector<int>, std::vector<int>);

cv::Mat cropImage(int center[2], cv::Mat, int, int);

cv::Mat idObj(cv::Mat);

cv::Mat process_pic2gray(cv::Mat,int,int);

int playContPic();
