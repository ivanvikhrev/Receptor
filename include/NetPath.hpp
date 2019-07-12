#pragma once
#include<string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class NetPath
{
public:
    String imgName;
    String modelPath;
    String configPath;
    String labelsPath;
    int width;
    int height;
    Scalar mean;
    bool swapRB;
    double scale;

    NetPath(string _modelPath, string _configPath, string _labelsPath,
        int inputWidth, int inputHeight, Scalar _mean, bool _swapRB, double _scale);
    ~NetPath();
};

