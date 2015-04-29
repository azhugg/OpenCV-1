/**
 * Salt and Pepper Noise
 *
 * Written by HyunJun Kim
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

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


/**
 * 프로그램의 시작
 */
int main () {
    
    string filePath;
    Mat image;
    
    // 변환을 수행할 파일을 입력받는다.
    while (true) {
        
        cout << "Input your image path: ";
        cin >> filePath;
        
        image = imread(filePath, 1);
        
        if(!image.data)
            cout << "Could not open or find the image." << endl;
        else break;
    }
    
    // 원본과 변환된 결과를 출력하고 결과를 파일로 저장한다.
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    namedWindow("Transformed", CV_WINDOW_AUTOSIZE);
    
    imshow("Original", image);
    
    saltAndPepperNoise(image, 0.1);
    imshow("Transformed", image);
    imwrite("transformed "+filePath, image);
    
    waitKey(0);
    return 0;
}
