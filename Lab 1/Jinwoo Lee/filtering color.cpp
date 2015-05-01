#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void sprinkleSaltAndPepper(Mat&, float density);
void minFiltering(Mat&);
void maxFiltering(Mat&);
void midFiltering(Mat&);
void medianFiltering(Mat&);
void julSaFiltering(Mat&);
void medianFiltering2(Mat&);

Mat pic1, pic2;

int main()
{
    srand((unsigned)time(0));
    pic1 = imread( "../../../choiNme.jpg", 1 );
    pic2.create(pic1.rows,pic1.cols, CV_8UC3);
    pic2=pic1.clone();
    
    namedWindow( "My input", CV_WINDOW_AUTOSIZE );
    imshow( "My input", pic2);
    
    sprinkleSaltAndPepper(pic2, 0.99f);
    
    namedWindow( "My input1.5", CV_WINDOW_AUTOSIZE );
    imshow( "My input1.5", pic2);
    
    //minFiltering(pic2);
    //minFiltering(pic2);
    //maxFiltering(pic2);
    //maxFiltering(pic2);
    //midFiltering(pic2);
    //midFiltering(pic2);
    //medianFiltering(pic2);
    //medianFiltering(pic2);
    //julSaFiltering(pic2);
    medianFiltering2(pic2);medianFiltering2(pic2);
    
    
    namedWindow("My input2", CV_WINDOW_AUTOSIZE);
    imshow("My input2", pic2);
    //imwrite("../../../change.jpg", pic2);
    
    waitKey(0);
    return 0;
}

void minFiltering(Mat& pic)//salt만 뿌리고 하면 꽤 복원된다(약간 뭉개짐). 하지만 pepper라면 검은 사각형이 많이 생긴다.
{
    int i, j, k, l;
    int minimum, wherei, wherej;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            minimum=255*3;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        int sum=(int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2];
                        if(minimum > sum)
                        {
                            minimum=sum;
                            wherei=k;
                            wherej=l;
                        }
                    }
                }
            }
            impic.at<Vec3b>(i, j)=pic.at<Vec3b>(wherei, wherej);
        }
    }
    
    pic=impic;
}

void maxFiltering(Mat& pic)//pepper만 뿌리고 하면 꽤 복원된다(약간 뭉개짐). 하지만 salt라면 하얀 사각형이 많이 생긴다.
{
    int i, j, k, l;
    int maximum = 0, wherei, wherej;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            maximum=0;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        int sum=(int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2];
                        if(maximum < sum)
                        {
                            maximum=sum;
                            wherei=k;
                            wherej=l;
                        }
                    }
                }
            }
            impic.at<Vec3b>(i, j)=pic.at<Vec3b>(wherei, wherej);
        }
    }
    
    pic=impic;
}

void medianFiltering(Mat& pic)//둘 다 뿌려도 꽤 복원된다(약간의 salt나 pepper가 남아있다).
{
    int i, j, k, l, gs;
    int arr[8];
    int med;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            gs=0;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        arr[gs++] = (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2];
                    }
                }
            }
            sort(arr, arr+gs);
            med=arr[gs/2];
            
            for(k=i-1;k<=i+1;++k)//중간값의 위치 찾아서 impic에 집어넣기
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        if(med == (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2])
                            impic.at<Vec3b>(i, j)=pic.at<Vec3b>(k, l);
                    }
                }
            }//spin~~~!!
            
        }
    }
    
    pic=impic;
}

void midFiltering(Mat& pic)//안좋다. 회색의 네모가 많이 생긴다.
{
    int i, j, k, l;
    int maximum, minimum, whereMaxi, whereMaxj, whereMini, whereMinj;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            maximum=0;
            minimum=255*3;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        int sum=(int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2];
                        if(maximum < sum)
                        {
                            maximum=sum;
                            whereMaxi=k;
                            whereMaxj=l;
                        }
                        if(minimum > sum)
                        {
                            minimum=sum;
                            whereMini=k;
                            whereMinj=l;
                        }
                    }
                }
            }
            impic.at<Vec3b>(i, j)[0] = pic.at<Vec3b>(whereMaxi, whereMaxj)[0]/2+pic.at<Vec3b>(whereMini, whereMinj)[0]/2;
            impic.at<Vec3b>(i, j)[1] = pic.at<Vec3b>(whereMaxi, whereMaxj)[1]/2+pic.at<Vec3b>(whereMini, whereMinj)[1]/2;
            impic.at<Vec3b>(i, j)[2] = pic.at<Vec3b>(whereMaxi, whereMaxj)[2]/2+pic.at<Vec3b>(whereMini, whereMinj)[2]/2;
        }
    }
    
    pic=impic;
}


