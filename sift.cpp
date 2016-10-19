#include <opencv2/opencv.hpp>
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

#define pi 3.1415926

Mat GaussianKerGenerate(int i=5,float sigma=1.6){
	Mat ker=Mat::zeros(i,i,CV_32FC1);
	int j=0,k=0,l=i/2,j1,k1;
	for(j=0;j<i;j++){
		j1=j-l;
		for(k=0;k<i;k++){
			k1=k-l;
				ker.at<float>(j,k)=exp(-((j1*j1)+(k1*k1))/(2*sigma*sigma))/(2*pi*sigma*sigma);
		}
	}
}
Mat Norm_GaussianKer(int i=5,float sigma=1.6){
	Mat_<float> ker=Mat::zeros(i,i,CV_32FC1);
	int j=0,k=0,l=i/2,j1,k1;
	for(j=0;j<i;j++){
		j1=j-l;
		for(k=0;k<i;k++){
			k1=k-l;
				ker.at<float>(j,k)=(1-((j1*j1)+k1*k1)/(2*sigma*sigma))*exp(-((j1*j1)+(k1*k1))/(2*sigma*sigma))/(pi*pow(sigma,4));
		}
	}
	return ker;
}
//for CV_8UC1
Mat Dog(Mat filt_array,vector<Point2i>& maxpoint,vector<Point2i>& minpoint){
	int i,j;
	vector<Mat> tempMat(5);
	Mat result(filt_array.size(),CV_8UC1,128);
	float k=0.8,sigma=1.6;
	for(i=0;i<5;i++){
		float sigma1=sigma*pow(k,i);
		filter2D(filt_array,tempMat[i],-1,Norm_GaussianKer(5,sigma1));
	}
	for(i=0;i<filt_array.rows;i++){
		uchar* data1=tempMat[1].ptr<uchar>(i);
		uchar* data2=tempMat[2].ptr<uchar>(i);
		uchar* data3=tempMat[3].ptr<uchar>(i);
		uchar* data4=result.ptr<uchar>(i);
		for(j=0;j<filt_array.cols;j++){
			if((*data2>*data1)&&(*data2>*data3))
				{*data4=255;maxpoint.push_back(Point(j,i));}
			if((*data2<*data1)&&(*data2<*data3))
				{*data4=0;minpoint.push_back(Point(j,i));}
			data1++;data2++;data3++;data4++;
		}
	}
	return result;
}
void draw(Mat& pic,vector<Point2i>& ppoint,int r,int ccolor){
	int i;
	for(i=0;i<ppoint.size();i++){
		circle(pic,ppoint[i],r,Scalar::all(ccolor%255));
	}
}
int main(){
	Mat src=imread("doge.jpg",0);//read doge
	Mat dog,todarw;
	Mat todraw=imread("doge.jpg",1);
	vector<Mat> filter_array(5); 
	vector<Point2i> maxp,minp;
	imshow("doge",src);
	dog=Dog(src,maxp,minp);
	imshow("Dog",dog);
	draw(todraw,maxp,20,255);
	draw(todraw,minp,10,50);
	imshow("dog",todraw);
	waitKey(0);
	return 0;
}
