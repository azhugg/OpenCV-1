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

const int YCrCb[3] = {CV_BGR2YCrCb, CV_YCrCb2BGR, 0};
const int HSV[3] = {CV_BGR2HSV, CV_HSV2BGR, 2};
const int HLS[3] = {CV_BGR2HLS, CV_HLS2BGR, 1};
const int Lab[3] = {CV_BGR2Lab, CV_Lab2BGR, 0};

// 색 공간 정보 (여기서는 L*a*b 색공간 사용)
const int* COLOR_SPACE_CONVERSION = HSV;
const int COLOR_MAX = 255;
const int CHANNEL = COLOR_SPACE_CONVERSION[2];

/* 
 * 원본 이미지의 V성분 히스토그램을 구한다.
 */
void evaluateHistogram(Mat& input, int* output) {
    
    for (int i = 0; i <= COLOR_MAX; i++)
        output[i] = 0;
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            output[(int)input.at<Vec3b>(y, x)[CHANNEL]]++;
        }
    }
}

/*
 * 히스토그램의 CDF데이터를 구하고, 그 최솟값을 리턴한다.
 */
int evaluateCdf(int* input, int *output) {
    
    int accumulation = 0;
    int minimum = 0;
    
    for (int i = 0; i <= COLOR_MAX; i++) {
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
    
    // 이미지의 색공간을 정의된 공간으로 변환한다.
    cvtColor(input, temp, COLOR_SPACE_CONVERSION[0]);
    
    double size = input.rows * input.cols;
    int histogram[COLOR_MAX + 1], cdf[COLOR_MAX + 1], minimum = 0;
    
    evaluateHistogram(temp, histogram);
    minimum = evaluateCdf(histogram, cdf);
    
    // 계산 공식에 의해 이미지를 히스토그램 균등화한다.
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            temp.at<Vec3b>(y, x)[CHANNEL] = round(((cdf[temp.at<Vec3b>(y, x)[CHANNEL]] - minimum) / (size - minimum)) * COLOR_MAX);
        }
    }
    
    // 이미지의 색공간을 RGB로 변환한다.
    cvtColor(temp, output, COLOR_SPACE_CONVERSION[1]);
}



int main() {
     
    Mat image = imread("b.jpg", 1);
    
    namedWindow("Result", CV_WINDOW_AUTOSIZE);
    
    Mat transformed, combined;
    
    equalize(image, transformed);
    hconcat(image, transformed, combined);
    
    imshow("Result", combined);
    imwrite("m.jpg", combined);
    
    waitKey(0);
    return 0;
}






