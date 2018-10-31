#include<opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;


using namespace std;

#if 1
void hsv_convert_white(Mat image_orig)
{
	Mat hsvImg;
	Mat imgThresholded;
	//convert to hsv img first
	cvtColor(image_orig, hsvImg, COLOR_BGR2HSV);
	
	//enum colorType{Red = 0, Green, Blue, ColorButt};

    const Scalar hsvWhiteLo( 0,  0,  221);
    const Scalar hsvWhiteHi(180, 30, 255);

    vector<Scalar> hsvLo{hsvWhiteLo};
    vector<Scalar> hsvHi{hsvWhiteHi};

    vector<String> textColor{"W"};
	 // 查找指定范围内的颜色
    inRange(hsvImg, hsvLo[0], hsvHi[0], imgThresholded);
        // 转换成二值图
    threshold(imgThresholded, imgThresholded, 1, 255, THRESH_BINARY);
	copyMakeBorder(imgThresholded, imag_1, 1, 1, 1, 1, BORDER_CONSTANT, 0);
    vector<vector<Point> > contours0;
    vector<Vec4i> hierarchy;
    findContours(imag_1, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
}
#endif

int main(int argc, char** argv)
{
	if (argc != 2)
    {
        cout << "Call this program like this:" << endl;
        cout << "./demo1 xxx.jpg" << endl;
        return 0;
    }
	
	Mat origImage = imread(argv[1] ,0);
	
	Mat srcImage;
	origImage.copyTo( srcImage );
	
	if( !srcImage.data )
	{
		cout << "读取失败" << endl;
		return 0;
	}
	//imshow( "原始图" , srcImage );
	//waitKey();

	// threshold只针对二值化图像
	threshold( srcImage , srcImage , 0.0 , 255.0 ,CV_THRESH_BINARY|CV_THRESH_OTSU ); 

	vector<vector<Point> >  contours;
	vector<Vec4i> hierarchy;

	findContours( srcImage , contours , hierarchy , CV_RETR_CCOMP , CV_CHAIN_APPROX_SIMPLE );

	vector<Point> maxcontours ;   //最大轮廓
	double maxArea = 0;

//	vector<vector<Point>>::const_iterator itContours = contours.begin();
	int maxAreaIdx = 0; 
	for( size_t i = 0; i < contours.size();i++ )
	{
		double area = contourArea( contours[i] );
		if( area > maxArea )
		{
			maxArea = area;
			maxcontours = contours[i];
			maxAreaIdx = i;
		}
	}
	
	cout<<"max area id = "<< maxAreaIdx <<endl;
	
	//color max contour red
	
	
	Scalar color = CV_RGB( 28, 68, 227);
	
	drawContours(origImage, contours, maxAreaIdx, color, CV_FILLED, 8, hierarchy);
    imshow( "biggest regions contour" , origImage );
	
	/*
	
	
	Rect maxRect = boundingRect( maxcontours );  //查找矩形框

	Mat result1 , result2;

	srcImage.copyTo( result1 );
	srcImage.copyTo( result2 );

	for( size_t i = 0; i < contours.size(); i++ )
	{
		Rect r = boundingRect( contours[i] );
		rectangle( result1 , r , Scalar( 255 ) );
	}
	imshow( "all regions" , result1 );
	waitKey();

	rectangle( result2 , maxRect , Scalar( 255 ) );
	imshow( "largest region" , result2 );
	*/
	
	waitKey();
}