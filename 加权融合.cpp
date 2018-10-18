#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

// 一张全黑的图，一张全白的图。用来测试test函数。

// 使用需要：1. 准确获悉接缝的位置。
//  对于拼接处直上直下的图，直接处理融合区域，应该更好。 用黑白图测试，可以直接处理融合区域。
void test(Mat& img1, Mat& trans, Mat& dst)
{
	int start = 200;
	double processWidth = img1.cols - start;//重叠区域的宽度  
	int rows = dst.rows;
	int cols = img1.cols; //注意，是列数*通道数
	double alpha = 1;//img1中像素的权重  
	for (int i = 0; i < rows; i++)
	{
		uchar* p = img1.ptr<uchar>(i);  //获取第i行的首地址
		uchar* t = trans.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = start; j < cols; j++)
		{
			//如果遇到图像trans中无像素的黑点，则完全拷贝img1中的数据
			if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
			{
				alpha = 1;
			}
			else
			{
				//img1中像素的权重，与当前处理点距重叠区域左边界的距离成正比，实验证明，这种方法确实好  
				alpha = (processWidth - (j - start)) / processWidth;
			}

			d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);

		}
	}


}
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst);
int main(int argc, char* argv[])
{
	Mat imageright = cv::Mat::ones(Size(1000, 1000), CV_8UC3);
	Mat imageleft(imageright.size(), imageright.type());
	imageleft = Scalar(255,255,255);
	//Mat dst = cv::Mat::zeros(Size(1200,1000),CV_8UC3);
	Mat dst = cv::Mat::zeros(Size(1000,1000),CV_8UC3);

	
	imageleft.copyTo(dst(Rect(0, 0, imageleft.cols, imageleft.rows)));



	OptimizeSeam(imageleft,imageright,dst);

	Mat left = imread("warp_0.jpg");
	Mat right = imread("D:\\hspj\\config\\me\\原图.jpg");
	Mat rightcopy = right.clone();


	Mat stitchleft=left(Range(0,220),Range(337,397)).clone();
	Mat stitchright = right(Range(0,220),Range(353,413)).clone();
	dst = right(Range(0,220), Range(350,370));
	OptimizeSeam(stitchleft, stitchright, dst);




	return 0;
}

//该函数，和拼接图dst中两种图的位置关系，有很大关联。
// 图像关系，trans在图像的左边，img1在图像的右边，后复制到dst，接缝在trans中。改了复制关系，会改变
//接缝位置，造成融合效果不好。
// 环视拼接中的接缝和上面的顺序不同，需要重新融合函数中给dst复制时，使用alpha的顺序。
// 使用这个函数，需要准确获悉接缝的位置，不然，影响融合效果。
// 当两张图都作为重叠区域时（完全重合）,img1,到trans的渐变。
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst)
{
	//int start = MIN(corners.left_top.x, corners.left_bottom.x);//开始位置，即重叠区域的左边界  
	int start = 000;
	double processWidth = img1.cols - start;//重叠区域的宽度  
	int rows = dst.rows;
	int cols = img1.cols; //注意，是列数*通道数
	double alpha = 1;//img1中像素的权重  
	for (int i = 0; i < rows; i++)
	{
		uchar* p = img1.ptr<uchar>(i);  //获取第i行的首地址
		uchar* t = trans.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = start; j < cols; j++)
		{
			//如果遇到图像trans中无像素的黑点，则完全拷贝img1中的数据
			if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
			{
				alpha = 1;
			}
			else
			{
				//img1中像素的权重，与当前处理点距重叠区域左边界的距离成正比，实验证明，这种方法确实好  
				alpha = (processWidth - (j - start)) / processWidth;
			}

			d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);

		}
	}

}