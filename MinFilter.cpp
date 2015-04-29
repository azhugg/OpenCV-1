/**
 * Min Filter
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
 * 이미지에 Minimum Value Filter를 적용한다.
 * (인접한 픽셀 중 가장 greyscale값이 낮은 픽셀값을 자신의 값으로 설정)
 *
 * @Mat& image - 변환될 이미지
 */
Mat minFilter(Mat& image) {
    
    const int neighbors[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
    
    Mat output(image.rows, image.cols, CV_8UC1);
    
    for (int x = 0; x < image.rows; x++) {
        for (int y = 0; y < image.cols; y++) {
            
            uchar minimum = 255;
            
            for (int i = 0; i < 8; i++) {
                
                int nx = neighbors[i][0] + x;
                int ny = neighbors[i][1] + y;
                
                if(nx < 0 || ny < 0 || nx >= image.rows || ny >= image.cols)
                    continue;
                
                Vec3b rgb = image.at<Vec3b>(nx, ny);
                
                // Greyscaling by luminosity method
                uchar greyscale = 0.07f * rgb[0] + 0.72f * rgb[1] + 0.21f * rgb[2];
                
                if(minimum > greyscale)
                    minimum = greyscale;
            }
            output.at<uchar>(x, y) = minimum;
        }
    }
    return output;
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
    
    image = minFilter(image);
    imshow("Transformed", image);
    imwrite("transformed "+filePath, image);
    
    waitKey(0);
    return 0;
}
