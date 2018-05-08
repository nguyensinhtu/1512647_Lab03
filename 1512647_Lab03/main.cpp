#include "Blur.h"
#include "EdgeDetector.h"
#include <iostream>

int main(int argc, char** argv) {

	if (argc < 4) {
		cout << "loi tham so" << endl;
		return 0;
	}

	// load ảnh 
	Mat img = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	
	Mat dst(img.rows, img.cols, img.type());

	int szWindow = atoi(argv[3]);

	Blur *blur = new Blur();
	EdgeDetector *edgeDetector = new EdgeDetector();
	int res = 0;
	if (strcmp(argv[1], "--mean") == 0) {
		cout << argv[1] << endl;
		blur->BlurImage(img, dst, szWindow, szWindow, 0);
	}
	else if (strcmp(argv[1], "--median") == 0) {
		blur->BlurImage(img, dst, szWindow, szWindow, 1);
	}else if (strcmp(argv[1], "--gauss") == 0) {
		res = blur->BlurImage(img, dst, szWindow, szWindow, 2);
	}else if (strcmp(argv[1], "--sobel") == 0) {
		res = edgeDetector->DetectEdge(img, dst, szWindow, szWindow, 1);
	}else if (strcmp(argv[1], "--prewitt") == 0) {
		res = edgeDetector->DetectEdge(img, dst, szWindow, szWindow, 2);
	}else if (strcmp(argv[1], "--laplace") == 0) {
		res = edgeDetector->DetectEdge(img, dst, szWindow, szWindow, 3);
	}
	else {
		cout << "loi tham so" << endl;
	}

	if (!res) {
		cout << "khong thanh cong" << endl;
	}

	//if (img.data)
	//	img.release();
	//if (dst.data != NULL)
	//	dst.release();

	return 0;
}


