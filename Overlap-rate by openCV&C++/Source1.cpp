#include <iostream>
#include<string>
#include<fstream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<algorithm>
using namespace std;
using namespace cv;

void DrawPolygon(Mat inputImage, vector<Point> polygonPoint, bool bIsFill, bool bIsClosed = true)//�e��
{
	vector<vector<Point>> contours;
	contours.push_back(polygonPoint);

	if (bIsFill)
		fillPoly(inputImage, contours, Scalar(255, 255, 255), 8);
	else
		polylines(inputImage, polygonPoint, bIsClosed, Scalar(255, 255, 255), 1, 8);
}


double Area(vector<Point> polygonPoint)//�D���n
{
	const int polysize = polygonPoint.size();
	if (polysize < 3)
		return 0;

	double area = polygonPoint.back().x * polygonPoint[0].y - polygonPoint[0].x * polygonPoint.back().y;
	for (int i = 1, k = 0; i < polysize; i++, k++)
	{
		area += (polygonPoint[k].x * polygonPoint[i].y);
		area -= (polygonPoint[i].x * polygonPoint[k].y);
	}
	return fabs(0.5 * area);
}

bool Ptinpolygon(Point p, vector<Point>& polygonPoint, int count)//�P�_�IP�O�_�b�h��θ�
{
	int n = 0;//���I�Ӽ�
	for (int i = 0; i < count; i++)
	{//p1,p2�s�u
		Point p1 = polygonPoint[i];
		Point p2 = polygonPoint[(i + 1) % count];

		if (p1.y == p2.y)continue;
		if (p.y < min(p1.y, p2.y))continue;
		if (p.y >= max(p1.y, p2.y))continue;

		//�D���IX�y��
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;

		if (x > p.x) n++;
	}

	//���I������->�b�h��Υ~
	//�_��->�b��
	if (n % 2 == 1) return true;
	else return false;
}


bool onsegment(Point p1, Point p2, Point q)//���I�O�_�b�u�q�W
{
	Point big;
	Point small;
	if (p1.x > p2.x) {
		big.x = p1.x;
		small.x = p2.x;
	}
	else {
		big.x = p2.x;
		small.x = p1.x;
	}

	if (p1.y > p2.y) {
		big.y = p1.y;
		small.y = p2.y;
	}
	else {
		big.y = p2.y;
		small.y = p1.y;
	}


	if (small.x <= q.x && q.x <= big.x && small.y <= q.y && q.y <= big.y)
		return true;
	else return false;
}

int cc(Point a, Point b)//�e�n->�^�ǯ¶q->�h����V
{
	return a.x * b.y - a.y * b.x;
}

Point cross(Point a1, Point a2, Point b1, Point b2)//����I
{
	Point c;
	Point a = a2 - a1;
	Point b = b2 - b1;
	Point s = b1 - a1;
	if (cc(a, b) == 0)
	{
		c.x = -1;
		c.y = -1;
		return c;
	}
	c.x = a1.x + a.x * cc(s, b) / cc(a, b);
	c.y = a1.y + a.y * cc(s, b) / cc(a, b);


	if (onsegment(a1, a2, c) && onsegment(b1, b2, c)) {
		cout<<"���I" << c << endl;
		return c;
	}
	else return Point(-1, -1);

}

bool PointCmp(const Point& a, const Point& b, const Point& center)
{
	if (a.x >= 0 && b.x < 0)
		return true;
	if (a.x == 0 && b.x == 0)
		return a.y > b.y;
	//�V�qOA�M�V�qOB���e�n
	int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
	if (det < 0)
		return true;
	if (det > 0)
		return false;
	//�V�qOA�M�V�qOB�@�u�A�H�Z���P�_�j�p
	int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
	int d2 = (b.x - center.x) * (b.x - center.y) + (b.y - center.y) * (b.y - center.y);
	return d1 > d2;
}

void ClockwiseSortPoints(std::vector<Point>& vPoints)//�y�бƧ�
{
	//�p�⭫��
	cv::Point center;
	double x = 0, y = 0;
	for (int i = 0; i < vPoints.size(); i++)
	{
		x += vPoints[i].x;
		y += vPoints[i].y;
	}
	center.x = (int)x / vPoints.size();
	center.y = (int)y / vPoints.size();

	//��w�Ƨ�
	for (int i = 0; i < vPoints.size() - 1; i++)
	{
		for (int j = 0; j < vPoints.size() - i - 1; j++)
		{
			if (PointCmp(vPoints[j], vPoints[j + 1], center))
			{
				cv::Point tmp = vPoints[j];
				vPoints[j] = vPoints[j + 1];
				vPoints[j + 1] = tmp;
			}
		}
	}
}



