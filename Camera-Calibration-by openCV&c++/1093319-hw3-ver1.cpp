#include <iostream>
#include<string>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<vector>
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	int checkboardsize[2]{ 6,9 };//棋盤的尺寸
	ifstream fin;
	Mat img, gray;
	char temp[300]{};
	fin.open("case1.txt");
	while (!fin.eof())
	{
		string temp;
		getline(fin, temp);
		img = imread(temp);

		vector<vector<Point3f> > objpoints;// 三維點
		vector<vector<Point2f> > imgpoints;//二維點


		vector<Point3f> objp;//三維坐標系
		for (int i{ 0 }; i < checkboardsize[1]; i++)
		{
			for (int j{ 0 }; j < checkboardsize[0]; j++)
			{
				objp.push_back(Point3f(j, i, 0));
			}
		}


		vector<Point2f> corner_pts;//儲存角點像素座標
		bool success;

		cvtColor(img, gray, COLOR_BGR2GRAY);

		//找角點
		//如果是所需數量的角則success = true
		success = findChessboardCorners(gray, Size(checkboardsize[0], checkboardsize[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

		if (success)//如果是需要的角點 優化他
		{
			TermCriteria criteria(TermCriteria::EPS | TermCriteria::Type::MAX_ITER, 30, 0.001);

			cornerSubPix(gray, corner_pts, Size(11, 11), Size(-1, -1), criteria);//優化二維點

			//drawChessboardCorners(img, Size(checkboardsize[0], checkboardsize[1]), corner_pts, success);//顯示角點

			objpoints.push_back(objp);
			imgpoints.push_back(corner_pts);
		}
		//imshow("Image", img);
		//waitKey(0);

		Mat cameraMatrix, distCoeffs, R, T;


		calibrateCamera(objpoints, imgpoints, Size(gray.rows, gray.cols), cameraMatrix, distCoeffs, R, T);//校準

		// 内参矩阵
		//cout << "cameraMatrix : " << cameraMatrix << std::endl;
		// 畸變系数
		//cout << "distCoeffs : " << distCoeffs << std::endl;
		// rvecs
		//cout << "Rotation vector : " << R << std::endl;
		// tvecs
		//cout << "Translation vector : " << T << std::endl;

		Mat dst;
		undistort(img, dst, cameraMatrix, distCoeffs, noArray());//矯正

		imshow("Image", img);
		imshow("Dst", dst);
		waitKey(0);
		//return 0;
	}


}