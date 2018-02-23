#include <iostream>
#include <fstream>
#include <stddef.h>
#include <vector>
#include <cmath>
#include <cstring>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

int image_numb = 1;
int numbTriangle1 = 0;
int sizePoints = 0;
int numbTotal = 0;

Mat image, morphImage;

ofstream outTriangle1, out, out1;

vector <Point2f> tiePointsImage1, tiePointsImage2, list1, list2, morphList;

int ** imageMatrix1;
int ** imageMatrix2;
int ** morphImageMatrix;

// This is used to draw a circle on the tie point
static void draw_point(Mat& img, Point2f fp, Scalar color){
	circle(img, fp, 2, color, CV_FILLED, CV_AA, 0);
}

// This is used to calculate area of triangle given 3 points of triangle
float area(int a1, int b1, int a2, int b2, int a3, int b3){
	return abs((a1*(b2 - b3) + a2*(b3 - b1) + a3*(b1 - b2)) / 2.0);
}

// This is used to solve 2 linear equations of the form a1x + b1y = k1 and a2x + b2y = k2
pair<float, float> solveEq(float a1, float b1, float k1, float a2, float b2, float k2){
	pair<float, float> ans;
	ans.first = (k1*b2 - k2*b1) / (b2*a1 - b1*a2);
	ans.second = (k1*a2 - k2*a1) / (b1*a2 - a1*b2);
	return ans;
}

// This is used to check whether a point (x,y) lies inside the triangle or not
bool checkIfIsInside(int a1, int b1, int a2, int b2, int a3, int b3, int a, int b){   

   float A = area (a1, b1, a2, b2, a3, b3);
 
   // Calculate area of triangle PBC 
   float A1 = area (a, b, a2, b2, a3, b3);
 
   // Calculate area of triangle APC   
   float A2 = area (a1, b1, a, b, a3, b3);
 
   // Calculate area of triangle ABP    
   float A3 = area (a1, b1, a2, b2, a, b);
   
   // Check if sum of A1, A2 and A3 is same as A 
   return (A == A1 + A2 + A3);
}

// This function is used to write the tie points to a text file
void onMouse(int event, int x, int y, int, void*){
	if(event != CV_EVENT_LBUTTONDOWN){
		return;
	}

	Point pt = Point(x, y);
	
	if(image_numb==1){
		out << pt.x <<" "<< pt.y <<"\n";
	}

	else{
		out1<< pt.x << " " << pt.y << "\n";
	}
}


/* 	

The following two functions, draw_delaunay(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color) and Triangle() for delaunay transformation is taken from:
	https://www.learnopencv.com/face-morph-using-opencv-cpp-python/
*/

// Draw delaunay triangles
static void draw_delaunay(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color){

	vector<Vec6f> triangleList;
	vector<Point> pt(3);
	
	subdiv.getTriangleList(triangleList);
	Size size = img.size();
	
	Rect rect(0, 0, size.width, size.height);

	for(size_t i = 0; i < triangleList.size(); i++){
		Vec6f t = triangleList[i];
		
		pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
		
		// Draw rectangles completely inside the image.
		if(rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])){

			line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
			line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
			line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);

			list1.push_back(Point2f(pt[0].x, pt[0].y));
			list1.push_back(Point2f(pt[1].x, pt[1].y));
			list1.push_back(Point2f(pt[2].x, pt[2].y));
			numbTotal += 3;
	
			for(int i = 0; i < sizePoints; i++){
					if(tiePointsImage1[i].x == pt[0].x && tiePointsImage1[i].y == pt[0].y){
						list2.push_back(tiePointsImage2[i]);
						outTriangle1 << i << " ";
					}
			}

			for(int i = 0; i < sizePoints; i++){
					if (tiePointsImage1[i].x == pt[1].x && tiePointsImage1[i].y == pt[1].y){
						list2.push_back(tiePointsImage2[i]);
						outTriangle1 << i << " ";
					}
				
			}


			for(int i = 0; i < sizePoints; i++){
					if(tiePointsImage1[i].x == pt[2].x && tiePointsImage1[i].y == pt[2].y){
						list2.push_back(tiePointsImage2[i]);
						outTriangle1 << i << endl;
					}
			}
				
			numbTriangle1++;
				
			
			
		}
	}
}

