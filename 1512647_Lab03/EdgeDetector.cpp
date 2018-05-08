#include "EdgeDetector.h"

int EdgeDetector::DetectEdge(const Mat & sourceImage, Mat & destinationImage, int kWidth, int kHeight, int method)
{
	// ảnh rỗng
	if (!sourceImage.data)
		return 0;

	// toán tử gradient
	// đạo hàm theo hướng x
	vector<float> gradient_x{ -1, 0, 1,
							-1, 0, 1,
							-1, 0, 1, };
	// đạo hàm theo hướng y
	vector<float> gradient_y{ -1, -1, -1,
							0, 0, 0,
							1, 1, 1 };

	// laplace kernel
	vector<float> Laplace{ 0, 1, 0,
						1, -4, 1,
						0, 1, 0 };

	
	Convolution convolution;
	int nrows = sourceImage.rows;
	int ncols = sourceImage.cols;

	// lưu giá trị đạo hàm hướng x, y
	Mat img_x(nrows, ncols, CV_8UC1), img_y(nrows, ncols, CV_8UC1);

	
	destinationImage.create(nrows, ncols, CV_8UC1);

	if (!destinationImage.data || !img_x.data || !img_y.data)
		return 0;

	switch (method)
	{
	case 1:
		gradient_x[3] = -2;
		gradient_x[5] = 2;
		gradient_y[1] = -2;
		gradient_y[7] = 2;

		// đạo hàm ảnh theo hướng x
		convolution.SetKernel(gradient_x, 3, 3);
		convolution.DoConvolution(sourceImage, img_x);

		// đạo hàm theo y
		convolution.SetKernel(gradient_y, 3, 3);
		convolution.DoConvolution(sourceImage, img_y);

		// tính toán kết quả ảnh góc cạnh
		for (int y = 0; y < nrows; ++y) {
			uchar* prow1 = img_x.ptr<uchar>(y);
			uchar* prow2 = img_y.ptr<uchar>(y);
			uchar* prow = destinationImage.ptr<uchar>(y);

			for (int x = 0; x < ncols; ++x) {
			
				prow[x] = (uchar)(sqrt(pow(prow1[x], 2) + pow(prow2[x], 2)/4.0));
			}
		}
		break;
	case 2:
		// đạo hàm ảnh theo hướng x
		convolution.SetKernel(gradient_x, 3, 3);
		convolution.DoConvolution(sourceImage, img_x);
		
		// đạo hàm theo y
		convolution.SetKernel(gradient_y, 3, 3);
		convolution.DoConvolution(sourceImage, img_y);

		// tính toán kết quả ảnh góc cạnh
		for (int y = 0; y < nrows; ++y) {
			uchar* prow1 = img_x.ptr<uchar>(y);
			uchar* prow2 = img_y.ptr<uchar>(y);
			uchar* prow = destinationImage.ptr<uchar>(y);

			for (int x = 0; x < ncols; ++x) {
				prow[x] = (uchar)((1.0/3)*sqrt(pow(prow1[x], 2)+ pow(prow2[x], 2)));
			}
		}
		break;
	case 3: 

		// với laplace thì không cần tính đạo
		// do là đạo hàm 2 lần của hàm gauss lên laplace có thể phát hiện những vùng màu biến thiên ít
		convolution.SetKernel(Laplace, 3, 3);
		convolution.DoConvolution(sourceImage, destinationImage);
		break;
	default:
		break;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("before", sourceImage);
	imshow("after", destinationImage);
	waitKey(0);
	return 1;
}

EdgeDetector::EdgeDetector()
{
}

EdgeDetector::~EdgeDetector()
{
}
