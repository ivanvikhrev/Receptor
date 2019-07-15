#include <opencv2/opencv.hpp>
#include "Recipes.hpp"
#include "detectedobject.hpp"
#include "detector.hpp"
#include <fstream>
#include <iostream>


using namespace std;
using namespace cv;
using namespace cv::dnn;

const char* cmdAbout = "Sample of OpenCV usage. ";



const char* cmdOptions =
"{ i  image                             |../images/16.jpg| image to process                  }"
"{ w  width                             |300| image width for classification    }"
"{ h  heigth                            |300| image heigth fro classification   }"
"{ model_path                           |../detection/sorted_inference_graph.pb|}"
"{ config_path                          |../detection/graph.pbtxt| path to model configuration}"
"{ label_path                           |../detection/labels.txt| path to class labels              }"
"{ mean                                 |""0.0 0.0 0.0""| vector of mean model values       }"
"{ swap                                 |1| swap R and B channels. TRUE|FALSE }"
"{ menu_path                            |../menu/menu.txt|  }"
"{ recipe_path                          |../menu/recipes|  }"
"{ img_path                             |../menu/images|  }";




int main(int argc, char** argv) {

  

    // Parse command line arguments.
    CommandLineParser parser(argc, argv, cmdOptions);
    parser.about(cmdAbout);

    // Load image and init parameters
    String modelPath = parser.get<String>("model_path");
    String configPath = parser.get<String>("config_path");
    String labelsPath = parser.get<string>("label_path");
    int width = parser.get<int>("width");
    int height = parser.get<int>("heigth");
    Scalar mean = parser.get<Scalar>("mean");
    bool swapRB = parser.get<bool>("swap");
    double scale = 1;
    DnnDetector detector(modelPath, configPath, labelsPath, width, height, mean, swapRB, scale);

    // Do something cool.

    String imgName(parser.get<String>("image"));
    Mat image = imread(imgName);
    vector<DetectedObject> res;
    res = detector.Detect(image);
    Mat tmp;
    vector<string> detectedObjects;

   for(int i = 0; i < res.size(); i++) {
        string objClass = "Class: " + std::to_string(res[i].uuid) + " " + res[i].classname;
        string conf = "Confidence: " + std::to_string(res[i].score);

        Point leftbottom(res[i].xLeftBottom, res[i].yLeftBottom);
        Point righttop(res[i].xRightTop, res[i].yRightTop);
        cout << leftbottom << " " << righttop << endl;
        Rect box(leftbottom, righttop);
        cout << objClass << " " << conf << endl;

        rectangle(image, box, Scalar(71, 99, 255), 1, 1, 0);

        image(box).copyTo(tmp);
        //imwrite(std::to_string(res.back().score) + res.back().classname + ".jpg", tmp);
        putText(image, objClass, Size(res[i].xLeftBottom - 10, res[i].yLeftBottom - 25), FONT_HERSHEY_COMPLEX_SMALL, 1,
            Scalar(10, 110, 255), 1, 0);
        putText(image, conf, Size(res[i].xLeftBottom - 10, res[i].yLeftBottom - 2), FONT_HERSHEY_COMPLEX_SMALL, 1,
            Scalar(10, 99, 255), 1, 0);

        detectedObjects.push_back(res[i].classname);


    }

   cout << "DetectedObjects: ";

    for (auto it : detectedObjects) {
        cout << it << " ";
    }

    cout << endl;

        imshow("win", image);
        waitKey(0);
        
       string menuPath = parser.get<String>("menu_path");
       string recipePath = parser.get<String>("recipe_path");
       string imgPath = parser.get<String>("img_path");

        Recipes data(menuPath, recipePath, imgPath);

        data.FindDish(detectedObjects, 3);
        data.SetImg(data.result);
        //data.Show(data.result);

	return 0;
}

