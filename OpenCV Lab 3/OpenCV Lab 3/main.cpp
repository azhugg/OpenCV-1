#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

#include "opencv2/videoio/videoio_c.h"
#include "opencv2/highgui/highgui_c.h"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                   CascadeClassifier& nestedCascade,
                   double scale, bool tryflip );
void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                  cv::Mat &output, cv::Point2i location);
Mat putMask(Mat src,Point center,Size face_size);
Mat putMask2(Mat src,Point center,Size face_size);
Mat sub, out;
Mat mask;

string cascadeName = "haarcascade_frontalface_alt.xml";
string nestedCascadeName = "haarcascade_eye_tree_eyeglasses.xml";

int main( int argc, const char** argv )
{
    sub = imread("sub3.png");
    
    CvCapture* capture = 0;
    Mat frame, frameCopy, image;
    const string scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const string cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();
    const string nestedCascadeOpt = "--nested-cascade";
    size_t nestedCascadeOptLen = nestedCascadeOpt.length();
    const string tryFlipOpt = "--try-flip";
    size_t tryFlipOptLen = tryFlipOpt.length();
    string inputName;
    bool tryflip = false;
    
    CascadeClassifier cascade, nestedCascade;
    double scale = 1;
    
    
    
    for( int i = 1; i < argc; i++ )
    {
        cout << "Processing " << i << " " <<  argv[i] << endl;
        if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 )
        {
            cascadeName.assign( argv[i] + cascadeOptLen );
            cout << "  from which we have cascadeName= " << cascadeName << endl;
        }
        else if( nestedCascadeOpt.compare( 0, nestedCascadeOptLen, argv[i], nestedCascadeOptLen ) == 0 )
        {
            if( argv[i][nestedCascadeOpt.length()] == '=' )
                nestedCascadeName.assign( argv[i] + nestedCascadeOpt.length() + 1 );
            if( !nestedCascade.load( nestedCascadeName ) )
                cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        }
        else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
        {
            if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
                scale = 1;
            cout << " from which we read scale = " << scale << endl;
        }
        else if( tryFlipOpt.compare( 0, tryFlipOptLen, argv[i], tryFlipOptLen ) == 0 )
        {
            tryflip = true;
            cout << " will try to flip image horizontally to detect assymetric objects\n";
        }
        else if( argv[i][0] == '-' )
        {
            cerr << "WARNING: Unknown option %s" << argv[i] << endl;
        }
        else
            inputName.assign( argv[i] );
    }
    
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }
    
    if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
    {
        capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
        int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0' ;
        if(!capture) cout << "Capture from CAM " <<  c << " didn't work" << endl;
    }
    else if( inputName.size() )
    {
        image = imread( inputName, 1 );
        if( image.empty() )
        {
            capture = cvCaptureFromAVI( inputName.c_str() );
            
            if(!capture) cout << "Capture from AVI didn't work" << endl;
        }
    }
    else
    {
        image = imread( "../data/lena.jpg", 1 );
        if(image.empty()) cout << "Couldn't read ../data/lena.jpg" << endl;
    }
    
    cvNamedWindow( "result", 1 );
    
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 600 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
    cvSetCaptureProperty( capture, CV_CAP_PROP_FPS, 48);
    if( capture )
    {
        cout << "In capture ..." << endl;
        for(;;)
        {
            IplImage* iplImg = cvQueryFrame( capture );
            frame = cv::cvarrToMat(iplImg);
            if( frame.empty() )
                break;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );
            
            detectAndDraw( frameCopy, cascade, nestedCascade, scale, tryflip );
            
            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }
        
        waitKey(0);
        
    _cleanup_:
        cvReleaseCapture( &capture );
    }
    else
    {
        cout << "In image read" << endl;
        if( !image.empty() )
        {
            detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
            waitKey(0);
        }
        else if( !inputName.empty() )
        {
            /* assume it is a text file containing the
             list of the image filenames to be processed - one per line */
            FILE* f = fopen( inputName.c_str(), "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
                    int len = (int)strlen(buf), c;
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
                    cout << "file " << buf << endl;
                    image = imread( buf, 1 );
                    if( !image.empty() )
                    {
                        detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
                        c = waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                    else
                    {
                        cerr << "Aw snap, couldn't read image " << buf << endl;
                    }
                }
                fclose(f);
            }
        }
    }
    
    cvDestroyWindow("result");
    
    return 0;
}

int a = 0;

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                   CascadeClassifier& nestedCascade,
                   double scale, bool tryflip )
{
    
    /*if((a++) % 2 ==0) {
        cv::imshow( "result", img );
        return;
    }*/
    
    int i = 0;
    double t = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
    
    cvtColor( img, gray, COLOR_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );
    
    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
                             1.1, 2, 0
                             //|CASCADE_FIND_BIGGEST_OBJECT
                             |CASCADE_DO_ROUGH_SEARCH
                             |CASCADE_SCALE_IMAGE
                             ,
                             Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 |CASCADE_DO_ROUGH_SEARCH
                                 |CASCADE_SCALE_IMAGE
                                 ,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)cvGetTickCount() - t;

    //printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        
        double aspect_ratio = (double)r->width/r->height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r -> x + r -> width * 0.5) * scale);
            center.y = cvRound((r -> y + r -> height * 0.5) * scale);
            radius = cvRound((r -> width + r -> height) * 0.25 * scale);
            //circle( img, center, radius, color, 3, 8, 0 );
            
            img = putMask(img,center,Size( faces[i].width, faces[i].height));
        }
    }
    overlayImage(img, sub, out, Point2i(0,0));
    addWeighted(img, 0.5, sub, 0.5, 0.0, out);
    
    cv::imshow( "result", out );
}

