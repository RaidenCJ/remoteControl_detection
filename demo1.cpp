#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
void thresh_callback(int, void* );
int main( int argc, char** argv )
{
  src = imread( argv[1], IMREAD_COLOR );
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );//模糊图片以摆脱噪点

  //显示原图片
  const char* source_window = "Source";
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );
	
  createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );
  waitKey(0);
  return(0);
}


void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
  //对于每个找到的轮廓，我们现在将逼近逼近具有精度±3的多边形，并表示曲线必须关闭。
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
  {
    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    minEnclosingCircle( contours_poly[i], center[i], radius[i] );
  }
  Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
  {
	//对于每一个轮廓,选择随机颜色
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    drawContours( drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    //rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
    //circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
  }
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}