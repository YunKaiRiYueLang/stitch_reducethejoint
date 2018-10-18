#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

// 一张全黑的图，一张全白的图。用来测试test函数。

void test()
{



}
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst);
int main(int argc, char* argv[])
{
	Mat imageright = cv::Mat::ones(Size(1000, 1000), CV_8UC3);
	Mat imageleft(imageright.size(), imageright.type());
	imageleft = Scalar(255,255,255);
	Mat dst = cv::Mat::zeros(Size(1200,1000),CV_8UC3);

	
	imageleft.copyTo(dst(Rect(0, 0, imageleft.cols, imageleft.rows)));



	OptimizeSeam(imageleft,imageright,dst);

	return 0;
}

//该函数，和拼接图dst中两种图的位置关系，有很大关联。
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst)
{
	//int start = MIN(corners.left_top.x, corners.left_bottom.x);//开始位置，即重叠区域的左边界  
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