void julSaFiltering(Mat& pic)//둘 다 뿌려도 꽤 복원된다(약간의 salt나 pepper가 남아있다).
{
    int i, j, k, l, gs;
    int arr[8], rgb[3]={0};
    int minimum, maximum;
    bool sw=false;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            gs=0;
            rgb[0]=rgb[1]=rgb[2]=0;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        arr[gs++] = (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2];
                        rgb[0]+=pic.at<Vec3b>(k, l)[0];
                        rgb[1]+=pic.at<Vec3b>(k, l)[1];
                        rgb[2]+=pic.at<Vec3b>(k, l)[2];
                    }
                }
            }
            sort(arr, arr+gs);
            
            minimum=arr[0];
            maximum=arr[gs-1];
            
            sw=false;
            for(k=i-1;k<=i+1;++k)//최소빼기
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        if(minimum == (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2])
                        {
                            rgb[0]-=pic.at<Vec3b>(k, l)[0];
                            rgb[1]-=pic.at<Vec3b>(k, l)[1];
                            rgb[2]-=pic.at<Vec3b>(k, l)[2];
                            sw=true;
                            break;
                        }
                    }
                }
                if(sw) break;
            }
            sw=false;
            for(k=i-1;k<=i+1;++k)//최대빼기
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        if(maximum == (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2])
                        {
                            rgb[0]-=pic.at<Vec3b>(k, l)[0];
                            rgb[1]-=pic.at<Vec3b>(k, l)[1];
                            rgb[2]-=pic.at<Vec3b>(k, l)[2];
                            sw=true;
                            break;
                        }
                    }
                }
                if(sw) break;
            }
            
            impic.at<Vec3b>(i, j)[0]=rgb[0]/(gs-2);
            impic.at<Vec3b>(i, j)[1]=rgb[1]/(gs-2);
            impic.at<Vec3b>(i, j)[2]=rgb[2]/(gs-2);
            
        }
    }
    
    pic=impic;
}


void medianFiltering2(Mat& pic)//둘 다 뿌려도 꽤 복원된다(약간의 salt나 pepper가 남아있다).
{
    int i, j, k, l, gs;
    int arr[8];
    int med;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    impic=pic.clone();
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            gs=0;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols)
                    {
                        arr[gs++] = (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2];
                        //printf("%d ", arr[gs-1]);
                        if(arr[gs-1]==0) gs--;
                        else if(arr[gs-1]==255*3) gs--;
                    }
                }
            }
            sort(arr, arr+gs);
            med=arr[gs/2];
//printf("%d ", arr[gs/2]);
            
            for(k=i-1;k<=i+1;++k)//중간값의 위치 찾아서 impic에 집어넣기
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols)
                    {
                        if(med == (int)pic.at<Vec3b>(k, l)[0]+pic.at<Vec3b>(k, l)[1]+pic.at<Vec3b>(k, l)[2])
                            impic.at<Vec3b>(i, j)=pic.at<Vec3b>(k, l);//, printf("%d %d\n", i, j);
                    }
                }
            }//spin~~~!!
            
        }
    }
    
    pic=impic;
}



void sprinkleSaltAndPepper(Mat& pic, float density)
{
    int i, j;
    int saltAndPepperGS = pic.rows*pic.cols*density, randX, randY;
    Vec3b salt={0,0,0}, pepper={255,255,255};
    
    for(i=0;i<saltAndPepperGS;++i)
    {
        randX=rand()%(pic.cols);
        randY=rand()%(pic.rows);
        if(rand()%2)
            pic.at<Vec3b>(randY, randX) = salt;
        else
            pic.at<Vec3b>(randY, randX) = pepper;
    }
    
}



















