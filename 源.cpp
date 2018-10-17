#include<opencv2\opencv.hpp>

using namespace cv;

void ComputeGainCompensator(const vector<Point> &corners,const vector<cv::Mat> &images)
{
	CV_Assert(corners.size() == images.size());
	const int num_images = static_cast<int>(images.size());
	Mat_<int> N(num_images, num_images); N.setTo(0);
	Mat_<double> I(num_images, num_images); I.setTo(0);




}

int main()
{




}