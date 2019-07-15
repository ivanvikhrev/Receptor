#include "detector.hpp"
#include <fstream>
#include <algorithm>

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
vector<DetectedObject> DnnDetector::findObjects(Mat& tmp, Mat& image, double thresh) {
	DetectedObject a;
	vector<DetectedObject> objects;
	float xL, yL, xR, yR;
	cout << tmp << endl;
	for (int i = 0; i < tmp.rows; i++) {
		double score = tmp.at<float>(i, 2);
		if (tmp.at<float>(i, 2) >= thresh) {
			a.score = tmp.at<float>(i, 2);
			a.uuid = tmp.at<float>(i, 1);
			xL = (tmp.at<float>(i, 3) > 1 || tmp.at<float>(i, 3) < 0) ? ((int)tmp.at<float>(i, 3)) : tmp.at<float>(i, 3);
		    yL = (tmp.at<float>(i, 4) > 1 || tmp.at<float>(i, 4) < 0) ? ((int)tmp.at<float>(i, 4)) : tmp.at<float>(i, 4);
			xR = (tmp.at<float>(i, 5) > 1 || tmp.at<float>(i, 5) < 0) ? ((int)tmp.at<float>(i, 5)) : tmp.at<float>(i, 5);
			yR = (tmp.at<float>(i, 6) > 1 || tmp.at<float>(i, 6) < 0) ? ((int)tmp.at<float>(i, 6)) : tmp.at<float>(i, 6);
			//cout << xL << yL << xR << yR;
			a.xLeftBottom = image.cols*xL;
			a.yLeftBottom = image.rows*yL;
			a.xRightTop = image.cols*xR;
			a.yRightTop = image.rows*yR;

			a.classname = labels[a.uuid - 1];
			objects.push_back(a);
		}
	}
	return objects;
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
   
    tmp = tmp.reshape(1, rows);

	objects = findObjects(tmp, image, thresh);
	if (objects.empty() && thresh > 0.4 || objects.size() ) {
		thresh -= 0.1;
		objects = findObjects(tmp, image, thresh);
	}
    return objects;
}