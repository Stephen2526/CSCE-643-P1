#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;

Point3f crossProduct(Point3f, Point3f);
int transform(const Mat &, const Mat &);

int main(int argc, char** argv) {
	//input argument validation
	if (argc != 2) {
		cerr << "Usage: ./TwoStep lineInfinity_3.jpg" << endl;
		return -1;
	}
	//read image
	Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {
		cerr << "open image error" << endl;
		return -1;
	}
	vector<Point3f> points; //points in affine
	//self draw wall 
	//red
	points.push_back(Point3f(1933, 804, 1)); //tl
	points.push_back(Point3f(1987, 1442, 1)); //bl
	points.push_back(Point3f(2937, 1561, 1)); //br
	//green
	points.push_back(Point3f(1933, 804, 1)); //tl
	points.push_back(Point3f(2883, 923, 1)); //tr
	points.push_back(Point3f(2937, 1561, 1)); //br
	points.push_back(Point3f(1987, 1442, 1)); //bl

	//self draw floor 
	//red
	// points.push_back(Point3f(1827, 1535, 1)); //tl
	// points.push_back(Point3f(1930, 1526, 1)); //bl
	// points.push_back(Point3f(2056, 1575, 1)); //br
	// //green
	// points.push_back(Point3f(1858, 1658, 1)); //tl
	// points.push_back(Point3f(1974, 1647, 1)); //tr
	// points.push_back(Point3f(2118, 1705, 1)); //br
	// points.push_back(Point3f(2000, 1716, 1)); //bl

	vector<Point3f> lines;
	lines.push_back(crossProduct(points[0], points[1]));
	lines.push_back(crossProduct(points[1], points[2]));
	lines.push_back(crossProduct(points[3], points[5]));
	lines.push_back(crossProduct(points[4], points[6]));

	cout << "lines = " << endl << lines << endl <<endl;

	//construct matrix A (l'_1m'_1,l'_1m'_2+l'_2m'_2, l'_2m'_2)
	Mat A = Mat(2,3, CV_32F, Scalar::all(0.));
	A.at<float>(0,0) = lines[0].x*lines[1].x;
	A.at<float>(0,1) = lines[0].x*lines[1].y + lines[0].y*lines[1].x;
	A.at<float>(0,2) = lines[0].y*lines[1].y;
	A.at<float>(1,0) = lines[2].x*lines[3].x;
	A.at<float>(1,1) = lines[2].x*lines[3].y + lines[2].y*lines[3].x;
	A.at<float>(1,2) = lines[2].y*lines[3].y;

	cout << "A = " << endl << A << endl <<endl;

	//SVD A
	Mat w, u, vt;
	SVD::compute(A, w, u, vt, SVD::FULL_UV);
	cout << "w = " << endl << w << endl <<endl;	
	cout << "vt = " << endl << vt << endl <<endl;

	Mat s = Mat(3,1, CV_32F, Scalar::all(0.));
	s.at<float>(0,0) = vt.at<float>(2,0);
	s.at<float>(1,0) = vt.at<float>(2,1);
	s.at<float>(2,0) = vt.at<float>(2,2);
	//s = (-1)*s; //force to be positive
	cout << "s = " << endl << s << endl <<endl;
	cout << "A*s = " << endl << A*s << endl <<endl;

	//S=KK.t()
	Mat S = Mat(2,2,CV_32F,Scalar::all(0.));
	S.at<float>(0,0) = s.at<float>(0,0);
	S.at<float>(0,1) = S.at<float>(1,0) = s.at<float>(1,0);
	S.at<float>(1,1) = s.at<float>(2,0);
	cout << "S = " << endl << S << endl <<endl;

	//SVD S
	Mat S_w, S_u, S_vt;
	SVD::compute(S, S_w, S_u, S_vt, SVD::FULL_UV);
	cout << "S_w = " << endl << S_w << endl <<endl;
	cout << "S_u = " << endl << S_u << endl <<endl;
	cout << "S_v = " << endl << S_vt.t() << endl <<endl;

	Mat S_w_root;
	pow(S_w, 0.5, S_w_root);
	cout << "S_w_root = " << endl << S_w_root << endl <<endl;	

	Mat S_ww = Mat(2,2,CV_32F,Scalar::all(0.));
	S_ww.at<float>(0,0) = S_w_root.at<float>(0);
	S_ww.at<float>(1,1) = S_w_root.at<float>(1);
	cout << "S_ww = " << endl << S_ww << endl <<endl;

	Mat K = S_u*S_ww*(S_u.t());
	//Mat K = S_u*S_ww;
	cout << "K = " << endl << K << endl <<endl;		

	Mat H_a = Mat::eye(3,3,CV_32F);
	Mat tmp = H_a(Rect(0,0,2,2));
	K.copyTo(tmp);
	cout << "H_a = " << endl << H_a << endl <<endl;	

	transform(img, H_a.inv());

	//calculate four points in rectified image
	Mat markPoints = Mat(3,4,CV_32F,Scalar::all(0.));
	for (int i = 0; i < 4; i++) {
		markPoints.at<float>(0,i) = lines[i].x;
		markPoints.at<float>(1,i) = lines[i].y;
		markPoints.at<float>(2,i) = lines[i].z;
	}

	Mat recPoints = H_a.t()*markPoints;
	cout << "recPoints = " << endl << " " << recPoints << endl << endl;
	Mat inRecPoints(recPoints);
	inRecPoints.col(0) = inRecPoints.col(0)/recPoints.at<float>(2,0);
	inRecPoints.col(1) = inRecPoints.col(1)/recPoints.at<float>(2,1);
	inRecPoints.col(2) = inRecPoints.col(2)/recPoints.at<float>(2,2);
	inRecPoints.col(3) = inRecPoints.col(3)/recPoints.at<float>(2,3);
	cout << "inRecPoints = " << endl << " " << inRecPoints << endl << endl;
	return 0;
}

