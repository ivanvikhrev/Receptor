#include <opencv2/opencv.hpp>
#include "Recipes.hpp"
#include "classificator.hpp"
#include "detectedobject.hpp"
#include "detector.hpp"
#include <fstream>
#include <iostream>


using namespace std;
using namespace cv;
using namespace cv::dnn;

const char* cmdAbout = "Sample of OpenCV usage. ";



const char* cmdOptions =
"{ i  image                             |C:/GitHub/Receptor/images/5.jpg| image to process                  }"
"{ w  width                             |300| image width for classification    }"
"{ h  heigth                            |300| image heigth fro classification   }"
"{ model_path                           |C:/GitHub/Receptor/object_detection/common/mobilenet-ssd/caffe/mobilenet-ssd.caffemodel|}"
"{ config_path                          |C:/GitHub/Receptor/object_detection/common/mobilenet-ssd/caffe/mobilenet-ssd.prototxt| path to model configuration}"
"{ label_path                           |C:/GitHub/Receptor/object_detection/common/mobilenet-ssd.labels.txt| path to class labels              }"
"{ mean                                 |""127.5 127.5 127.5""| vector of mean model values       }"
"{ swap                                 |0| swap R and B channels. TRUE|FALSE }"
"{ menu_path                            |C:/GitHub/Receptor/recipes/menu.txt|  }"
"{ recipe_path                          |C:/GitHub/Receptor/recipes|  }"
"{ img_path                             |C:/GitHub/Receptor/recipes/images|  }";


string menuPath = "C:/GitHub/Receptor/recipes/1_recipes.txt";
string recipePath = "C:/GitHub/Receptor/recipes";


int main(int argc, char** argv) {

    setlocale(LC_ALL, "Ukrainian");;

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
    double scale = 1 / 127.50223128904757;
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
        //imwrite("C:/Users/temp2019/Documents/GitHub/Receptor/images/" + std::to_string(res.back().score) + res.back().classname + ".jpg", tmp);
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

        detectedObjects.push_back("potato");
        detectedObjects.push_back("cheese");
        data.FindDish(detectedObjects, 3);
        data.SetImg(data.result);
        data.Show(data.result);



	return 0;
}

/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

// @function main /
int main(int argc, char** argv)
{
    /// Load image and template
    img = imread("C:/GitHub/Receptor/images/5.jpg", 1);
    templ = imread("C:/GitHub/Receptor/images/2.jpg", 1);

    /// Create windows
    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);

    /// Create Trackbar
    char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

    MatchingMethod(0, 0);

    waitKey(0);
    return 0;
}


 // @function MatchingMethod
 // @brief Trackbar callback
 
void MatchingMethod(int, void*)
{
    /// Source image to display
    Mat img_display;
    img.copyTo(img_display);

    /// Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);

    /// Do the Matching and Normalize
    matchTemplate(img, templ, result, match_method);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
   // cv::threshold(result, result, 0.8, 1, match_method);
    
    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    cout << maxVal;

    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
    }
    else
    {
        matchLoc = maxLoc;
    }

    /// Show me what you got
    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

    imshow(image_window, img_display);
    imshow(result_window, result);

    return;
}

*/
