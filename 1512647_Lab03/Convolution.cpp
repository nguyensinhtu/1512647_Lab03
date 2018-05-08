#include "Convolution.h"

vector<float> Convolution::GetKernel()
{
	return _kernel;
}

void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{
	_kernel = kernel;
	_kernelWidth = kWidth;
	_kernelHeight = kHeight;
}

int Convolution::DoConvolution(const Mat & sourceImage, Mat & destinationImage)
{
	// ảnh rỗng
	if (!sourceImage.data)
		return 0;

	// kiểm tra source Image có phải là ảnh xám
	if (sourceImage.type() != CV_8UC1)
		return 0;

	int nrows = sourceImage.rows;
	int ncols = sourceImage.cols;
	if (!destinationImage.data)
		return 0;

	int sz1 = _kernelHeight / 2;
	int sz2 = _kernelWidth / 2;
	//cout << sz1 << " " << sz2 << endl;
	cout << "kernel " << endl;
	for (int i = -sz1; i <= sz1; ++i) {
		for (int j = -sz2; j <= sz2; ++j)
			cout << _kernel[(sz1 - i)*_kernelWidth + (sz2 - j)] << " ";
		cout << endl;
	}
	const uchar *data = (uchar*)sourceImage.data;
	for (int y = 0; y < nrows; ++y) {
		uchar* prow = destinationImage.ptr<uchar>(y);
		for (int x = 0; x < ncols; ++x) {
			double res = 0.0;
			for (int i = -sz1; i <= sz1; ++i) {
				for (int j = -sz2; j <= sz2; ++j) {
					if (y + i < 0 || y + i >= nrows || x + j < 0 || x + j >= ncols)
						continue;
					res += data[(y + i)*ncols + (x + j)] * _kernel[(sz1 - i)*_kernelWidth + (sz2 - j)];
				}
			}

			//cout << (int)res << endl;
			if (res > 255)
				res = 255;
			if (res < 0)
				res = 0;
			
			prow[x] = (uchar)(res);
		}
	}
	return 1;
}

Convolution::Convolution()
{
}

Convolution::~Convolution()
{
}

