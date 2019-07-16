#include "detector.hpp"
#include <fstream>
#include <algorithm>

struct Freak
{
	string name;
	int count;
	int place;
	int probability;

};

DnnDetector::DnnDetector(string _modelPath, string _configPath, string _labelsPath,
	int inputWidth, int inputHeight, Scalar _mean, bool _swapRB, double _scale) {
	modelPath = _modelPath;
	configPath = _configPath;
	labelsPath = _labelsPath;
	width = inputWidth;
	height = inputHeight;
	mean = _mean;
	swapRB = _swapRB;
	scale = _scale;
	int backendId = DNN_BACKEND_OPENCV;
	int targetId = DNN_TARGET_CPU;

	net = readNet(modelPath, configPath);
	net.setPreferableBackend(backendId);
	net.setPreferableTarget(targetId);

	//std::string name;

	ifstream in(labelsPath);

	string name;

	if (in.is_open())
	{
		while (getline(in, name))
		{
			labels.push_back(name);
		}
	}
	in.close();
}


vector<DetectedObject> DnnDetector::Detect(Mat image) {
	Mat inputTensor, tmp;
	vector<DetectedObject> objects;
	int ddepth = CV_32F;
	double thresh = 0.5;
	blobFromImage(image, inputTensor, scale, Size(width, height), mean, swapRB, false, ddepth);
	net.setInput(inputTensor);

	tmp = net.forward().reshape(1, 1);
	int rows = tmp.cols / 7;
	int col = tmp.cols;
	tmp = tmp.reshape(1, rows);

	DetectedObject a;

	vector<DetectedObject> allObjects;
	float xL, yL, xR, yR;
	for (int i = 0; i < rows; i++) {
		a.score = tmp.at<float>(i, 2);
		a.uuid = tmp.at<float>(i, 1);

		xL = (tmp.at<float>(i, 3) > 1 || tmp.at<float>(i, 3) < 0) ? ((int)tmp.at<float>(i, 3)) : tmp.at<float>(i, 3);
		yL = (tmp.at<float>(i, 4) > 1 || tmp.at<float>(i, 4) < 0) ? ((int)tmp.at<float>(i, 4)) : tmp.at<float>(i, 4);
		xR = (tmp.at<float>(i, 5) > 1 || tmp.at<float>(i, 5) < 0) ? ((int)tmp.at<float>(i, 5)) : tmp.at<float>(i, 5);
		yR = (tmp.at<float>(i, 6) > 1 || tmp.at<float>(i, 6) < 0) ? ((int)tmp.at<float>(i, 6)) : tmp.at<float>(i, 6);

		a.xLeftBottom = image.cols * xL;
		a.yLeftBottom = image.rows * yL;
		a.xRightTop = image.cols * xR;
		a.yRightTop = image.rows * yR;


		a.classname = labels[a.uuid - 1];
		allObjects.push_back(a);

	}

	vector<DetectedObject> freak;
	if (allObjects.size() != 0) {

		freak.push_back(allObjects[0]);
		freak.back().count = 0;

		for (int i = 0; i < allObjects.size(); i++) {
			int count = 0;
			for (int j = 0; j < freak.size(); j++) {

				if (allObjects[i].classname != freak[j].classname) {
					count++;
					if (count == freak.size()) {

						freak.push_back(allObjects[i]);
						freak.back().count = 1;
					}
				}
				else {

					if (allObjects[i].score > freak[j].score)
						freak[j] = allObjects[i];

					freak[j].count++;
				}
			}

		}


		for (int i = 0; i < freak.size(); i++) {
			cout << freak[i].classname << " " << freak[i].count << endl;
		}



	}

	for (int i = 0; i < rows; i++) {
		if (tmp.at<float>(i, 2) >= thresh) {
			a.score = tmp.at<float>(i, 2);
			a.uuid = tmp.at<float>(i, 1);
			xL = (tmp.at<float>(i, 3) > 1 || tmp.at<float>(i, 3) < 0) ? ((int)tmp.at<float>(i, 3)) : tmp.at<float>(i, 3);
			yL = (tmp.at<float>(i, 4) > 1 || tmp.at<float>(i, 4) < 0) ? ((int)tmp.at<float>(i, 4)) : tmp.at<float>(i, 4);
			xR = (tmp.at<float>(i, 5) > 1 || tmp.at<float>(i, 5) < 0) ? ((int)tmp.at<float>(i, 5)) : tmp.at<float>(i, 5);
			yR = (tmp.at<float>(i, 6) > 1 || tmp.at<float>(i, 6) < 0) ? ((int)tmp.at<float>(i, 6)) : tmp.at<float>(i, 6);

			a.xLeftBottom = image.cols * xL;
			a.yLeftBottom = image.rows * yL;
			a.xRightTop = image.cols * xR;
			a.yRightTop = image.rows * yR;


			a.classname = labels[a.uuid - 1];
			objects.push_back(a);
		}
	}

	if (objects.size() == 0 && freak.size() != 0) {
		std::sort(freak.begin(), freak.end(),
			[](const DetectedObject& a, const DetectedObject& b) { return a.count > b.count; });
		objects.push_back(freak[0]);
	}


	return objects;
}