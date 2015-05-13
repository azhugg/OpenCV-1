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

/*
 * 원본 이미지의 V성분 히스토그램을 구한다.
 */
void evaluateHistogram(Mat& input, int* output, int channel) {
    
    for (int i = 0; i < 256; i++)
    output[i] = 0;
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            output[input.at<Vec3b>(y, x)[channel]]++;
        }
    }
}

/*
 * 히스토그램의 CDF데이터를 구하고, 그 최솟값을 리턴한다.
 */
int evaluateCdf(int* input, int *output) {
    
    int accumulation = 0;
    int minimum = 0;
    
    for (int i = 0; i < 256; i++) {
        accumulation += input[i];
        output[i] = accumulation;
        if (minimum == 0 && accumulation >= 0)
        minimum = accumulation;
    }
    
    return minimum;
}

/**
 * 컬러 이미지를 히스토그램 균등화한다.
 *
 * @Mat input 균등화할 이미지
 * @Mat output 균등화된 이미지
 */
void equalize(Mat& input, Mat& output) {
    
    Mat temp;
    int channel = 0;
    
    // 이미지의 색공간을 HSV로 변환한다.
    cvtColor(input, temp, CV_RGB2Lab);
    
    double size = input.rows * input.cols;
    int histogram[256], cdf[256], minimum = 0;
    
    evaluateHistogram(temp, histogram, channel);
    minimum = evaluateCdf(histogram, cdf);
    
    // 계산 공식에 의해 이미지를 히스토그램 균등화한다.
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            temp.at<Vec3b>(y, x)[channel] = round(((cdf[temp.at<Vec3b>(y, x)[channel]] - minimum) / (size - minimum)) * 255);
        }
    }
    
    // 이미지의 색공간을 RGB로 변환한다.
    cvtColor(temp, output, CV_Lab2RGB);
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






