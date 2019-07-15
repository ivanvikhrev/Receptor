#include <opencv2/opencv.hpp>
#include "Recipes.hpp"
#include "detectedobject.hpp"
#include "detector.hpp"
#include <fstream>
#include <iostream>
#include "time.h"

using namespace std;
using namespace cv;
using namespace cv::dnn;

const char* cmdAbout = "Sample of OpenCV usage. ";
int vers = 3;
const char* cmdOptions =
"{ i  image                             |../images/20.jpg| image to process                  }"
"{ w  width                             |300| image width for classification    }"
"{ h  heigth                            |300| image heigth fro classification   }"
"{ model_path                           |../detection/sorted_inference_graph.pb|}"
"{ config_path                          |../detection/graph.pbtxt| path to model configuration}"
"{ label_path                           |../detection/labels.txt| path to class labels              }"
"{ mean                                 |""0. 0. 0.""| vector of mean model values       }"
"{ swap                                 |1| swap R and B channels. TRUE|FALSE }"
"{ menu_path                            |../menu/menu.txt|  }"
"{ recipe_path                          |../menu/recipes|  }"
"{ img_path                             |../menu/images|  }";

const int alpha_slider_max = 2;
int alpha_slider;


Mat src1;
Mat src2;
Mat src3;

void on_trackbar(int numb, void*)
{
    if (numb == 0) {
        imshow("Receptor", src1);
   }
    if (numb == 1) {
        imshow("Receptor", src2);
    }
    if (numb == 2) {
        imshow("Receptor", src3);
    }

}




void PutText(Mat& image, string text) {
   
    string word;
   
    ifstream file("../menu/recipes/"+text+".txt");
                      
    string str;                                     
    int i = 0;
    while (getline(file, str)) 
    {                       
        cv::putText(image, str, cv::Point(70, 70 + i*20), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0), 1, 8, false);
        i++;
    }
}

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

   image.copyTo(src1);
    vector<DetectedObject> res;
    res = detector.Detect(image);
    Mat tmp;
    vector<string> detectedObjects;

   for(int i = 0; i < res.size(); i++) {
        string objClass =  std::to_string(res[i].uuid) + " " + res[i].classname;
        string conf = std::to_string(res[i].score);
        Point leftbottom(res[i].xLeftBottom, res[i].yLeftBottom);
        Point righttop(res[i].xRightTop, res[i].yRightTop);
        cout << leftbottom << " " << righttop << endl;
        Rect box(leftbottom, righttop);
        cout << objClass << " " << conf << endl;

        rectangle(image, box, Scalar(204, 153, 0), 2, 1, 0);

        image(box).copyTo(tmp);
        putText(image, objClass, Size(res[i].xLeftBottom - 10, res[i].yLeftBottom - 25), FONT_HERSHEY_COMPLEX_SMALL, 1,
            Scalar(204, 153, 0), 1, 0);
        putText(image, conf, Size(res[i].xLeftBottom - 10, res[i].yLeftBottom - 5), FONT_HERSHEY_COMPLEX_SMALL, 1,
            Scalar(204, 153, 0), 1, 0);

        detectedObjects.push_back(res[i].classname);


    }

   cout << "DetectedObjects: ";

    for (auto it : detectedObjects) {
        cout << it << " ";
    }

    cout << endl;

        //imshow("win", image);
       // waitKey(0);
        
       string menuPath = parser.get<String>("menu_path");
       string recipePath = parser.get<String>("recipe_path");
       string imgPath = parser.get<String>("img_path");

        Recipes data(menuPath, recipePath, imgPath);

        data.FindDish(detectedObjects, 1);

        data.SetImg(data.result);
       
        src2 = data.result[0].img;
        
       // namedWindow("Receptor", 1);
       // waitKey(0);
        
        data.Show(data.result, 0);
        src1 = image;
        imwrite("../save/" + std::to_string(vers) + std::to_string(time(0))  + ".jpg", image);
        
        Mat text = imread("../images/frame.jpg",1);

        PutText(text,  data.result[0].name);

        src3 = text;

        namedWindow("Receptor", WINDOW_AUTOSIZE);
        createTrackbar("Photo", "Receptor", &alpha_slider, alpha_slider_max, on_trackbar);
        on_trackbar(alpha_slider, 0);
        waitKey(0);

     
	return 0;
}