// Reads the tie points from the test file and passes a vector to draw delaunay for making triangles
void Triangle(){
	outTriangle1.open("Triangle1.txt");
	string win_delaunay = "Delaunay Triangulation";
	string win_delaunay1 = "Delaunay Triangulation";
	
	// Turn on animation while drawing triangles
	bool animate = true;
	
	image_numb = 1;
	
	// Define colors for drawing.
	Scalar delaunay_color(255, 255, 255), points_color(0, 0, 255);

	// Read in the image.
	Mat img = imread("image1.jpg");
	
	// Keep a copy of original image
	Mat img_orig = img.clone();

	// Rectangle to be used with Subdiv2D
	Size size = img.size();
	Rect rect(0, 0, size.width, size.height);

	// Create an instance of Subdiv2D
	Subdiv2D subdiv(rect);

	// Create a vector of points.
	vector<Point2f> points;

	// Read in the points from a text file
	ifstream ifs("File1.txt");
	int x, y;
	while(ifs >> x >> y){
		points.push_back(Point2f(x, y));
	}

	// Insert points into subdiv
	for(vector<Point2f>::iterator it = points.begin(); it != points.end(); it++){
		subdiv.insert(*it);
	}

	
	draw_delaunay(img, subdiv, delaunay_color);

	// Draw points
	for(vector<Point2f>::iterator it = points.begin(); it != points.end(); it++){
		draw_point(img, *it, points_color);
	}

	// Show results.
	imshow(win_delaunay, img);
	waitKey(0);
}


// --------------------------------------------------------------------------------------------------------

