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
		cerr << "Usage: ./OneStep ***.jpg" << endl;
		return -1;
	}
	//read image
	Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	if (img.empty()) {
		cerr << "open image error" << endl;
		return -1;
	}

	//input points
	vector<Point3f> points; //points in projective

	//wall big 1 pair
	points.push_back(Point3f(419, 68, 1)); //tl
	points.push_back(Point3f(657, 92, 1)); //tr
	points.push_back(Point3f(666, 300, 1)); //br
	//2 pair
	points.push_back(Point3f(687, 830, 1)); //tl
	points.push_back(Point3f(696, 1023, 1)); //tr
	points.push_back(Point3f(457, 1062, 1)); //br
	//3 pair
	points.push_back(Point3f(1448, 174, 1)); //tl
	points.push_back(Point3f(1261, 154, 1)); //tr
	points.push_back(Point3f(1264, 350, 1)); //br
	//4 pair
	points.push_back(Point3f(1269, 782, 1)); //tl
	points.push_back(Point3f(1270, 921, 1)); //tr
	points.push_back(Point3f(1468, 887, 1)); //br
	//5 pair
	points.push_back(Point3f(1271, 949, 1)); //tl
	points.push_back(Point3f(1618, 888, 1)); //tr
	points.push_back(Point3f(1614, 1188, 1)); //br
	points.push_back(Point3f(1273, 1295, 1)); //bl

	/*//wall small 1 pair
	points.push_back(Point3f(1270, 922, 1)); //tl
	points.push_back(Point3f(1266, 527, 1)); //tr
	points.push_back(Point3f(1624, 520, 1)); //br
	//2 pair
	points.push_back(Point3f(1266, 527, 1)); //tl
	points.push_back(Point3f(1624, 520, 1)); //tr
	points.push_back(Point3f(1620, 861, 1)); //br
	//3 pair
	points.push_back(Point3f(1624, 520, 1)); //tl
	points.push_back(Point3f(1620, 861, 1)); //tr
	points.push_back(Point3f(1270, 922, 1)); //br
	//4 pair
	points.push_back(Point3f(1620, 861, 1)); //tl
	points.push_back(Point3f(1270, 922, 1)); //tr
	points.push_back(Point3f(1266, 527, 1)); //br
	//5 pair
	points.push_back(Point3f(1266, 527, 1)); //tl
	points.push_back(Point3f(1624, 520, 1)); //tr
	points.push_back(Point3f(1620, 861, 1)); //br
	points.push_back(Point3f(1270, 922, 1)); //bl*/

	/*//floor3*3 1 pair
	points.push_back(Point3f(1509, 1398, 1)); //tl
	points.push_back(Point3f(1424, 1371, 1)); //tr
	points.push_back(Point3f(1499, 1344, 1)); //br
	//2 pair
	points.push_back(Point3f(1568, 1318, 1)); //tl
	points.push_back(Point3f(1635, 1293, 1)); //tr
	points.push_back(Point3f(1719, 1317, 1)); //br
	//3 pair
	points.push_back(Point3f(1807, 1340, 1)); //tl
	points.push_back(Point3f(1899, 1364, 1)); //tr
	points.push_back(Point3f(1838, 1394, 1)); //br
	//4 pair
	points.push_back(Point3f(1771, 1425, 1)); //tl
	points.push_back(Point3f(1696, 1456, 1)); //tr
	points.push_back(Point3f(1600, 1426, 1)); //br
	//5 pair
	points.push_back(Point3f(1347, 1460, 1)); //tl
	points.push_back(Point3f(1431, 1428, 1)); //tr
	points.push_back(Point3f(1522, 1460, 1)); //br
	points.push_back(Point3f(1438, 1494, 1)); //br*/

	//floor 1*1  works
	//1 pair
	/*points.push_back(Point3f(1568, 1317, 1)); //tl
	points.push_back(Point3f(1634, 1292, 1)); //tr
	points.push_back(Point3f(1718, 1316, 1)); //br
	//2 pair
	points.push_back(Point3f(1634, 1292, 1)); //tl
	points.push_back(Point3f(1718, 1316, 1)); //tr
	points.push_back(Point3f(1654, 1341, 1)); //br
	//3 pair
	points.push_back(Point3f(1718, 1316, 1)); //tl
	points.push_back(Point3f(1654, 1341, 1)); //tr
	points.push_back(Point3f(1568, 1317, 1)); //br
	//4 pair
	points.push_back(Point3f(1654, 1341, 1)); //tl
	points.push_back(Point3f(1568, 1317, 1)); //tr
	points.push_back(Point3f(1634, 1292, 1)); //br
	//5 pair
	points.push_back(Point3f(1568, 1317, 1)); //tl
	points.push_back(Point3f(1634, 1292, 1)); //tr
	points.push_back(Point3f(1718, 1316, 1)); //br
	points.push_back(Point3f(1654, 1341, 1)); //bl*/
	

	//calculate ortho line parts
	vector<Point3f> lines;
	//1 pair
	lines.push_back(crossProduct(points[0], points[1]));
	lines.push_back(crossProduct(points[2], points[1]));

	//2 pair
	lines.push_back(crossProduct(points[3], points[4]));
	lines.push_back(crossProduct(points[4], points[5]));

	//3 pair
	lines.push_back(crossProduct(points[6], points[7]));
	lines.push_back(crossProduct(points[7], points[8]));

	//4 pair
	lines.push_back(crossProduct(points[9], points[10]));
	lines.push_back(crossProduct(points[10], points[11]));

	//5 pair
	lines.push_back(crossProduct(points[12], points[14]));
	lines.push_back(crossProduct(points[13], points[15]));

	cout << "lines = " << endl << lines << endl << endl;
	//construct A
	Mat A = Mat(5,6,CV_32F,Scalar::all(0.));
	for (int i = 0; i < 5; i++) {
		A.at<float>(i,0) = lines[2*i].x*lines[2*i+1].x;
		A.at<float>(i,1) = 0.5*(lines[2*i].x*lines[2*i+1].y + lines[2*i].y*lines[2*i+1].x);
		A.at<float>(i,2) = lines[2*i].y*lines[2*i+1].y;
		A.at<float>(i,3) = 0.5*(lines[2*i].x*lines[2*i+1].z + lines[2*i].z*lines[2*i+1].x);
		A.at<float>(i,4) = 0.5*(lines[2*i].y*lines[2*i+1].z + lines[2*i].z*lines[2*i+1].y);
		A.at<float>(i,5) = lines[2*i].z*lines[2*i+1].z;
	}
	cout << "A = " << endl << A << endl << endl;

	//SVD A
	Mat Aw, Au, Avt;
	//cout << "debug 3" << endl;
	SVD::compute(A, Aw, Au, Avt, SVD::FULL_UV);
	cout << "Aw = " << endl << Aw << endl <<endl;
	cout << "Au = " << endl << Au << endl <<endl;
	cout << "Av = " << endl << Avt.t() << endl <<endl;

	Mat c = Mat(6,1,CV_32F,Scalar::all(0.));
	Avt.row(5).copyTo(c);
	cout << "c = " << endl << c << endl <<endl;	

	//construct C
	Mat C = Mat(3,3,CV_32F,Scalar::all(0.));
	C.at<float>(0,0) = c.at<float>(0);
	C.at<float>(1,1) = c.at<float>(2);
	C.at<float>(2,2) = c.at<float>(5);
	C.at<float>(0,1) = C.at<float>(1,0) = 0.5*c.at<float>(1);
	C.at<float>(0,2) = C.at<float>(2,0) = 0.5*c.at<float>(3);
	C.at<float>(1,2) = C.at<float>(2,1) = 0.5*c.at<float>(4);
	//C = (-1)*C;
	cout << "C = " << endl << C << endl <<endl;	

	//SVD C
	Mat Cw, Cu, Cvt;
	SVD::compute(C, Cw, Cu, Cvt, SVD::FULL_UV);
	cout << "Cw = " << endl << Cw << endl <<endl;
	cout << "Cu = " << endl << Cu << endl <<endl;
	cout << "Cv = " << endl << Cvt.t() << endl <<endl;

	//decompose w to three matrix w' * w* * w' 
	//[diag(sqrt(sigma1), sqrt(sigma2), 10^3)]*[diag(1, 1, sigma3/10^6)]*[diag(sqrt(sigma1), sqrt(sigma2), 10^3)]
	Mat ww = Mat(3,3,CV_32F,Scalar::all(0.));
	ww.at<float>(0,0) = sqrt(Cw.at<float>(0));
	ww.at<float>(1,1) = sqrt(Cw.at<float>(1));
	ww.at<float>(2,2) = 1.0;
	cout << "ww = " << endl << ww << endl <<endl;	

	//construct H = Cu*ww
	Mat H = Cu*ww;
	//Mat H = Cu*ww/ww.at<float>(2,2);
	cout << "H = " << endl << H << endl <<endl;		
	cout << "H.inv = " << endl << H.inv()<< endl <<endl;	
	
	//verify the decomposition


	/*Mat S = Mat(2,2,CV_32F,Scalar::all(0.));
	Mat tmp = C(Rect(0,0,2,2));
	tmp.copyTo(S);
	cout << "S = " << endl << S << endl << endl;

	//calculate K, S=K*K_t
	//SVD S
	Mat Sw, Su, Svt;
	//cout << "debug 3" << endl;
	SVD::compute(S, Sw, Su, Svt, SVD::FULL_UV);
	cout << "Sw = " << endl << Sw << endl <<endl;
	cout << "Su = " << endl << Su << endl <<endl;
	cout << "Sv = " << endl << Svt.t() << endl <<endl;

	Mat Sw_root;
	pow(Sw, 0.5, Sw_root);
	cout << "Sw_root = " << endl << Sw_root << endl <<endl;	

	Mat Sww = Mat(2,2,CV_32F,Scalar::all(0.));
	Sww.at<float>(0,0) = Sw_root.at<float>(0);
	Sww.at<float>(1,1) = Sw_root.at<float>(1);
	cout << "Sww = " << endl << Sww << endl <<endl;

	Mat K = Su*Sww*(Su.t());
	//Mat K = Su*Sww;
	cout << "K = " << endl << K << endl <<endl;

	//calcualte v
	Mat Rhs = Mat(2,1 ,CV_32F,Scalar::all(0.));
	Mat v = Mat(2,1 ,CV_32F,Scalar::all(0.));
	Rhs.at<float>(0) = C.at<float>(0,2);
	Rhs.at<float>(1) = C.at<float>(1,2);
	cout << "Rhs = " << endl << Rhs << endl <<endl;
	solve(S,Rhs,v,DECOMP_SVD);
	cout << "v = " << endl << v << endl <<endl;

	//construct Hp
	Mat Hp = Mat::eye(3,3,CV_32F);
	Hp.at<float>(2,0) = v.at<float>(0);
	Hp.at<float>(2,1) = v.at<float>(1);
	cout << "Hp = " << endl << Hp << endl <<endl;
	//construct Ha
	Mat Ha = Mat::eye(3,3,CV_32F);
	tmp = Ha(Rect(0,0,2,2));
	K.copyTo(tmp);
	cout << "Ha = " << endl << Ha << endl <<endl;

	//construct H
	Mat H = Hp*Ha;
	cout << "H = " << endl << H << endl <<endl;*/	

	transform(img, H.inv());

	//calculate four points in rectified image
	Mat markPoints = Mat(3,10,CV_32F,Scalar::all(0.));
	for (int i = 0; i < 10; i++) {
		markPoints.at<float>(0,i) = points[i].x;
		markPoints.at<float>(1,i) = points[i].y;
		markPoints.at<float>(2,i) = points[i].z;
	}
	cout << "markPoints = " << endl << " " << markPoints << endl << endl;
	Mat recPoints = H.t()*markPoints;
	cout << "recPoints = " << endl << " " << recPoints << endl << endl;
	Mat inRecPoints(recPoints);
	inRecPoints.col(0) = inRecPoints.col(0)/recPoints.at<float>(2,0);
	inRecPoints.col(1) = inRecPoints.col(1)/recPoints.at<float>(2,1);
	inRecPoints.col(2) = inRecPoints.col(2)/recPoints.at<float>(2,2);
	inRecPoints.col(3) = inRecPoints.col(3)/recPoints.at<float>(2,3);
	cout << "inRecPoints = " << endl << " " << inRecPoints << endl << endl;

	/*Mat recImg;
	warpPerspective(img, recImg, H.inv(), img.size());
	namedWindow("rectified Image", WINDOW_NORMAL);
	imshow("rectified Image", recImg);*/

	//draw line in original image
	namedWindow("original Image", WINDOW_NORMAL);
	line(img, Point(points[0].x, points[0].y), Point(points[1].x,points[1].y), Scalar( 255, 0, 0 ),  2, 8 );

	line(img, Point(points[1].x, points[1].y), Point(points[2].x,points[2].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[3].x, points[3].y), Point(points[4].x,points[4].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[4].x, points[4].y), Point(points[5].x,points[5].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[6].x, points[6].y), Point(points[7].x,points[7].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[7].x, points[7].y), Point(points[8].x,points[8].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[9].x, points[9].y), Point(points[10].x,points[10].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[10].x, points[10].y), Point(points[11].x,points[11].y), Scalar( 255, 0, 0 ),  2, 8 );
	line(img, Point(points[12].x, points[12].y), Point(points[14].x,points[14].y), Scalar( 110, 220, 0 ),  2, 8 );
	line(img, Point(points[13].x, points[13].y), Point(points[15].x,points[15].y), Scalar( 110, 220, 0 ),  2, 8 );
  	imshow("original Image",img);
	waitKey(0);
	
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
	warpPerspective(img, recImg, TF, img.size()*6);
	namedWindow("rectified Image", WINDOW_NORMAL);
	imshow("rectified Image", recImg);
	waitKey(0);
}