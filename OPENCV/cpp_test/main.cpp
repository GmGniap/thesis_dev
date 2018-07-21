#include<highgui.h>
#include<iostream>
#include<cv.h>
 
int main( int argc, char** argv ) {
  
  Mat image;
  image = imread("sample.jpeg",CV_LOAD_IMAGE_COLOR);
  
  if(! image.data ) {
      cout <<  "Could not open or find the image" << endl ;
      return -1;
    }
  
  namedWindow( "Display window",CV_WINDOW_AUTOSIZE );
  imshow( "Display window", image );
  
  waitKey(0);
  return 0;
}