// Apply affine transform calculated using image1Tri and MorphedTri to src
void apply_Transform(Mat &morphedImage, Mat &image1, vector<Point2f> &image1Tri, vector<Point2f> &MorphedTri){
	Mat warped = getAffineTransform(image1Tri, MorphedTri);
	warpAffine(image1, morphedImage, warped, morphedImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}

// Used to determine the transformation matrix between the image and the morphed image
void Affine_transformation(){
	int x1, y1, k1, x2, y2, k2, m, n, i, j, k, z1, z2;
	for(i = 0; i < numbTotal; i = i + 3){
	
		x2 = list1[i].x - list1[i + 2].x;
		y2 = list1[i].y - list1[i + 2].y;
		k2 = morphList[i].x - morphList[i + 2].x;
		
		x1 = list1[i].x - list1[i + 1].x;
		y1 = list1[i].y - list1[i + 1].y;
		k1 = morphList[i].x - morphList[i + 1].x;
		
		pair <float, float> row2 = solveEq(x1, y1, morphList[i].y - morphList[i + 1].y, x2, y2, morphList[i].y - morphList[i + 2].y);
		pair <float, float> row1 = solveEq(x1, y1 , k1, x2, y2, k2);
		
		m = morphList[i].x - row1.first*list1[i].x - row1.second*list1[i].y;
		n = morphList[i].y - row2.first*list1[i].x - row2.second*list1[i].y;
		
		float res[2][1] = { 
					{0.0},
					{0.0} 
				  };
		
		float s[2][1] = { 
					(float){m},
					(float){n} 
				};
		
		float t[2][2] = {
				 	{ row1.first, row1.second },
					{ row2.first, row2.second } 
				};
				
		Mat RES = Mat(2, 1, CV_32FC1, res);		
		Mat A = Mat(2, 2, CV_32FC1, t);
		Mat B = Mat(2, 1, CV_32FC1, s);
		
		for(j = 0; j < image.rows; j++){
			for (k = 0; k < image.cols; k++){
				float x[2][1] = { 
							(float){ j },
							(float){ k } 
						};
						
				Mat X = Mat(2, 1, CV_32FC1, x);
				
				RES = A*X + B;

				z1 = abs((int)(RES.at<int>(0, 0)));
				z2 = abs((int)(RES.at<int>(1, 0)));

				if(checkIfIsInside(list1[i].x, list1[i].y, list1[i + 1].x, list1[i + 1].y, list1[i + 2].x, list1[i + 2].y, j, k)){
					
					if (checkIfIsInside(morphList[i].x, morphList[i].y, morphList[i + 1].x, morphList[i + 1].y, morphList[i + 2].x, morphList[i + 2].y, z1, z2)){
						morphImageMatrix [z1][z2] = 0.5*imageMatrix1[j][k]+0.8*imageMatrix2[j][k];
						morphImage.at<uchar>(z1, z2) = (uchar)(morphImageMatrix[z1][z2]);
					}
				}
			}
		}		
	}
}

// This is used to find the minimum area rectangle which encloses the given triangle
Rect boundingRectangle(vector<Point2f> &t){

	int leftMost, topMost, rightMost, bottomMost, i;

	rightMost = t[0].x;
	leftMost = t[0].x;
	topMost = t[0].y;
	bottomMost = t[0].y;

	for(i = 0; i < 3; i++){

		if (t[i].x < leftMost){
			leftMost = t[i].x;
		}

		if (t[i].y < topMost){
			topMost = t[i].y;
		}


		if (t[i].x > rightMost){
			rightMost = t[i].x;
		}

		if (t[i].y > bottomMost){
			bottomMost = t[i].y;
		}
	}

	return Rect(leftMost, topMost, rightMost + 1 - leftMost, bottomMost + 1 - topMost);
}

// Applies affine transform to the triangle and alpha blends the image
void morph_Tri(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t, double alpha){

	// Find bounding rectangle for each triangle
	Rect rectangle1 = boundingRectangle(t1);
	Rect rectangle2 = boundingRectangle(t2);
	Rect rectangle = boundingRectangle(t);

	// Offset points by left top corner of the respective rectangles
	vector<Point2f> t1Rect, t2Rect, tRect;
	vector<Point> tRectInt;
	
	for(int i = 0; i < 3; i++){
		t1Rect.push_back(Point2f(t1[i].x - rectangle1.x, t1[i].y - rectangle1.y));
		t2Rect.push_back(Point2f(t2[i].x - rectangle2.x, t2[i].y - rectangle2.y));
		
		tRect.push_back(Point2f(t[i].x - rectangle.x, t[i].y - rectangle.y));
		tRectInt.push_back(Point(t[i].x - rectangle.x, t[i].y - rectangle.y)); 
	}

	// Get mask by filling triangle
	Mat mask = Mat::zeros(rectangle.height, rectangle.width, CV_32FC3);
	fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

	// Apply warpImage to small rectangular patches
	Mat img1Rect, img2Rect;
	
	img1(rectangle1).copyTo(img1Rect);
	img2(rectangle2).copyTo(img2Rect);

	Mat warpImage1 = Mat::zeros(rectangle.height, rectangle.width, img1Rect.type());
	Mat warpImage2 = Mat::zeros(rectangle.height, rectangle.width, img2Rect.type());

	apply_Transform(warpImage1, img1Rect, t1Rect, tRect);
	apply_Transform(warpImage2, img2Rect, t2Rect, tRect);

	// Alpha blend rectangular patches
	Mat imgRect = alpha * warpImage2 + (1.0 - alpha) * warpImage1;

	// Copy triangular region of the rectangular patch to the output image
	multiply(imgRect, mask, imgRect);
	multiply(img(rectangle), Scalar(1.0, 1.0, 1.0) - mask, img(rectangle));
	img(rectangle) += imgRect;
}

//Calculates the average tie points and calls morph_tri on every triangle
void performMorph(string s1, string s2){
	double alpha;
	int i, a, b, c, q;
	q = 0;
	
	for(alpha = 0.05; alpha <= 1.0; alpha = alpha + 0.08){
		q++;
		
		//Read input images
		Mat img1 = imread(s1);
		Mat img2 = imread(s2);

		//convert Mat to float data type
		img1.convertTo(img1, CV_32F);
		img2.convertTo(img2, CV_32F);

		//empty average image
		Mat imgMorph = Mat::zeros(img1.size(), CV_32FC3);

		//Read points
		vector<Point2f> points;

		//compute weighted average point coordinates
		for(i = 0; i < tiePointsImage1.size(); i++){
			float x, y;
			
			y = alpha * tiePointsImage2[i].y + (1 - alpha) * tiePointsImage1[i].y;
			x = alpha * tiePointsImage2[i].x + (1 - alpha) * tiePointsImage1[i].x;

			points.push_back(Point2f(x, y));
		}

		//Read triangle indices
		ifstream ifs("Triangle1.txt");

		while (ifs >> a >> b >> c){
		
			// Triangles
			vector<Point2f> triangle1, triangle2, triangle;
			
			// Triangle corners for morphed image.
			triangle.push_back(points[a]);
			triangle.push_back(points[b]);
			triangle.push_back(points[c]);

			// Triangle corners for image 1.
			triangle1.push_back(tiePointsImage1[a]);
			triangle1.push_back(tiePointsImage1[b]);
			triangle1.push_back(tiePointsImage1[c]);

			// Triangle corners for image 2.
			triangle2.push_back(tiePointsImage2[a]);
			triangle2.push_back(tiePointsImage2[b]);
			triangle2.push_back(tiePointsImage2[c]);

			morph_Tri(img1, img2, imgMorph, triangle1, triangle2, triangle, alpha);
		}

		// Display Result
		imshow("Morphed Face", imgMorph / 255.0);
		string str = "Morph_image12" + to_string(q) + ".png";
		imwrite(str, imgMorph);
		waitKey(100);
	}
}


// Functions and variables for chosing tie points manually

Mat img1, img2;

// stores coordinates
vector<Point> capturePoint1;
vector<Point> capturePoint2;

// keeps count of number of mouse clicks 
int n1 = 0;
int n2 = 0;

// function for storing tie points of first image
void on_mouse1(int event, int x, int y, int flags, void *usic){
	// mouse position
	Point pt;
	char coordinate[16];
	
	if(event == CV_EVENT_LBUTTONDOWN){
		pt = Point(x, y);
		cout << "First image tie point #" << n1 << ": " << x << " " << y << "\n";
		capturePoint1.push_back(pt);
		n1++;
		
		circle(img1, pt, 5, CV_RGB(0, 0, 255), -1);
		imshow("img1", img1);
		
		// checking that number of points clicked is equal for both images
		if(n1 >= 50 && n2 >= 50){
			cvDestroyAllWindows();
		}
	}
}

// function for storing tie points of second image
void on_mouse2(int event, int x, int y, int flags, void *usic){
	// mouse position
	Point pt;
	char coordinate[16];
	
	if(event == CV_EVENT_LBUTTONDOWN){
		pt = Point(x, y);
		cout << "Second image tie point #" << n2 << ": " << x  << " "<< y << "\n";
		capturePoint2.push_back(pt);
		n2++;
		
		circle(img2, pt, 5, CV_RGB(0, 0, 255), -1);
		imshow("img2", img2);
		
		// checking that number of points clicked is equal for both images
		if(n1 >= 50 && n2 >= 50){
			cvDestroyAllWindows();
			
		}
	}
}

void choseTiePoints(){
	
	// file variables for storing the coordinates clicked
	ofstream file1, file2;
	
	img1 = imread("image1.jpg");
	img2 = imread("image2.jpg");
	
	namedWindow("img1", WINDOW_AUTOSIZE);
	namedWindow("img2", WINDOW_AUTOSIZE);
	
	imshow("img1", img1);
	imshow("img2", img2);
	
	string image1("image1.jpg");
	string image2("image2.jpg");
	
	string S = image1 + ".txt";
	char *nam = (char *)S.c_str();
	
	file1.open(nam, ios::app);
	
	S = image2 + ".txt";
	nam = (char *)S.c_str();
	
	file2.open(nam, ios::app);
	
	// mouse callback function
	setMouseCallback("img1", on_mouse1);
	setMouseCallback("img2", on_mouse2);
	
	if(!file1 || !file2){
		cout << "Open file error!\n";
		return;
	}
	
	waitKey(0);
	
	vector<Point>::iterator it1 = capturePoint1.begin();
	vector<Point>::iterator it2 = capturePoint2.begin();
	
	for(; it1 != capturePoint1.end(); it1++){
		file1 << it1->x << " " << it1->y << "\n";
	}
	
	for(; it2 != capturePoint2.end(); it2++){
		file2 << it2->x << " " << it2->y << "\n";
	}
	
	// closing both files
	file1.close();
	file2.close();
	
	waitKey(0);
}

int main(){
	
	int x, y, a, b, choice, i;
	choice = -1;
	
	cout << "Enter 1 to chose points manually or 0 to use default coordinates provided.\n";
	cin >> choice;
	
	if(choice == 1){
		// call manual function
		cout << "ATTENTION!\nAfter selecting the tie points manually, perform the following steps:\n";
		cout << "(1) Delete the original File1.txt and File2.txt files\n";
		cout << "(2) Rename image1.jpg.txt and image2.jpg.txt to File1.txt and File2.txt respectively\n";
		choseTiePoints();
	}
		
	if(choice != 1 && choice != 0){
		cout << "You have entered a wrong choice! Aborting\n";
		return 0;
	}
	
	string s1 = "image1.jpg";
	string s2 = "image2.jpg";

	ifstream ifs1("File1.txt");
	
	while(ifs1 >> x >> y){
		tiePointsImage1.push_back(Point2f(x, y));
		sizePoints++;
	}

	ifstream ifs2("File2.txt");
	while(ifs2 >> x >> y){
		tiePointsImage2.push_back(Point2f(x, y));
	}
	
	Triangle();

   	for(i = 0; i < numbTotal; i++){
		a = (list1[i].x + list2[i].x)/2;
		b = (list1[i].y + list2[i].y)/2;
		morphList.push_back(Point2f(a, b));
	}

	performMorph(s1, s2);
	return 0;
}