Mat putMask2(Mat src,Point center,Size face_size)
{
    Mat mask1,src1;
    resize(mask,mask1,face_size);
    
    Rect roi(center.x - face_size.width/2, center.y - face_size.width/2, face_size.width, face_size.width);
    src(roi).copyTo(src1);
    
    Mat mask2,m,m1;
    cvtColor(mask1,mask2,CV_BGR2GRAY);
    threshold(mask2,mask2,230,255,CV_THRESH_BINARY_INV);
    
    vector<Mat> maskChannels(3),result_mask(3);
    split(mask1, maskChannels);
    bitwise_and(maskChannels[0],mask2,result_mask[0]);
    bitwise_and(maskChannels[1],mask2,result_mask[1]);
    bitwise_and(maskChannels[2],mask2,result_mask[2]);
    merge(result_mask,m );
    
    mask2 = 255 - mask2;
    vector<Mat> srcChannels(3);
    split(src1, srcChannels);
    bitwise_and(srcChannels[0],mask2,result_mask[0]);
    bitwise_and(srcChannels[1],mask2,result_mask[1]);
    bitwise_and(srcChannels[2],mask2,result_mask[2]);
    merge(result_mask,m1 );
    
    addWeighted(m,1,m1,1,0,m1);
    
    m1.copyTo(src(roi));
    
    return src;
}


Mat putMask(Mat src,Point center,Size face_size)
{
    Rect roi(center.x - face_size.width / 2, center.y - face_size.height / 2, face_size.width, face_size.height);
    
    roi.width = roi.width >= 600 ? 640 - 1 : abs(roi.width);
    roi.height = roi.height >= 480 ? 480 - 1 : abs(roi.height);
    
    cv::Mat tgt = src(roi);
    cv::Mat dst = src(roi);
    
    cv::blur(tgt, dst, cv::Size(40,40));
    dst.copyTo(src(roi));
    /*
    Mat mask1,src1;
    //resize(mask,mask1,face_size);
    mask1 = mask.clone();
    
    // ROI selection
    Rect roi(center.x - face_size.width/2, center.y - face_size.width/2, face_size.width, face_size.width);
    src(roi).copyTo(src1);
    
    // to make the white region transparent
    Mat mask2,m,m1;
    cvtColor(mask1,mask2,CV_BGR2GRAY);
    threshold(mask2,mask2,230,255,CV_THRESH_BINARY_INV);
    
    vector<Mat> maskChannels(3),result_mask(3);
    split(mask1, maskChannels);
    bitwise_and(maskChannels[0],mask2,result_mask[0]);
    bitwise_and(maskChannels[1],mask2,result_mask[1]);
    bitwise_and(maskChannels[2],mask2,result_mask[2]);
    merge(result_mask,m );         //    imshow("m",m);
    
    mask2 = 255 - mask2;
    vector<Mat> srcChannels(3);
    split(src1, srcChannels);
    bitwise_and(srcChannels[0],mask2,result_mask[0]);
    bitwise_and(srcChannels[1],mask2,result_mask[1]);
    bitwise_and(srcChannels[2],mask2,result_mask[2]);
    merge(result_mask,m1 );        //    imshow("m1",m1);
    
    addWeighted(m,1,m1,1,0,m1);    //    imshow("m2",m1);
    
    m1.copyTo(src(roi));
    */
    return src;
}


void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                  cv::Mat &output, cv::Point2i location)
{
    background.copyTo(output);
    
    
    // start at the row indicated by location, or at row 0 if location.y is negative.
    for(int y = std::max(location.y , 0); y < background.rows; ++y)
    {
        int fY = y - location.y; // because of the translation
        
        // we are done of we have processed all rows of the foreground image.
        if(fY >= foreground.rows)
        break;
        
        // start at the column indicated by location,
        
        // or at column 0 if location.x is negative.
        for(int x = std::max(location.x, 0); x < background.cols; ++x)
        {
            int fX = x - location.x; // because of the translation.
            
            // we are done with this row if the column is outside of the foreground image.
            if(fX >= foreground.cols)
            break;
            
            // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
            double opacity =
            ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])
            
            / 255.;
            
            
            // and now combine the background and foreground pixel, using the opacity,
            
            // but only if opacity > 0.
            for(int c = 0; opacity > 0 && c < output.channels(); ++c)
            {
                unsigned char foregroundPx =
                foreground.data[fY * foreground.step + fX * foreground.channels() + c];
                unsigned char backgroundPx =
                background.data[y * background.step + x * background.channels() + c];
                output.data[y*output.step + output.channels()*x + c] =
                backgroundPx * (1.-opacity) + foregroundPx * opacity;
            }
        }
    }
}