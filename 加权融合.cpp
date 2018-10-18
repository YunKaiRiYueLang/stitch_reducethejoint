#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

// һ��ȫ�ڵ�ͼ��һ��ȫ�׵�ͼ����������test������

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

//�ú�������ƴ��ͼdst������ͼ��λ�ù�ϵ���кܴ������
void OptimizeSeam(Mat& img1, Mat& trans, Mat& dst)
{
	//int start = MIN(corners.left_top.x, corners.left_bottom.x);//��ʼλ�ã����ص��������߽�  
	int start = 200;
	double processWidth = img1.cols - start;//�ص�����Ŀ��  
	int rows = dst.rows;
	int cols = img1.cols; //ע�⣬������*ͨ����
	double alpha = 1;//img1�����ص�Ȩ��  
	for (int i = 0; i < rows; i++)
	{
		uchar* p = img1.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
		uchar* t = trans.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = start; j < cols; j++)
		{
			//�������ͼ��trans�������صĺڵ㣬����ȫ����img1�е�����
			if (t[j * 3] == 0 && t[j * 3 + 1] == 0 && t[j * 3 + 2] == 0)
			{
				alpha = 1;
			}
			else
			{
				//img1�����ص�Ȩ�أ��뵱ǰ�������ص�������߽�ľ�������ȣ�ʵ��֤�������ַ���ȷʵ��  
				alpha = (processWidth - (j - start)) / processWidth;
			}

			d[j * 3] = p[j * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[j * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[j * 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);

		}
	}

}