#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

float *HOG(Mat refMat) {
	int blockSize = 16;
	int nX = refMat.cols / blockSize;
	int nY = refMat.rows / blockSize;
	Mat gx, gy;
	Mat mag, angle;
	int cellnums = nX * nY;	//cell总数
	int bins = cellnums * 8;
	Sobel(refMat, gx, CV_32F, 1, 0, 1);
	Sobel(refMat, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	//定义存储数组
	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float) * bins);
	int binnumber = 0;
	for (int j = 0; j < nY; j++){
		for (int i = 0; i < nX; i++) {

			for (int a = j * blockSize; a < (j + 1)*blockSize; a++) {
				for (int b = i * blockSize; b < (i + 1)*blockSize; b++) {
					int angle1 = 0;//
					float angle2 = angle.at<float>(a, b);	//像素的角度值
					angle1 = angle2/45;	//角度位置
					float mag1 = mag.at<float>(a, b);		//像素的幅度值
					ref_hist[angle1 + binnumber * 8] += mag1;				//在数组的角度位置增加幅度值
				}
				}
			binnumber++;//令循环开始下一个cell的操作
			}
		}
	return ref_hist;
}

int main() {
	Mat src = imread("E://Git//hogTemplate.jpg");
	Mat img1 = imread("E://Git//img1.jpg");
	Mat img2 = imread("E://Git//img2.jpg");

	int blockSize = 16;
	int nX = src.cols / blockSize;
	int nY = src.rows / blockSize;
	int cellnums = nX * nY;	//cell总数
	int bins = cellnums * 8;

	*HOG(src);
	*HOG(img1);
	*HOG(img2);
	int result1 = 0;
	int result2 = 0;
	for (int i = 0; i < bins; i++)
	{
		//不会算平方根......
		result1 += abs (HOG(src)[i] - HOG(img1)[i]);
		result2 += abs (HOG(src)[i] - HOG(img2)[i]);
	}
	cout << result1 << endl;
	cout << result2 << endl;
	if (result1 < result2)
	{
		cout << "img1" << endl;
	}
	else
		cout << "img2" << endl;

	waitKey(0);
	return 0;
}