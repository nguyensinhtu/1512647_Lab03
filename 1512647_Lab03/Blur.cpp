#include "Blur.h"

// thuật toán tìm kiếm median với thời gian tuyến tính
// input : mảng 1 chiều chữa giá trị chưa được sắp xếp
// output : trả về giá trị median


int Blur::BlurImage(const Mat & sourceImage, Mat & destinationImage, int kWidth, int kHeight, int method)
{
	Convolution convolution;
	// kernel cho toán tử lọc trung bình
	vector<float> avg_kernel{ 1.0f / 9, 1.0f / 9, 1.0f / 9,
						1.0f / 9, 1.0f / 9, 1.0f / 9,
						1.0f / 9, 1.0f / 9, 1.0f / 9, };

	// kernel cho toán tử gaussian
	vector<float> gau_kernel{ 1.0f / 273, 4.0f / 273, 7.0f / 273, 4.0f / 273, 1.0f / 273,
							4.0f / 273, 16.0f / 273, 26.0f / 273, 16.0f / 273, 4.0f / 273,
							7.0f / 273, 26.0f / 273, 41.0f / 273, 26.0f / 273, 7.0f / 273, 
							4.0f / 273, 16.0f / 273, 26.0f / 273, 16.0f / 273, 4.0f / 273, 
							1.0f / 273, 4.0f / 273, 7.0f / 273, 4.0f / 273, 1.0f / 273, };
	
	int nrows = sourceImage.rows;
	int ncols = sourceImage.cols;
	const uchar *data = sourceImage.data;

	int sz = kWidth*kHeight;
	int *arr = new int[sz];
	int edgex = kWidth / 2;
	int edgey = kHeight / 2;
	destinationImage.create(nrows, ncols, CV_8UC1);
	if (!destinationImage.data)
		return 0;

	if (!data)
		return 0;

	switch (method)
	{
	case 0 : // lọc trung bình
		convolution.SetKernel(avg_kernel, kWidth, kHeight);
		convolution.DoConvolution(sourceImage, destinationImage);
		break;
	case 1: // lọc trung vị

		// lấy những pixel trong lân cận kWidth x kHeight
		// sắp xếp lại những giá trị đó tăng dần
		// giá trị trung vị ở vị trí kWidth*kHeight/2
		for (int y = edgey; y < nrows; ++y) {
			uchar *prow = destinationImage.ptr<uchar>(y);
			for (int x = edgex; x < ncols; ++x) {
				prow[x] = data[y*ncols + x];
				int i = 0;
				for (int fx = 0; fx < kHeight; ++fx)
					for (int fy = 0; fy < kWidth; ++fy) {
						arr[i] = data[(y + fx - edgey)*ncols + (x + fy + edgex)];
						++i;
					}
				sort(arr, arr + sz);
				prow[x] = (uchar)(arr[sz / 2]);
			}
		}
		break;
	case 2: // lọc gaussian
		convolution.SetKernel(gau_kernel, 5, 5);
		convolution.DoConvolution(sourceImage, destinationImage);
		break;
	default:
		break;
	}

	if (arr != NULL)
		delete arr;

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("before", sourceImage);
	imshow("after", destinationImage);
	waitKey(0);
	return 1;
}

Blur::Blur()
{
}

Blur::~Blur()
{
}
