//
//  Salt and Pepper Noise
//
//  Created by HyunJun Kim on 2015. 4. 29.
//  Copyright (c) 2015년 HyunJun Kim. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void saltAndPepperNoise(Mat& image, float density);

int main() {
    
    string filePath;
    Mat image;
    
    // 변환을 적용할 이미지를 불러온다.
    while (true) {
        
        cout << "Input your image path: ";
        cin >> filePath;

        image = imread(filePath, 1);
        
        if(!image.data)
            cout << "Could not open or find the image." << endl;
        else break;
    }
    
    // 이미지에 변환을 적용한다.
    saltAndPepperNoise(image, 0.1f);
    
    // 변환된 이미지를 화면에 띄운다.
    namedWindow("Transformed", CV_WINDOW_AUTOSIZE);
    imshow("Transformed", image);
    
    // 변환된 이미지를 저장한다.
    cout << "Input path to save the output: ";
    cin >> filePath;
    imwrite(filePath, image);
    
    waitKey(0);
    
    return 0;
}

/**
 * 이미지에 노이즈를 추가한다.
 *
 * @Mat& image - 변환될 이미지
 * @float density - 점잡음의 농도 (0이면 변환되지 않고, 1이면 모든 이미지가 노이즈화된다.)
 */
void saltAndPepperNoise(Mat& image, float density) {
    
    const Vec3b salt(255, 255, 255);
    const Vec3b pepper(0, 0, 0);
    
    int totalNoisePixels = image.rows * image.cols * density;
    
    for (int i = 0; i < totalNoisePixels; i++) {
        
        int rx = rand() % image.cols;
        int ry = rand() % image.rows;
        
        image.at<Vec3b>(Point(rx,ry)) = rand() % 2 ? salt : pepper;
    }
}