Point3f crossProduct(Point3f vec1, Point3f vec2) {
	Point3f vec = Point3f(0.,0.,0.);
	vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
	vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
	vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return vec; 
}

int transform(const Mat &img, const Mat &H) {
	//construct a translation transform
	Mat Corners = Mat(3,4,CV_32F, Scalar::all(0.));
	//(0,0,1)
	Corners.at<float>(0,0) = 0;
	Corners.at<float>(1,0) = 0;
	Corners.at<float>(2,0) = 1;
	//(width,0,1)
	Corners.at<float>(0,1) = img.size().width;
	Corners.at<float>(1,1) = 0;
	Corners.at<float>(2,1) = 1;
	//(width,height,1)
	Corners.at<float>(0,2) = img.size().width;
	Corners.at<float>(1,2) = img.size().height;
	Corners.at<float>(2,2) = 1;
	//(0,height,1)
	Corners.at<float>(0,3) = 0;
	Corners.at<float>(1,3) = img.size().height;
	Corners.at<float>(2,3) = 1;
	cout << "Corners = " << endl << Corners << endl <<endl;

	//rectifies corners
	Mat recCorners = H*Corners;
	cout << "recCorners = " << endl << recCorners << endl <<endl;
	
	//find the smallest x,y coordinates
	float xMin = 0.;
	float yMin = 0.;
	for (int i = 0; i < 4; i++) {
		float xTmp = recCorners.at<float>(0,i)/recCorners.at<float>(2,i);
		float yTmp = recCorners.at<float>(1,i)/recCorners.at<float>(2,i);
		if ( xTmp < xMin) {
			xMin = xTmp; 
		}

		if (yTmp < yMin) {
			yMin = yTmp;
		}
	}
	Mat TL = Mat::eye(3,3,CV_32F);
	TL.at<float>(0,2) = (-1)*xMin;
	TL.at<float>(1,2) = (-1)*yMin;
	cout << "TL = " << endl << TL << endl <<endl;

	//warp image
	Mat TF = TL*H;
	cout << "TF = " << endl << TF << endl <<endl;
	Mat recImg;
	warpPerspective(img, recImg, TF, img.size()*2);
	namedWindow("rectified Image", WINDOW_NORMAL);
	imshow("rectified Image", recImg);
	waitKey(0);
}