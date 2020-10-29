#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
using namespace cv;

Mat SimplestColorBalance(Mat img, int percent) {
	if (percent <= 0)
		percent = 5;
	img.convertTo(img, CV_32F);
	vector<Mat> channels (3);
	int rows = img.rows; // number of rows of image
	int cols = img.cols; // number of columns of image
	int chnls = img.channels(); //  number of channels of image
	double halfPercent = percent / 200.0;
	if (chnls == 3) split(img, channels);
	//else channels.add(img);
	vector<Mat> results;
	for (int i = 0; i < chnls; i++) {
		// find the low and high precentile values (based on the input percentile)
		Mat flat;
		channels[i].reshape(1, 1).copyTo(flat);
		cv::sort(flat, flat, SORT_ASCENDING);
		double lowVal = flat.at<int>(0, (int)floor(flat.cols * halfPercent));
		double topVal = flat.at<int>(0, (int)ceil(flat.cols * (1.0 - halfPercent)));
		// saturate below the low percentile and above the high percentile
		Mat channel = channels[i];
		for (int m = 0; m < rows; m++) {
			for (int n = 0; n < cols; n++) {
				if (channel.at<int>(m, n) < lowVal) channel.at<int>(m, n)= lowVal;
				if (channel.at<int>(m, n) > topVal) channel.at<int>(m, n)= topVal;
			}
		}
		normalize(channel, channel, 0.0, 255.0,NORM_MINMAX);
		channel.convertTo(channel,CV_32F);
		results.push_back(channel);
	}
	Mat outval;
	merge(results, outval);
	return outval;
}