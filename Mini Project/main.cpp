#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "fastDigitalImageInpainting.hpp"

int main(){
	Mat src = imread("image.png");
	Mat mask = imread("mask.png");
	Mat res;
	
	inpaint(src, mask, K, res, 500);
	
	imshow("Inpainting Result", res);
	waitKey();
	
	return 0;
}
