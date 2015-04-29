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
 * @Mat& input - 변환될 이미지
 * @float density - 점잡음의 농도 (0이면 변환되지 않고, 1이면 모든 이미지가 노이즈화된다.)
 */
void saltAndPepperNoise(Mat& input, Mat& output, float density) {
    
    const Vec3b salt(255, 255, 255);
    const Vec3b pepper(0, 0, 0);
    
    output = input.clone();
    
    int size = input.rows * input.cols;
    
    int* randa = new int[size];
    int* randc = new int[(int)(size * density)];
    
    for (int i = 0; i < size; i++)
        randa[i] = i;
    
    for (int i = 0; i < size * density; i++) {
        int rando = rand() % (size - i);
        randc[i] = randa[rando];
        randa[rando] = randa[size - i - 1];
    }
    
    for (int i = 0; i < size * density; i++) {
        
        int rx = randc[i] % input.cols;
        int ry = randc[i] / input.cols;
        
        output.at<Vec3b>(Point(rx, ry)) = rand() % 2 ? salt : pepper;
    }
    
    delete randc;
    delete randa;
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
    
    saltAndPepperNoise(image, transform, 0.1f);
    
    // 원본과 변환된 결과를 출력하고 결과를 파일로 저장한다.
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    namedWindow("Transform", CV_WINDOW_AUTOSIZE);
    
    imshow("Original", image);
    imshow("Transform", transform);
    
    imwrite("transform "+filePath, image);
    
    waitKey(0);
    return 0;
}
