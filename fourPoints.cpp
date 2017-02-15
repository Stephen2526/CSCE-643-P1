#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <cmath>

using namespace std;
using namespace cv;

void onMouse(int, int, int, int, void*)
vector<Point> mousePickFourPoints(Mat&)
int markPoints(Mat&, vector<Point>&)
int remDistortion(Mat&, vector<Point2f>&, vector<Point2f>&)

/***main function***/
int main(int argc, char** argv) {
	//argument verification
	if (argc != 2) {
		cerr << "Usage: ./fourPoints image" << endl;
		return -1;
	}
	//initialize
	Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	
	if (img.empty() ) 
    { 
          cout << "Error loading the image" << endl;
          return -1; 
    }
	vector<Point2f> imgPoints;
	//fourPoints = mousePickFourPoints(img);
	//markPoints(img, fourPoints);

	//wall
	// imgPoints.push_back(Point(1266, 527));
	// imgPoints.push_back(Point(1624, 520));
	// imgPoints.push_back(Point(1620, 861));
	// imgPoints.push_back(Point(1270, 922));
	
	//points on floor 4*4
	// imgPoints.push_back(Point(1424, 1371)); //tl
	// imgPoints.push_back(Point(1697, 1271)); //tr
	// imgPoints.push_back(Point(2057, 1361)); //br
	// imgPoints.push_back(Point(1800, 1488)); //bl

	//floor small 1*1
	// imgPoints.push_back(Point(1513, 1398));
	// imgPoints.push_back(Point(1584, 1371));
	// imgPoints.push_back(Point(1669, 1398));
	// imgPoints.push_back(Point(1600, 1426));

	//floor small 2*2
	imgPoints.push_back(Point(1435, 1428));
	imgPoints.push_back(Point(1584, 1371));
	imgPoints.push_back(Point(1766, 1424));
	imgPoints.push_back(Point(1619, 1489));

	//four points in the world
	vector<Point2f> worPoints;
	float x1 = floor(img.size().width/2);
	float y1 = floor(img.size().height/2);
	
	//wall
	// worPoints.push_back(Point(x1, y1)); //tl
	// worPoints.push_back(Point(x1+500, y1)); //tr
	// worPoints.push_back(Point(x1+500, y1+500)); //br
	// worPoints.push_back(Point(x1, y1+500));  //bl
	
	//floor
	worPoints.push_back(Point(x1, y1)); //tl
	worPoints.push_back(Point(x1+100, y1)); //tr
	worPoints.push_back(Point(x1+100, y1+100)); //br
	worPoints.push_back(Point(x1, y1+100));  //bl
	cout << "worPoints = " << endl << " " << worPoints << endl << endl;

	remDistortion(img, imgPoints, worPoints);
	return 0;
}

/***call back of mouse action***/
void onMouse(int evt, int x, int y, int flags, void* param) {
    if(evt == CV_EVENT_RBUTTONDOWN) {

        vector<Point>* ptPtr = (vector<Point>*)param;
        ptPtr->push_back(Point(x,y));
        cout << "x: " << x <<" y: " << y << " size: " << ptPtr->size()<< endl;
    }
}

/***mouse picking four points and mark the point on the image***/
vector<Point> mousePickFourPoints(Mat& img) {
	//initialize
	vector<Point> points;
	namedWindow("Picking Four Points", WINDOW_NORMAL);
	//handler for mouse
	setMouseCallback("Picking Four Points", onMouse, (void*) &points);
	//wait for mouse picking
	imshow("Picking Four Points", img);
		
	//return time
	while (points.size() < 4) {
		waitKey(0);
	}
		
	return points;
}

/***mark the point and save images***/
int markPoints(Mat& img, vector<Point>& fourPoints) {
	Mat tmp_img(img);
	//mark the picked points onto image and show it
	circle(tmp_img, fourPoints[0], 5, Scalar(0, 0, 255), 2);
	putText(tmp_img, format("(%d,%d)", fourPoints[0].x, fourPoints[0].y), Point(fourPoints[0].x+15,fourPoints[0].y+15), FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 1.2, CV_AA);
	circle(tmp_img, fourPoints[1], 5, Scalar(0, 0, 255), 2);
	putText(tmp_img, format("(%d,%d)", fourPoints[1].x, fourPoints[1].y), Point(fourPoints[1].x+15,fourPoints[1].y+15), FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 1.2, CV_AA);
	circle(tmp_img, fourPoints[2], 5, Scalar(0, 0, 255), 2);
	putText(tmp_img, format("(%d,%d)", fourPoints[2].x, fourPoints[2].y), Point(fourPoints[2].x+15,fourPoints[2].y+15), FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 1.2, CV_AA);
	circle(tmp_img, fourPoints[3], 5, Scalar(0, 0, 255), 2);
	putText(tmp_img, format("(%d,%d)", fourPoints[3].x, fourPoints[3].y), Point(fourPoints[3].x+15,fourPoints[3].y+15), FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 1.2, CV_AA);

	namedWindow("Origin Image", WINDOW_NORMAL);
	imshow("Origin Image", tmp_img);
	waitKey(0);
	imwrite( "../four_points.jpg", tmp_img);

	return 0;
}

