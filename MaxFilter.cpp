/**
 * Max Filter
 *
 * Written by HyunJun Kim
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

uchar intensity(Vec3b rgb) {
    return (rgb[0] + rgb[1] + rgb[2]) / 3;
}

/**
 * 이미지에 Maximum Value Filter를 적용한다.
 * (인접한 픽셀 중 가장 greyscale값이 높은 픽셀값을 자신의 값으로 설정)
 *
 * @Mat& image - 변환될 이미지
 */
 void maxFilter(Mat& input, Mat& output) {
    
    const int neighbors[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
    
    output.create(input.rows, input.cols, CV_8UC3);
    
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            
            uchar maximum = 0;
            Vec3b rgb;
            
            for (int i = 0; i < 8; i++) {
                
                int nx = neighbors[i][0] + x;
                int ny = neighbors[i][1] + y;
                
                if(nx < 0 || ny < 0 || nx >= input.cols || ny >= input.rows)
                    continue;
                
                uchar greyscale = intensity(input.at<Vec3b>(ny, nx));
                
                if(maximum < greyscale) {
                    maximum = greyscale;
                    rgb = input.at<Vec3b>(ny, nx);
                }
            }
            output.at<Vec3b>(y, x) = rgb;
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
    
    maxFilter(image, transform);
    
    // 원본과 변환된 결과를 출력하고 결과를 파일로 저장한다.
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    namedWindow("Transform", CV_WINDOW_AUTOSIZE);
    
    imshow("Original", image);
    imshow("Transform", transform);
    
    imwrite("transform "+filePath, image);
    
    waitKey(0);
    return 0;
}