int main()
{
	ifstream infile;
	infile.open("26.JSON");

	string* data = new string[500];
	int Linenum = 0;
	if (!infile) {
		cout << "File could not be open!" << endl;
	}

	while (!infile.eof()) {
		getline(infile, data[Linenum]);
		Linenum++;
	}
	infile.close();

	string A[200][2];
	int posA = 0;//����xy����m
	int numA = 0;//�y�ЭӼ�
	int pA[10];//
	int a = 0;//
	for (int i = 0; i < Linenum - 1; i++)
	{
		int pos;
		if (data[i].find("A") != string::npos) {
			pos = data[i].find("A");
			numA++;
		}
		pos += 12;

		while (data[i][pos] != ']') {
			for (int j = 0; j < 2; j++) {
				for (pos++; data[i][pos] != ',' && data[i][pos] != ']'; pos++) {
					A[posA][j].push_back(data[i][pos]);
				}
			}
			posA++;
		}
		pA[a] = posA;
		a++;
		break;
	}




	int intA[1000][2];//����int
	for (int i = 0; i < posA; i++) {
		for (int j = 0; j < 2; j++) {
			intA[i][j] = atof(A[i][j].c_str());
		}
	}
	vector<Point> PolygonPoint;//A
	vector<Point> test_PolygonPoint;//X

	test_PolygonPoint.push_back(Point(523, 488));
	test_PolygonPoint.push_back(Point(431, 693));
	test_PolygonPoint.push_back(Point(816, 676));
	test_PolygonPoint.push_back(Point(683, 467));

	int frist = 0;
	for (int k = 0; k < numA; k++) {
		for (int i = frist; i < pA[k]; i++) {
			if (intA[i][0] < 0) {
				intA[i][0] = 0;//�p��0->��0
			}
			else if (intA[i][0] > 959) {
				intA[i][0] = 959;//�W�L959->��959
			}
			else if (intA[i][1] < 0) {
				intA[i][1] = 0;//�p��0->��0
			}
			else if (intA[i][1] > 959) {
				intA[i][1] = 959;//�W�L959->��959
			}
			PolygonPoint.push_back(Point(intA[i][0], intA[i][1]));
			//cout << "A(x,y)" << '(' << PolygonPoint[i].x << ',' << PolygonPoint[i].y << ')' << endl;
		}
		frist = pA[k];
	}




	double areaA = Area(PolygonPoint);//A���n
	double areaT = Area(test_PolygonPoint);//X���n
	cout << "A's Area: " << areaA << endl << "X's Area: " << areaT << endl;


	const int polysizeA = PolygonPoint.size();//A���X���I
	const int polysizeT = test_PolygonPoint.size();

	vector<Point> CrossPoint;//�b�h��Τ������I



	vector<Point>::iterator itA;
	vector<Point>::iterator itT;

	int time = 0;
	for (itA = PolygonPoint.begin(); itA != PolygonPoint.end(); itA++) {//����I
		for (itT = test_PolygonPoint.begin(); itT != test_PolygonPoint.end(); itT++) {
			Point nexta = itA + 1 == PolygonPoint.end() ? *PolygonPoint.begin() : *(itA + 1);
			Point nextt = itT + 1 == test_PolygonPoint.end() ? *test_PolygonPoint.begin() : *(itT + 1);
			Point temp = cross(*itA, nexta, *itT, nextt);
			if (temp.x != -1 && temp.y != -1)
				CrossPoint.push_back(temp);
			if (time == 0) {
				for (int i = 0; i < polysizeA; i++) {
					Point p = PolygonPoint[i];
					if (Ptinpolygon(p, test_PolygonPoint, polysizeT)) {
						CrossPoint.push_back(p);
						//cout << "inside point: " << '(' << p.x << ',' << p.y << ')' << endl;
					}
				}
				time++;
			}
		}
	}

	for (int i = 0; i < polysizeT; i++) {
		Point p = test_PolygonPoint[i];
		if (Ptinpolygon(p, PolygonPoint, polysizeA)) {
			CrossPoint.push_back(p);
			//cout << "inside point: " << '(' << p.x << ',' << p.y << ')' << endl;
		}
	}

	//sort(CrossPoint.begin(), CrossPoint.end
	ClockwiseSortPoints(CrossPoint);
	for (int i = 0; i < CrossPoint.size(); i++)
	{
		cout << "cross point:" << CrossPoint[i] << endl;
	}


	Mat imgDrawA = Mat(960, 960, CV_8UC3, Scalar(255, 0, 255));
	Mat imgDrawT = Mat(960, 960, CV_8UC3, Scalar(255, 255, 0));
	Mat imgDrawC = Mat(960, 960, CV_8UC3, Scalar(0, 255, 255));

	DrawPolygon(imgDrawA, PolygonPoint, false);
	//DrawPolygon(imgDrawA, test_PolygonPoint, false);
	DrawPolygon(imgDrawT, test_PolygonPoint, false);
	//DrawPolygon(imgDrawA, CrossPoint, false);


	const int polysizeC = CrossPoint.size();


	DrawPolygon(imgDrawA, test_PolygonPoint, false);
	DrawPolygon(imgDrawA, CrossPoint, false);

	double areaC = Area(CrossPoint);//���|���n
	//cout << "cross area: " << areaC << endl;

	//���|�v
	float overlaparea = (float)areaC / (areaA + areaT - areaC) * 100;
	cout << "Overlap rate: " << overlaparea << '%' << endl;



	//�e
	imshow("imgDrawA", imgDrawA);
	//imshow("imgDrawT", imgDrawT);
	//imshow("imgDrawC", imgDrawC);

	waitKey(0);
	destroyAllWindows();
}