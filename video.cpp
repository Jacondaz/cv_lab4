#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {

	Mat img = imread("C:/Users/karet/Downloads/temp.png");
	Mat frame, gray, gauss, contur;
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	VideoCapture cap("C:/Users/karet/Downloads/test_video.mp4");
	if (!cap.isOpened()) {
		std::cout << "Error: Unable to open the video file." << std::endl;
		return -1;
	}

	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("output.mp4", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));


	while (true) {

		Mat frame;
		cap >> frame;

		if (frame.empty()) {
			break;
		}

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gauss, Size(5, 5), 0);
		Canny(gauss, contur, 50, 200);
		findContours(contur, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		for (size_t i = 0; i < contours.size(); i++){
			double area = contourArea(contours[i]);

			if (area > 300){
				Rect rect = boundingRect(contours[i]);
				rectangle(frame, rect, Scalar(0, 255, 0), 2);

				Point textOrg(rect.x, rect.y - 10);
				putText(frame, "rectangle", textOrg, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
			}
		}

		imshow("Video", frame);
		video.write(frame);

		char c = (char)waitKey(1);

		if (c == 27) {
			break;
		}
	}

	cap.release();
	destroyAllWindows();
	return 0;
}