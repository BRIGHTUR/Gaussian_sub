#include<iostream>
#include<opencv2\opencv.hpp>
using namespace std;
using namespace cv;

int calcGaussianBackground(std::vector<cv::Mat> srcMats, cv::Mat &meanMat, cv::Mat &varMat) {
	int rows = srcMats[0].rows;
	int cols = srcMats[0].cols;
	for (int h = 0; h < rows; h++) {
		for (int w = 0; w < cols; w++) {
			int sum = 0;
			float var = 0;
			//��������
			for (int i = 0; i < srcMats.size(); i++){
				sum = sum + srcMats[i].at<uchar>(h, w);
			}
			meanMat.at<uchar>(h, w) = sum / srcMats.size();
			//�󷽲�
			for (int i = 0; i < srcMats.size(); i++) {
				var = var + pow((srcMats[i].at<uchar>(h, w) - meanMat.at<uchar>(h, w)), 2);
			}
			varMat.at<float>(h, w) = var / srcMats.size();
		}
	}
	return 0;
}

int gaussianThreshold(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float weight, cv::Mat &dstMat) {
	int srcI;
	int meanI;
	int dstI;
	int rows = srcMat.rows;
	int cols = srcMat.cols;

	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			srcI = srcMat.at<uchar>(h, w);
			meanI = meanMat.at<uchar>(h, w);
			int dif = abs(srcI - meanI);
			int th = weight*varMat.at<float>(h, w);
			if (dif > th) {
				dstMat.at<uchar>(h, w) = 255;
			}
			else {
				dstMat.at<uchar>(h, w) = 0;
			}
		}
	}
	return 0;
}
int main() {
	VideoCapture capVideo(0);
	if (!capVideo.isOpened())
	{
		std::cout << "���ܴ���Ƶ�ļ�" << std::endl;
		return -1;
	}
	//�������㱳��ģ�͵�ͼ��
	vector<cv::Mat> srcMats;
	//��������
	int nBg = 200;
	float wVar = 1;
	int cnt = 0;
	Mat frame;
	Mat meanMat;
	Mat varMat;
	Mat dstMat;
	while (true) {
		capVideo >> frame;
		cvtColor(frame, frame, CV_BGR2GRAY);
		if (cnt < nBg) {
			srcMats.push_back(frame);
			if (cnt == 0) {
				cout << "reading frame" << endl;
			}
		}
		else if (cnt == nBg) {
			//����ģ��
			meanMat.create(frame.size(), CV_8UC1);
			varMat.create(frame.size(), CV_32FC1);
			cout << "calculating backgroud models" << endl;
			calcGaussianBackground(srcMats, meanMat, varMat);
		}
		else {
			//�������
			dstMat.create(frame.size(), CV_8UC1);
			gaussianThreshold(frame, meanMat, varMat, wVar, dstMat);
			imshow("result", dstMat);
			imshow("frame", frame);
			waitKey(0);
		}
		cnt++;
	}
}