#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;

Point3f crossProduct(Point3f, Point3f);

int main(int argc, char** argv) {
	//input argument validation
	if (argc != 2) {
		cerr << "Usage: ./lineInfinity img_dir" << endl;
		return -1;
	}
	//read image
	Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {
		cerr << "open image error" << endl;
		return -1;
	}
	vector<Point3f> points;
	
	//wall (works)
	points.push_back(Point3f(1266, 527, 1)); //tl
	points.push_back(Point3f(1624, 520, 1)); //tr
	points.push_back(Point3f(1620, 861, 1)); //br
	points.push_back(Point3f(1270, 922, 1)); //bl

	//floor 1*1 (works)
	// points.push_back(Point3f(1568, 1317, 1)); //tl
	// points.push_back(Point3f(1634, 1292, 1)); //tr
	// points.push_back(Point3f(1718, 1316, 1)); //br
	// points.push_back(Point3f(1654, 1341, 1)); //bl

	vector<Point3f> lines;
	lines.push_back(crossProduct(points[0], points[1]));
	lines.push_back(crossProduct(points[3], points[2]));
	lines.push_back(crossProduct(points[0], points[3]));
	lines.push_back(crossProduct(points[1], points[2]));
	//cout << "line 1_1: " << lines[0] << "line 1_2: " << lines[1]<< endl;
	//cout << "line 2_1: " << lines[2] << "line 2_2: " << lines[3]<< endl;

	vector<Point3f> infPoints;
	infPoints.push_back(crossProduct(lines[0], lines[1]));
	infPoints.push_back(crossProduct(lines[2], lines[3]));

	Point3f infLine = crossProduct(infPoints[0], infPoints[1]);
	cout << "inf line: " << infLine << endl;

	//construct transform H
	Mat H_p = Mat(3,3, CV_32F, cvScalar(0.));
	H_p.at<float>(0,0) = H_p.at<float>(1,1) = 1.0;
	H_p.at<float>(2,0) = infLine.x/infLine.z;
	H_p.at<float>(2,1) = infLine.y/infLine.z;
	H_p.at<float>(2,2) = infLine.z/infLine.z;
	cout << "H_p = " << endl << " " << H_p << endl << endl;

	Mat recImg;
	warpPerspective(img, recImg, H_p, img.size()*2);
	namedWindow("rectified Image", WINDOW_NORMAL);
	imshow("rectified Image", recImg);
	waitKey(0);

	//calculate four points in rectified image
	Mat markPoints = Mat(3,4,CV_32F,Scalar::all(0.));
	for (int i = 0; i < 4; i++) {
		markPoints.at<float>(0,i) = points[i].x;
		markPoints.at<float>(1,i) = points[i].y;
		markPoints.at<float>(2,i) = points[i].z;
	}
	cout << "markPoints = " << endl << " " << markPoints << endl << endl;
	Mat recPoints = H_p*markPoints;
	cout << "recPoints = " << endl << " " << recPoints << endl << endl;
	Mat inRecPoints(recPoints);
	inRecPoints.col(0) = inRecPoints.col(0)/recPoints.at<float>(2,0);
	inRecPoints.col(1) = inRecPoints.col(1)/recPoints.at<float>(2,1);
	inRecPoints.col(2) = inRecPoints.col(2)/recPoints.at<float>(2,2);
	inRecPoints.col(3) = inRecPoints.col(3)/recPoints.at<float>(2,3);
	cout << "inRecPoints = " << endl << " " << inRecPoints << endl << endl;

	//save image
	vector<int> qulity_params;
    qulity_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    qulity_params.push_back(95);
    imwrite("../Q3build/lineInfinity_wall.jpg", recImg, qulity_params);
	return 0;
}

Point3f crossProduct(Point3f vec1, Point3f vec2) {
	Point3f vec = Point3f(0.,0.,0.);
	vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
	vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
	vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return vec; 
}