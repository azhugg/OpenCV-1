//
//  main.cpp
//  OpenCV Lab 2
//
//  Created by HyunJun Kim on 2015. 5. 7..
//  Copyright (c) 2015년 HyunJun Kim. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/**
 * 컬러 이미지를 히스토그램 균등화한다.
 *
 * @Mat input 균등화할 이미지
 * @Mat output 균등화된 이미지
 */
void equalize(Mat& input, Mat& output) {
    
    Mat temp;
    
    // 이미지의 색공간을 HSV로 변환한다.
    cvtColor(input, temp, CV_RGB2HSV);
    
    double size = input.rows * input.cols;
    
    int histogram[256];
    int cumulativeHistogram[256];
    int minimumCumulativeValue = 0;
    
    // 원본 이미지의 V성분 히스토그램을 구한다.
    for (int i = 0; i < 256; i++)
    histogram[i] = 0;
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            uchar greyvalue = temp.at<Vec3b>(y, x)[2];
            histogram[greyvalue]++;
        }
    }
    
    // 히스토그램의 최소 성분을 구한다.
    int accumulation = 0;
    for (int i = 0; i < 256; i++) {
        accumulation += histogram[i];
        cumulativeHistogram[i] = accumulation;
        if (minimumCumulativeValue == 0 && accumulation > 0)
        minimumCumulativeValue = accumulation;
    }
    
    // 계산 공식에 의해 이미지를 히스토그램 균등화한다.
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            uchar greyvalue = temp.at<Vec3b>(y, x)[2];
            uchar normalizedIntensity = round(((cumulativeHistogram[greyvalue] - minimumCumulativeValue) / size) * 255);
            temp.at<Vec3b>(y, x)[2] = normalizedIntensity;
        }
    }
    
    // 이미지의 색공간을 RGB로 변환한다.
    cvtColor(temp, output, CV_HSV2RGB);
}

int main() {
     
    Mat image = imread("cat.jpg", 1);
    
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    namedWindow("Transformed", CV_WINDOW_AUTOSIZE);
    
    Mat transformed;
    equalize(image, transformed);
    
    imshow("Original", transformed);
    imshow("Transformed", transformed);
    
    waitKey(0);
    return 0;
}






