#include "NetPath.hpp"



NetPath::NetPath(string _modelPath, string _configPath, string _labelsPath,
    int inputWidth, int inputHeight, Scalar _mean, bool _swapRB, double _scale) {
    modelPath = _modelPath;
    configPath = _configPath;
    labelsPath = _labelsPath;
    width = inputWidth;
    height = inputHeight;
    mean = _mean;
    swapRB = _swapRB;
    scale = _scale;
}


NetPath::~NetPath()
{
}
