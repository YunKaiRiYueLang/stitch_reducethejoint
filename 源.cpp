#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

// 调整两张图的亮度平衡  num_images是图片数量，images是对应的重叠区域。
double sqr(int value)
{
	return value*value;
}
void test(const int &num_images,   vector<cv::Mat> &images,Mat_<double> &gains)
{
	double Isum1 = 0, Isum2 = 0;//分子部分。

	Mat_<double> I(num_images, num_images); I.setTo(0);
	Mat_<int> N(num_images, num_images); N.setTo(0);

	for (int i = 0; i < num_images; ++i)
	{
		for (int j = i; j < num_images; ++j)
		{
			
		
			N(i, j) = N(j, i) = images[i].rows*images[i].cols;

			for (int r = 0; r < images[0].rows; r++)
			{
				uchar *r1;
				r1 = images[0].ptr<uchar>(r);
				uchar *r2;
				r2 = images[1].ptr<uchar>(r);
				for (int c = 0; c < images[0].cols; c++)
				{
					Isum1 += sqrt(static_cast<double>(sqr(r1[c]) + sqr(r1[c + 1]) + sqr(r1[c + 2])));
					Isum2 += sqrt(static_cast<double>(sqr(r2[c]) + sqr(r2[c + 1]) + sqr(r2[c + 2])));
				}
			}
			I(i, j) = Isum1 / N(i, j);
			I(j, i) = Isum2 / N(i, j);

		}
	}



	double alpha = 0.01;   
	double beta = 100;    

	Mat_<double> A(num_images, num_images); A.setTo(0);
	Mat_<double> b(num_images, 1); b.setTo(0);



	// 明天自己解这个方程，看结果。
	for (int i = 0; i < num_images; ++i)
	{
		for (int j = 0; j < num_images; ++j)
		{
			//std::cout << b(i, 0) << std::endl;
			b(i, 0) += beta * N(i, j);
			A(i, i) += beta * N(i, j);
			if (j == i) continue;
			A(i, i) += 2 * alpha * I(i, j) * I(i, j) * N(i, j);
			A(i, j) -= 2 * alpha * I(i, j) * I(j, i) * N(i, j);
		}

	}
	std::cout << "A:" << std::endl;
	std::cout << A(0, 0) << " " << A(0, 1) << std::endl;
	std::cout << A(1, 0) << " " << A(1, 1) << std::endl;
	std::cout << "b" << std::endl;
	std::cout << b(0, 0) << " " << b(1, 0) << std::endl;
	solve(A, b, gains);
	

}
void gainsapply(int index, Mat &image, Mat_<double> &gains)
{
	image *= gains(index, 0);
}



int main()
{
	//读入两幅有色差的原图。
	Mat sourceimage1 = imread("warp_0.jpg", 1);
	Mat sourceimage2 = imread("warp_1.jpg", 1);
	Mat src1 = sourceimage1.clone();
	Mat src2 = sourceimage2.clone();
	Mat sourceimage3 = imread("D:\\hspj\\config\\me\\front.jpg");
	Mat sourceimage4 = imread("D:\\hspj\\config\\me\\right.jpg");
	Mat src3 = sourceimage3.clone();
	Mat src4 = sourceimage4.clone();
	vector<Mat> image1;
	image1.push_back(sourceimage3);
	image1.push_back(sourceimage4);

	vector<Mat> images;
	images.push_back(sourceimage1);
	images.push_back(sourceimage2);
	// 已知的重合区域
	Mat roiimage1 = sourceimage1(Range(50,170), Range(340, 520));
	Mat roiimage2 = sourceimage2(Range(50, 170), Range(360, 540));


	vector<Mat> roiimages;
	roiimages.push_back(roiimage1);
	roiimages.push_back(roiimage2);

	Mat_<double> gains;
	test(2, roiimages,gains);
	for (int i = 0; i < 2; i++)
	{
		gainsapply(i, images[i], gains);
	}
	imwrite("front1.jpg", images[0]);
	imwrite("right1.jpg", images[1]);




	return 0;


}
