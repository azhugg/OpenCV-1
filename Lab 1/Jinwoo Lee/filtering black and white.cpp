#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <algorithm>

using namespace std;
using namespace cv;

void toBNW(Mat&);
void sprinkleSaltAndPepper(Mat&, float density);
void minFiltering(Mat&);
void maxFiltering(Mat&);
void midFiltering(Mat&);
void medianFiltering(Mat&);

Mat pic1, pic2;

int main()
{
    srand(time(0));
    pic1 = imread( "../../../choiNme.jpg", 1 );
    pic2.create(pic1.rows,pic1.cols, CV_8UC3);
    pic2=pic1.clone();
    
    toBNW(pic2);
    namedWindow( "My input", CV_WINDOW_AUTOSIZE );
    imshow( "My input", pic2);
    
    sprinkleSaltAndPepper(pic2, 0.1f);
    namedWindow( "My input1.5", CV_WINDOW_AUTOSIZE );
    imshow( "My input1.5", pic2);
    
    //minFiltering(pic2);
    //minFiltering(pic2);
    //maxFiltering(pic2);
    //maxFiltering(pic2);
    //midFiltering(pic2);
    //midFiltering(pic2);
    //medianFiltering(pic2);
    medianFiltering(pic2);
    
    
    namedWindow("My input2", CV_WINDOW_AUTOSIZE);
    imshow("My input2", pic2);
    //imwrite("../../../change.jpg", pic2);
    
    waitKey(0);
    return 0;
}

void toBNW(Mat& pic)
{
    int i, j, k;
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            int sum=0;
            Vec3b color=pic.at<Vec3b>(i, j);
            for(k=0;k<3;++k)
                sum+=color[k];
            for(k=0;k<3;++k)
                color[k]=sum/3;
            pic2.at<Vec3b>(i, j)=color;
        }
    }
}

void minFiltering(Mat& pic)//salt만 뿌리고 하면 꽤 복원된다(약간 뭉개짐). 하지만 pepper라면 검은 사각형이 많이 생긴다.
{
    int i, j, k, l;
    unsigned char minimum = 255;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            minimum=255;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        if(minimum > pic.at<Vec3b>(k, l)[0])
                            minimum=pic.at<Vec3b>(k, l)[0];
                    }
                }
            }
            impic.at<Vec3b>(i, j)[0]=minimum;
            impic.at<Vec3b>(i, j)[1]=minimum;
            impic.at<Vec3b>(i, j)[2]=minimum;
        }
    }
    
    pic=impic;
}

void maxFiltering(Mat& pic)//pepper만 뿌리고 하면 꽤 복원된다(약간 뭉개짐). 하지만 salt라면 하얀 사각형이 많이 생긴다.
{
    int i, j, k, l;
    unsigned char maximum = 0;
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
                        if(maximum < pic.at<Vec3b>(k, l)[0])
                            maximum=pic.at<Vec3b>(k, l)[0];
                    }
                }
            }
            impic.at<Vec3b>(i, j)[0]=maximum;
            impic.at<Vec3b>(i, j)[1]=maximum;
            impic.at<Vec3b>(i, j)[2]=maximum;
        }
    }
    
    pic=impic;
}

void medianFiltering(Mat& pic)//둘 다 뿌려도 꽤 복원된다(약간의 salt나 pepper가 남아있다).
{
    int i, j, k, l, gs;
    unsigned char arr[8];
    unsigned char med;
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
                        arr[gs++]=pic.at<Vec3b>(k, l)[0];
                    }
                }
            }
            sort(arr, arr+gs);
            med=arr[gs/2];
            impic.at<Vec3b>(i, j)[0]=med;
            impic.at<Vec3b>(i, j)[1]=med;
            impic.at<Vec3b>(i, j)[2]=med;
        }
    }
    
    pic=impic;
}

void midFiltering(Mat& pic)//안좋다. 회색의 네모가 많이 생긴다.
{
    int i, j, k, l;
    int mid, max, min;
    Mat impic;
    impic.create(pic.rows, pic.cols, CV_8UC3);
    
    for(i=0;i<pic.rows;++i)
    {
        for(j=0;j<pic.cols;++j)
        {
            max=0;
            min=255;
            for(k=i-1;k<=i+1;++k)
            {
                for(l=j-1;l<=j+1;++l)
                {
                    if(k>=0 && k<pic.rows && l>=0 && l<pic.cols && !(k==i && l==j))
                    {
                        if(max < pic.at<Vec3b>(k, l)[0])
                            max=pic.at<Vec3b>(k, l)[0];
                        if(min > pic.at<Vec3b>(k, l)[0])
                            min=pic.at<Vec3b>(k, l)[0];
                    }
                }
            }
            mid=max/2+min/2;
            impic.at<Vec3b>(i, j)[0]=mid;
            impic.at<Vec3b>(i, j)[1]=mid;
            impic.at<Vec3b>(i, j)[2]=mid;
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
/*이거 해결하면 좋겠다.
 void sprinkleSaltAndPepper(Mat& pic, float density)
 {
    int i, j;
    int saltAndPepperGS = pic.rows*pic.cols*density, randX, randY, r;
    Vec3b salt={0,0,0}, pepper={255,255,255};
     int **randPixel = 0;
     randPixel = new int*[pic.cols*2];
    
     for(int i=0;i<pic.cols*2 ;++i)
         randPixel[i] = new int[pic.rows];
     for(i=0;i<pic.rows;++i)
     {
         for(j=0;j<pic.cols;++j)
         {
             randPixel[i][j*2]=i;
             randPixel[i][j*2+1]=j;
             
//printf("%d %d\n", i, j);
         }
     }
     
    for(i=0;i<saltAndPepperGS;++i)
    {
        r=rand()%(pic.rows*pic.cols-i);
        randX=rand()%pic.rows;
        randY=rand()/pic.cols;
printf("%d %d %d\n", randPixel[randX][randY*2], randX, randY);
        swap(randPixel[randX][randY*2], randPixel[(pic.rows*pic.cols-i)%pic.rows][(pic.rows*pic.cols-i)/pic.rows*2]);
        swap(randPixel[randX][randY*2+1], randPixel[(pic.rows*pic.cols-i)%pic.rows][(pic.rows*pic.cols-i)/pic.rows*2+1]);
        if(rand()%2)
            pic.at<Vec3b>(randPixel[(pic.rows*pic.cols-i)%pic.rows][(pic.rows*pic.cols-i)/pic.rows*2+1], randPixel[(pic.rows*pic.cols-i)%pic.rows][(pic.rows*pic.cols-i)/pic.rows*2]) = salt;
        else
            pic.at<Vec3b>(randPixel[(pic.rows*pic.cols-i)%pic.rows][(pic.rows*pic.cols-i)/pic.rows*2+1], randPixel[(pic.rows*pic.cols-i)%pic.rows][(pic.rows*pic.cols-i)/pic.rows*2]) = pepper;
    }
     
     delete[] randPixel;
 }



*/



