/***calculate H and remove the projective distortion***/
int remDistortion(Mat& img, vector<Point2f>& srcPoints, vector<Point2f>& resPoints) {
	//construct matrix A
	Mat A = Mat(8,9, CV_32F, cvScalar(0.));
	for (int i = 0; i < 4; i++) {
		A.at<float>(i*2, 0) = A.at<float>(i*2+1, 3) = srcPoints[i].x;
		A.at<float>(i*2, 1) = A.at<float>(i*2+1, 4) = srcPoints[i].y;
		A.at<float>(i*2, 2) = A.at<float>(i*2+1, 5) = 1.;
		A.at<float>(i*2, 6) = (-1)*srcPoints[i].x*resPoints[i].x;
		A.at<float>(i*2, 7) = (-1)*srcPoints[i].y*resPoints[i].x;
		A.at<float>(i*2, 8) = (-1)*resPoints[i].x;

		A.at<float>(i*2+1, 6) = (-1)*srcPoints[i].x*resPoints[i].y;
		A.at<float>(i*2+1, 7) = (-1)*srcPoints[i].y*resPoints[i].y;
		A.at<float>(i*2+1, 8) = (-1)*resPoints[i].y;
	}
	//cout << "A size: " << A.rows << "*" << A.cols << endl;
	cout << "A = "<< endl << " "  << A << endl << endl;

	//SVD Decomposition of A
	Mat w, u, vt;
	SVD::compute(A, w, u, vt, SVD::FULL_UV);
	//cout << "vt size: " << vt.rows << "*" << vt.cols << endl;
	//cout << "w size: " << w.rows << "*" << w.cols << endl;
	//cout << "u size: " << u.rows << "*" << u.cols << endl;
	//cout << "vt = "<< endl << " "  << vt << endl << endl;

	//determine H
	Mat H = Mat(3,3, CV_32F, cvScalar(0.));
	H.at<float>(0,0) = vt.at<float>(8,0);
	H.at<float>(0,1) = vt.at<float>(8,1);
	H.at<float>(0,2) = vt.at<float>(8,2);
	H.at<float>(1,0) = vt.at<float>(8,3);
	H.at<float>(1,1) = vt.at<float>(8,4);
	H.at<float>(1,2) = vt.at<float>(8,5);
	H.at<float>(2,0) = vt.at<float>(8,6);
	H.at<float>(2,1) = vt.at<float>(8,7);
	H.at<float>(2,2) = vt.at<float>(8,8);
	cout << "H = " << endl << " " << H << endl << endl;
	//cout << "H_inv = " << endl << " " << H.inv() << endl << endl;

	//calculate four points in rectified image
	Mat markPoints = Mat(3,4,CV_32F,Scalar::all(0.));
	for (int i = 0; i < 4; i++) {
		markPoints.at<float>(0,i) = srcPoints[i].x;
		markPoints.at<float>(1,i) = srcPoints[i].y;
		markPoints.at<float>(2,i) = 1.0;
	}
	cout << "markPoints = " << endl << " " << markPoints << endl << endl;
	Mat recPoints = H*markPoints;
	cout << "recPoints = " << endl << " " << recPoints << endl << endl;
	Mat inRecPoints(recPoints);
	inRecPoints.col(0) = inRecPoints.col(0)/recPoints.at<float>(2,0);
	inRecPoints.col(1) = inRecPoints.col(1)/recPoints.at<float>(2,1);
	inRecPoints.col(2) = inRecPoints.col(2)/recPoints.at<float>(2,2);
	inRecPoints.col(3) = inRecPoints.col(3)/recPoints.at<float>(2,3);
	cout << "inRecPoints = " << endl << " " << inRecPoints << endl << endl;

	Mat recImg;
	warpPerspective(img, recImg, H, img.size());
	namedWindow("rectified Image", WINDOW_NORMAL);
	imshow("rectified Image", recImg);
	waitKey(0);
	//imwrite( "../rectified.jpg", recImg);
	return 0;
}