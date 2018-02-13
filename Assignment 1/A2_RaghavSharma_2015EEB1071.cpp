/*

Digital Image Processing (CSL 461) Assignment 1:
--------------------------------------------------------------------------------
Aim:
--------------------------------------------------------------------------------
Implement the following image enhancement operations/methods:
(1) Image Resize				- (Not Implemented)
(2) Affine Transformations			- (Not Implemented)
(3) Image negatives
(4) Log transformations
(5) Power-law/Gamma Transformations
(6) Piecewise linear transformation functions
(7) Bitplane slicing
(8) Image reconstruction given some tie points	- (Not Implemented)
(9) Histogram equalization
(10) Adaptive histogram equalization
(11) Histogram matching
---------------------------------------------------------------------------------
Developed By:
Raghav Sharma
2015EEB1071
---------------------------------------------------------------------------------
Submitted On: 06/02/2018
---------------------------------------------------------------------------------
References:
---------------------------------------------------------------------------------
(1) http://opencv-cpp.blogspot.in
(2) https://www.opencv-srf.com/2017/11/load-and-display-image.html
(3) http://www.aishack.in/tutorials/opencv/
(4) https://docs.opencv.org/3.4.0/d7/da8/tutorial_table_of_content_imgproc.html
(5) https://stackoverflow.com/

*/

#include <iostream>
#include <cmath>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

// contains the cumulative probability of occurrence of each intensity value from (0-255)
float histogramProbability[256];

void initialiseHistogram(){
	int i;
	for(i = 0; i < 256; i++){
		histogramProbability[i] = 0;
	}
}

