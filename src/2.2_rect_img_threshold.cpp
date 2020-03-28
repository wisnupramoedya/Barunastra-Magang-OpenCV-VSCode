#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat src_gray;
int thresh = 100;
RNG rng(12345);

void thresh_callback(int, void*);

/** Global Variables */
const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
const String window_detection_name2 = "Object Detection 2";

int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

int low_H2 = 0, low_S2 = 0, low_V2 = 0;
int high_H2 = max_value_H, high_S2 = max_value, high_V2 = max_value;

//Trackbar for first object
static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", window_detection_name, low_H);
}

static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", window_detection_name, high_H);
}

static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", window_detection_name, low_S);
}

static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detection_name, high_S);
}

static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", window_detection_name, low_V);
}

static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", window_detection_name, high_V);
}

//Trackbar for second object
static void on_low_H_thresh_trackbar2(int, void *)
{
	low_H2 = min(high_H2 - 1, low_H2);
	setTrackbarPos("Low H 2", window_detection_name2, low_H2);
}

static void on_high_H_thresh_trackbar2(int, void *)
{
	high_H2 = max(high_H2, low_H2 + 1);
	setTrackbarPos("High H 2", window_detection_name2, high_H2);
}

static void on_low_S_thresh_trackbar2(int, void *)
{
	low_S2 = min(high_S2 - 1, low_S2);
	setTrackbarPos("Low S 2", window_detection_name2, low_S2);
}

static void on_high_S_thresh_trackbar2(int, void *)
{
	high_S2 = max(high_S2, low_S2 + 1);
	setTrackbarPos("High S 2", window_detection_name2, high_S2);
}

static void on_low_V_thresh_trackbar2(int, void *)
{
	low_V2 = min(high_V2 - 1, low_V2);
	setTrackbarPos("Low V 2", window_detection_name2, low_V2);
}

static void on_high_V_thresh_trackbar2(int, void *)
{
	high_V2 = max(high_V2, low_V2 + 1);
	setTrackbarPos("High V 2", window_detection_name2, high_V2);
}

int main(int argc, char* argv[])
{
	//! [cap]
	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
	//! [cap]

	//! [window]
	namedWindow(window_capture_name);
	namedWindow(window_detection_name);
	namedWindow(window_detection_name2);
	//! [window]

	//! [trackbar]
	// Trackbars to set thresholds for HSV values
	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

	createTrackbar("Low H 2", window_detection_name2, &low_H2, max_value_H, on_low_H_thresh_trackbar2);
	createTrackbar("High H 2", window_detection_name2, &high_H2, max_value_H, on_high_H_thresh_trackbar2);
	createTrackbar("Low S 2", window_detection_name2, &low_S2, max_value, on_low_S_thresh_trackbar2);
	createTrackbar("High S 2", window_detection_name2, &high_S2, max_value, on_high_S_thresh_trackbar2);
	createTrackbar("Low V 2", window_detection_name2, &low_V2, max_value, on_low_V_thresh_trackbar2);
	createTrackbar("High V 2", window_detection_name2, &high_V2, max_value, on_high_V_thresh_trackbar2);
	//! [trackbar]

	Mat frame, frame_HSV, frame_HSV2, frame_threshold, frame_threshold2;
	while (true) {
		//! [while]
		cap >> frame;
		if (frame.empty())
		{
			break;
		}

		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
		cvtColor(frame, frame_HSV2, COLOR_BGR2HSV);
		cvtColor(frame, src_gray, COLOR_BGR2GRAY);
		blur(src_gray, src_gray, Size(3, 3));

		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
		inRange(frame_HSV2, Scalar(low_H2, low_S2, low_V2), Scalar(high_H2, high_S2, high_V2), frame_threshold2);
		//! [while]

		//! [show]
		// Show the frames
		imshow(window_capture_name, frame);
		imshow(window_detection_name, frame_threshold);
		imshow(window_detection_name2, frame_threshold2);

		//! [show]

		const int max_thresh = 255;
		createTrackbar("Canny thresh:", window_capture_name, &thresh, max_thresh, thresh_callback);
		thresh_callback(255, 0);

		char key = (char)waitKey(30);
		if (key == 'q' || key == 27)
		{
			break;
		}
	}
	return 0;
}

void thresh_callback(int, void*)
{
	Mat canny_output;
	Canny(src_gray, canny_output, thresh, thresh * 2);
	vector<vector<Point> > contours;
	findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(contours[i], contours_poly[i], 3, true);
		boundRect[i] = boundingRect(contours_poly[i]);
	}

	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2);
	}
	imshow("Contours", drawing);
}