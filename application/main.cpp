#include "test.hpp"
#include <opencv2/opencv.hpp>
#include "Recipes.hpp"
#include "classificator.hpp"
#include "detectedobject.hpp"
#include "detector.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

const char* cmdAbout = "Sample of OpenCV usage. ";


const char* cmdOptions =
"{ i  image                             |C:/Users/temp2019/Documents/GitHub/Receptor/images/1.jpg| image to process                  }"
"{ w  width                             |300| image width for classification    }"
"{ h  heigth                            |300| image heigth fro classification   }"
"{ model_path                           |C:/Users/temp2019/Documents/GitHub/CV-SUMMER-CAMP-build/data/object_detection/common/mobilenet-ssd/caffe/mobilenet-ssd.caffemodel|}"
"{ config_path                          |C:/Users/temp2019/Documents/GitHub/CV-SUMMER-CAMP-build/data/object_detection/common/mobilenet-ssd/caffe/mobilenet-ssd.prototxt| path to model configuration}"
"{ label_path                           |        | path to class labels              }"
"{ mean                                 |""127.5 127.5 127.5""| vector of mean model values       }"
"{ swap                                 |FALSE| swap R and B channels. TRUE|FALSE }";


/*
const char* cmdOptions =
"{ i  image                             |C:/Users/temp2019/Documents/GitHub/Receptor/images/7.jpg| image to process                  }"
"{ w  width                             |227| image width for classification    }"
"{ h  heigth                            |227| image heigth fro classification   }"
"{ model_path                           |C:/Users/temp2019/Documents/GitHub/CV-SUMMER-CAMP-build1/data/classification/squeezenet/1.1/caffe/squeezenet1.1.caffemodel|}"
"{ config_path                          |C:/Users/temp2019/Documents/GitHub/CV-SUMMER-CAMP-build1/data/classification/squeezenet/1.1/caffe/squeezenet1.1.prototxt| path to model configuration}"
"{ label_path                           |        | path to class labels              }"
"{ mean                                 |""104.0 117.0 123.0""| vector of mean model values       }"
"{ swap                                 |FALSE| swap R and B channels. TRUE|FALSE }";
*/


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
    double scale = 1 / 127.50223128904757;
    DnnDetector detector(modelPath, configPath, labelsPath, width, height, mean, swapRB, scale);

    // Do something cool.

        String imgName(parser.get<String>("image"));
        Mat image = imread(imgName);
        vector<DetectedObject> res;
        res = detector.Detect(image);
        Mat tmp;

        while (!res.empty()) {
            string objClass = "Class: " + std::to_string(res.back().uuid) + " " + res.back().classname;
            string conf = "Confidence: " + std::to_string(res.back().score);
            Point leftbottom(res.back().xLeftBottom, res.back().yLeftBottom);
            Point righttop(res.back().xRightTop, res.back().yRightTop);
            cout << leftbottom << " " << righttop << endl;
            Rect box(leftbottom, righttop);
            cout << objClass << " " << conf << endl;
            rectangle(image, box, Scalar(71, 99, 255), 1, 1, 0);

            if (!box.empty()) {
                image(box).copyTo(tmp);
                imwrite("C:/Users/temp2019/Documents/GitHub/Receptor/images/" + res.back().classname + ".jpg", tmp);


                putText(image, objClass, Size(res.back().xLeftBottom - 10, res.back().yLeftBottom - 20), FONT_HERSHEY_COMPLEX_SMALL, 1,
                    Scalar(10, 110, 255), 1, 0);
                putText(image, conf, Size(res.back().xLeftBottom - 10, res.back().yLeftBottom - 2), FONT_HERSHEY_COMPLEX_SMALL, 1,
                    Scalar(10, 99, 255), 1, 0);
                res.pop_back();
            }
            
        }
        imshow("win", image);
        waitKey(0);
    

    
    
    
    
    /*
    // Process input arguments
    CommandLineParser parser(argc, argv, cmdOptions);
    parser.about(cmdAbout);

  
    // Load image and init parameters
    String imgName(parser.get<String>("image"));
    String model_path(parser.get<String>("model_path"));
    String config_path(parser.get<String>("config_path"));
    String path_label(parser.get<String>("label_path"));
    int width(parser.get<int>("width"));
    int height(parser.get<int>("heigth"));
    Scalar mean(parser.get<Scalar>("mean"));
    int swapRB(parser.get<int>("swap"));

    DnnClassificator dnn(model_path, config_path,
        path_label, width, height, mean, swapRB);

    Mat image = imread(imgName);
    Mat prob;
    //Image classification

    prob = dnn.Classify(image);

    //Show result
    Point classIdPoint;
    double confidence;
    minMaxLoc(prob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);

    int classId = classIdPoint.x;

    std::cout << "Class: " << classId << '\n';
    std::cout << "Confidence: " << confidence << '\n';

    std::string name;

    std::ifstream in("C:/Users/temp2019/Documents/GitHub/CV-SUMMER-CAMP/data/squeezenet1.1.labels");
    int count = 0;
    if (in.is_open())
    {
        while (getline(in, name))
        {
            if (count == classId)
            {
                break;
            }
            count++;

        }
    }
    in.close();



    Mat res = image;
    putText(res, "Class: " + to_string(classId), Point(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 0, 250));
    putText(res, "Confidence: " + to_string(confidence), Point(30, 60), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 0, 250));
    putText(res, name, Point(30, 90), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 0, 250));


    imshow("image", res);
    waitKey();

    /*
    HELLO();

    Recipes data;

    vector<string> tmp;
    tmp.push_back("fig");
    tmp.push_back("pepper");
    data.SetMenu("C:\\Users\\temp2019\\Documents\\GitHub\\Receptor-build\\recipes\\1_recipes.txt");
    data.Show(data.menu);
    vector<string> res;
    res = data.FindDish(tmp, 2);

    
    for (auto tmp : res) {
        cout << tmp <<" ";
    }
    
    data.Show(data.result);
    */
    

	return 0;
}