int main(){
	Mat img, img0, img1, img2, img3, img4, img5, img6, img7;
	int height, width, step, channels, i, j, k, l, m, X, totalPixels;
	
	uchar *data, *data0, *data1, *data2, *data3, *data4, *data5, *data6, *data7;
	
	// image negative
	img = imread("negativeFilter.jpeg");
	
	if(!img.data){
		cout << "Could not find the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height 		= img.rows;
	width 		= img.cols;
	channels 	= img.channels();
	step		= width*channels;
	data 		= (uchar *)img.data;
	
	// finding the negative of image
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = 255 - data[i*step + j*channels + k];
			}
		}
	}
	
	// create a window
	namedWindow("Negative Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Negative Image", 250, 250);
	// show the modified image
	imshow("Negative Image", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Negative Image");
		

	// log transformation
	img = imread("logTransformation.jpeg");
	if(!img.data){
		cout << "Could not find the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height 		= img.rows;
	width 		= img.cols;
	channels 	= img.channels();
	step 		= width*channels;
	data 		= (uchar *)img.data;
	
	// logarithmic transformation of an image
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = 45*log(data[i*step + j*channels + k] + 1);
			}
		}
	}
	
	// create a window
	namedWindow("Logarithmic Transformation", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Logarithmic Transformation", 250, 250);
	// show the modified image
	imshow("Logarithmic Transformation", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Logarithmic Transformation");
	
	
	// power law/gamma transformation
	img = imread("gammaTransformation.jpeg");
	
	if(!img.data){
		cout << "Could not find the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height		= img.rows;
	width 		= img.cols;
	channels	= img.channels();
	step 		= width*channels;
	data 		= (uchar *)img.data;
	
	// gamma transformation of image
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = 8*pow(data[i*step + j*channels + k], 0.62);
			}
		}
	}
	
	
	// create a window
	namedWindow("Gamma Transformation", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Gamma Transformation", 250, 250);
	// show the modified image
	imshow("Gamma Transformation", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Gamma Transformation");
	
	
	// contrast stretching
	img = imread("piecewiseLinear.jpeg");
	
	if(!img.data){
		cout << "Could not find the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height 		= img.rows;
	width 		= img.cols;
	channels	= img.channels();
	step 		= width*channels;
	data 		= (uchar *)img.data;
	
	
	// contrast stretching of an image
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				if(data[i*step + j*channels + k] < 85){
					data[i*step + j*channels + k] = 0;
				}
				
				else if(85 <= data[i*step + j*channels + k] && data[i*step + j*channels + k] < 115){
					data[i*step + j*channels + k] = data[i*step + j*channels + k]/2;
				}
				
				else if(115 <= data[i*step + j*channels + k] && data[i*step + j*channels + k] < 150){
					data[i*step + j*channels + k] = 1.5*data[i*step + j*channels + k];
				}
				
				
				
				else if(150 <= data[i*step + j*channels + k] && data[i*step + j*channels + k] <= 255){
					data[i*step + j*channels + k] = 255;
				}
			}
		}
	}
	
	
	// create a window
	namedWindow("Contrast Stretching", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Contrast Stretching", 250, 250);
	// show the modified image
	imshow("Contrast Stretching", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Contrast Stretching");
	
	
	
	
	// bit plane slicing
	
	img0 = imread("bitPlaneSlicing.jpeg");
	img1 = imread("bitPlaneSlicing.jpeg");
	img2 = imread("bitPlaneSlicing.jpeg");
	img3 = imread("bitPlaneSlicing.jpeg");
	img4 = imread("bitPlaneSlicing.jpeg");
	img5 = imread("bitPlaneSlicing.jpeg");
	img6 = imread("bitPlaneSlicing.jpeg");
	img7 = imread("bitPlaneSlicing.jpeg");
	
	if(!(img0.data && img1.data && img2.data && img3.data && img4.data && img5.data && img6.data && img7.data)){
		cout << "Could not load one of the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img0);
	// wait for a key
	waitKey(0);
	
	
	
	// image data for all of the images is identical
	height 		= img0.rows;
	width 		= img0.cols;
	channels	= img0.channels();
	step 		= width*channels;
	data0		= (uchar *)img0.data;
	data1 		= (uchar *)img1.data;
	data2 		= (uchar *)img2.data;
	data3 		= (uchar *)img3.data;
	data4 		= (uchar *)img4.data;
	data5 		= (uchar *)img5.data;
	data6 		= (uchar *)img6.data;
	data7		= (uchar *)img7.data;
	
	// analysing each bit-plane
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				X = data0[i*step + j*channels + k];	
				for(l = 0; l < 7; l++){
					m = X%2;
					X = X/2;
					if(l == 0){		
						if(m == 0){
							data0[i*step + j*channels + k] = 0;
						}
						else{
							data0[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 1){		
						if(m == 0){
							data1[i*step + j*channels + k] = 0;
						}
						else{
							data1[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 2){		
						if(m == 0){
							data2[i*step + j*channels + k] = 0;
						}
						else{
							data2[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 3){		
						if(m == 0){
							data3[i*step + j*channels + k] = 0;
						}
						else{
							data3[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 4){		
						if(m == 0){
							data4[i*step + j*channels + k] = 0;
						}
						else{
							data4[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 5){		
						if(m == 0){
							data5[i*step + j*channels + k] = 0;
						}
						else{
							data5[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 6){		
						if(m == 0){
							data6[i*step + j*channels + k] = 0;
						}
						else{
							data6[i*step + j*channels + k] = 255;
						}
					}
					
					if(l == 7){		
						if(m == 0){
							data7[i*step + j*channels + k] = 0;
						}
						else{
							data7[i*step + j*channels + k] = 255;
						}
					}
					
				}
			}
		}
	}
	
	// show different plane images
	
	// create a window
	namedWindow("Plane #0", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #0", 250, 250);
	// show the modified image
	imshow("Plane #0", img0);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #0");
	
	// create a window
	namedWindow("Plane #1", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #1", 250, 250);
	// show the modified image
	imshow("Plane #1", img1);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #1");
	
	// create a window
	namedWindow("Plane #2", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #2", 250, 250);
	// show the modified image
	imshow("Plane #2", img2);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #2");
	
	// create a window
	namedWindow("Plane #3", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #3", 250, 250);
	// show the modified image
	imshow("Plane #3", img3);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #3");
	
	// create a window
	namedWindow("Plane #4", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #4", 250, 250);
	// show the modified image
	imshow("Plane #4", img4);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #4");
	
	// create a window
	namedWindow("Plane #5", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #5", 250, 250);
	// show the modified image
	imshow("Plane #5", img5);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #5");
	
	// create a window
	namedWindow("Plane #6", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #6", 250, 250);
	// show the modified image
	imshow("Plane #6", img6);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #6");
	
	// create a window
	namedWindow("Plane #7", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Plane #7", 250, 250);
	// show the modified image
	imshow("Plane #7", img7);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Plane #7");
	
	
	// histogram equilisation
	
	// initialise the value of histogram
	initialiseHistogram();
	
	img = imread("histogramEquilisation.jpeg");
	
	if(!img.data){
		cout << "Could not fing the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height 		= img.rows;
	width 		= img.cols;
	channels	= img.channels();
	totalPixels 	= height*width*channels;
	step 		= width*channels;
	data		= (uchar *)img.data;
	
	
	// going through each pixel
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				histogramProbability[data[i*step + j*channels + k]]++;
			}
		}
	}
	
	// finding the probability of occurrence of each intensity value
	for(i = 0; i < 256; i++){
		histogramProbability[i] = histogramProbability[i]/totalPixels;
	}
	
	// finding the cumulative probability of each pixel value
	for(i = 1; i < 256; i++){
		histogramProbability[i] += histogramProbability[i-1];
	}
	
	
	// modifying intensity of each pixel
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = (255)*(histogramProbability[data[i*step + j*channels + k]]);
			}
		}
	}
	
	// create a window
	namedWindow("Histogram Equilisation", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Histogram Equilisation", 250, 250);
	// show the modified image
	imshow("Histogram Equilisation", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Histogram Equilisation");
	
	
	// adaptive histogram equilisation
	
	img = imread("adaptiveHistogramEquilisation.jpeg");
	
	if(!img.data){
		cout << "Could not find the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height 		= img.rows;
	width 		= img.cols;
	channels	= img.channels();
	totalPixels 	= height*width*channels;
	step 		= width*channels;
	data		= (uchar *)img.data;
	
	// first quadrant
	initialiseHistogram();
	for(i = 0; i < height/2; i++){
		for(j = 0; j < width/2; j++){
			for(k = 0; k < channels; k++){
				histogramProbability[data[i*step + j*channels + k]]++;
			}
		}
	}
	
	// finding the probability of occurrence of each intensity value
	for(i = 0; i < 256; i++){
		histogramProbability[i] = histogramProbability[i]/totalPixels;
	}
	
	// finding the cumulative probability of each pixel value
	for(i = 1; i < 256; i++){
		histogramProbability[i] += histogramProbability[i-1];
	}
	
	// modifying intensity of each pixel in first quadrant
	for(i = 0; i <= height/2; i++){
		for(j = 0; j <= width/2; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = (255)*(histogramProbability[data[i*step + j*channels + k]]);
			}
		}
	}
	
	
	
	
	// second quadrant
	initialiseHistogram();
	for(i = 0; i <= height/2; i++){
		for(j = width/2+1; j < width; j++){
			for(k = 0; k < channels; k++){
				histogramProbability[data[i*step + j*channels + k]]++;
			}
		}
	}
	
	// finding the probability of occurrence of each intensity value
	for(i = 0; i < 256; i++){
		histogramProbability[i] = histogramProbability[i]/totalPixels;
	}
	
	// finding the cumulative probability of each pixel value
	for(i = 1; i < 256; i++){
		histogramProbability[i] += histogramProbability[i-1];
	}
	
	// modifying intensity of each pixel in first quadrant
	for(i = 0; i <= height/2; i++){
		for(j = width/2+1; j < width; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = (255)*(histogramProbability[data[i*step + j*channels + k]]);
			}
		}
	}
	
	
	
	// third quadrant
	initialiseHistogram();
	for(i = height/2+1; i < height; i++){
		for(j = 0; j <= width/2; j++){
			for(k = 0; k < channels; k++){
				histogramProbability[data[i*step + j*channels + k]]++;
			}
		}
	}
	
	// finding the probability of occurrence of each intensity value
	for(i = 0; i < 256; i++){
		histogramProbability[i] = histogramProbability[i]/totalPixels;
	}
	
	// finding the cumulative probability of each pixel value
	for(i = 1; i < 256; i++){
		histogramProbability[i] += histogramProbability[i-1];
	}
	
	// modifying intensity of each pixel in first quadrant
	for(i = height/2+1; i < height; i++){
		for(j = 0; j <= width/2; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = (255)*(histogramProbability[data[i*step + j*channels + k]]);
			}
		}
	}
	
	// fourth quadrant
	initialiseHistogram();
	for(i = height/2; i < height; i++){
		for(j = width/2+1; j < width; j++){
			for(k = 0; k < channels; k++){
				histogramProbability[data[i*step + j*channels + k]]++;
			}
		}
	}
	
	// finding the probability of occurrence of each intensity value
	for(i = 0; i < 256; i++){
		histogramProbability[i] = histogramProbability[i]/totalPixels;
	}
	
	// finding the cumulative probability of each pixel value
	for(i = 1; i < 256; i++){
		histogramProbability[i] += histogramProbability[i-1];
	}
	
	// modifying intensity of each pixel in first quadrant
	for(i = height/2+1; i < height; i++){
		for(j = width/2+1; j < width; j++){
			for(k = 0; k < channels; k++){
				data[i*step + j*channels + k] = (255)*(histogramProbability[data[i*step + j*channels + k]]);
			}
		}
	}
	
	// create a window
	namedWindow("Adaptive Histogram Equilisation", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Adaptive Histogram Equilisation", 250, 250);
	// show the modified image
	imshow("Adaptive Histogram Equilisation", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Adaptive Histogram Equilisation");
	
	
	
	
	// histogram matching
	// initialise the histogram
	initialiseHistogram();
	
	img = imread("histogramMatching.jpeg");
	
	if(!img.data){
		cout << "Could not find the image!\n";
		return -1;
	}
	
	// create a window
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Original Image", 250, 250);
	// show the modified image
	imshow("Original Image", img);
	// wait for a key
	waitKey(0);
	
	// get the image data
	height 		= img.rows;
	width 		= img.cols;
	channels	= img.channels();
	totalPixels 	= height*width*channels;
	step 		= width*channels;
	data		= (uchar *)img.data;
	
	// going through each pixel
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				histogramProbability[data[i*step + j*channels + k]]++;
			}
		}
	}
	
	// finding the probability of occurrence of each intensity value
	for(i = 0; i < 256; i++){
		histogramProbability[i] = histogramProbability[i]/totalPixels;
	}
	
	// finding the cumulative probability of each pixel value
	for(i = 1; i < 256; i++){
		histogramProbability[i] += histogramProbability[i-1];
	}
	
	
	// modifying intensity of each pixel
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			for(k = 0; k < channels; k++){
				if(0 <= data[i*step + j*channels + k] && data[i*step + j*channels + k] < 85){
					data[i*step + j*channels + k] = (85)*(histogramProbability[data[i*step + j*channels + k]]);
				}
				else if(85 <= data[i*step + j*channels + k] && data[i*step + j*channels + k] < 170){
					data[i*step + j*channels + k] = (255)*(histogramProbability[data[i*step + j*channels + k]]);
				}
				else if(170 <= data[i*step + j*channels + k] && data[i*step + j*channels + k] < 256){
					data[i*step + j*channels + k] = (310)*(histogramProbability[data[i*step + j*channels + k]]);
				}
			}
		}
	}
	
	
	// create a window
	namedWindow("Histogram Matching", WINDOW_AUTOSIZE);
	// move the window to desired position
	moveWindow("Histogram Matching", 250, 250);
	// show the modified image
	imshow("Histogram Matching", img);
	// wait for a key
	waitKey(0);
	// terminate the window
	cvDestroyWindow("Histogram Matching");
	
	return 0;
}

