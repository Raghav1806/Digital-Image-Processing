#include <opencv2/opencv.hpp>
using namespace cv;

// values of 'a' and 'b' are user defined, and are determined by hit and trial method
static const float a(0.073235f);
static const float b(0.176765f);

// K is the gaussian kernel
static const Mat K = (Mat_<float>(3, 3) << a, b, a, b, 0.0f, b, a, b, a);

void inpaint(const Mat &src, const Mat &mask, const Mat kernel, Mat &dst, int maxNumOfIter){

	// checking whether mask, source, and gaussian kernel follow certain conditions or not
	assert(src.type() == mask.type() && mask.type() == CV_8UC3);
	assert(src.size() == mask.size());
	assert(kernel.type() == CV_32F);
	
	// fill in the missing region with input's average color
	auto avgColor = sum(src)/(src.cols*src.rows);
	
	Mat avgColorMat(1, 1, CV_8UC3);
	avgColorMat.at<Vec3b>(0, 0) = Vec3b(avgColor[0], avgColor[1], avgColor[2]);
	resize(avgColorMat, avgColorMat, src.size(), 0.0, 0.0, INTER_NEAREST);
	
	Mat result = (mask/255).mul(src) + (1-mask/255).mul(avgColorMat);
	
	// convolution between kernel and border pixels
	int bSize = K.cols/2;
	Mat kernel3ch, inWithBorder;
	
	result.convertTo(result, CV_32FC3);
	cvtColor(kernel, kernel3ch, COLOR_GRAY2BGR);
	copyMakeBorder(result, inWithBorder, bSize, bSize, bSize, bSize, BORDER_REPLICATE);

	Mat resInWithBorder = Mat(inWithBorder, Rect(bSize, bSize, result.cols, result.rows));
	
	const int ch = result.channels();
	
	// repeating the process by user defined number of iterations
	for(int itr = 0; itr < maxNumOfIter; itr++){
		copyMakeBorder(result, inWithBorder, bSize, bSize, bSize, bSize, BORDER_REPLICATE);
		
		for(int r = 0; r < result.rows; r++){
			const uchar *pMask = mask.ptr(r);
			float *pRes = result.ptr<float>(r);
			
			for(int c = 0; c < result.cols; c++){
				if(pMask[ch*c] == 0){
					Rect rectRoi(c, r, K.cols, K.rows);
					Mat roi(inWithBorder, rectRoi);
					
					auto sum = cv::sum(kernel3ch.mul(roi));
					
					pRes[ch*c + 0] = sum[0];
					pRes[ch*c + 1] = sum[1];
					pRes[ch*c + 2] = sum[2];
				}
			}
		}
	}
	result.convertTo(dst, CV_8UC3);
}
