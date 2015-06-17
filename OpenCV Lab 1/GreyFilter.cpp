/**
 * Grey Filter
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
 * 이미지를 흑백 이미지로 변환한다.
 *
 * @Mat& input - 변환될 이미지
 */
void greyFilter(Mat& input, Mat& output) {
    
    output.create(input.rows, input.cols, CV_8UC1);
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            
            Vec3b rgb = input.at<Vec3b>(y, x);
            
            // Greyscaling by luminosity method
            uchar greyscale = 0.07f * rgb[0] + 0.72f * rgb[1] + 0.21f * rgb[2];
            
            output.at<uchar>(y, x) = greyscale;
        }
    }
}


/**
 * 프로그램의 시작
 */
int main () {
    
    string filePath;
    Mat image, transform;
    
    // 변환을 수행할 파일을 입력받는다.
    while (true) {
        
        cout << "Input your image path: ";
        cin >> filePath;
        
        image = imread(filePath, 1);
        
        if(!image.data)
            cout << "Could not open or find the image." << endl;
        else break;
    }
    
    greyFilter(image, transform);
    
    // 원본과 변환된 결과를 출력하고 결과를 파일로 저장한다.
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    namedWindow("Transform", CV_WINDOW_AUTOSIZE);
    
    imshow("Original", image);
    imshow("Transform", transform);
    
    imwrite("transform "+filePath, image);
    
    waitKey(0);
    return 0;
}