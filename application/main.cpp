#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <time.h>
#include <tchar.h>
#include <windows.h>
#include "Recipes.hpp"
#include "detectedobject.hpp"
#include "detector.hpp"

#define BUFSIZE MAX_PATH

using namespace std;
using namespace cv;
using namespace cv::dnn;

const char* cmdAbout = "Sample of OpenCV usage. ";
int vers = 3;
const char* cmdOptions =
"{ i  image                             |../images/2.jpg| image to process                  }"
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

const int alpha_slider_max = 2;
int alpha_slider;


Mat src1;
Mat src2;
Mat src3;


void openURL(string URL)
{
    system((std::string("start ") + URL).c_str());
}

String openFile()
{
    TCHAR Buffer[BUFSIZE];
    DWORD dwRet;
    dwRet = GetCurrentDirectory(BUFSIZE, Buffer);
    char filename[MAX_PATH];

    OPENFILENAME ofn;
    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
    ofn.lpstrFilter = _T("Image files (*.jpg;*.png;*.bmp)\0*.jpg;*.png;*.bmp\0""All files\0*.*\0");
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = _T("Select a File");
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        std::cout << "You chose the file \"" << filename << "\"\n";
    }
    else
    {
        // All this stuff below is to tell you exactly how you messed up above. 
        // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
        switch (CommDlgExtendedError())
        {
        case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
        case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
        case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
        case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
        case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
        case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
        case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
        case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
        case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
        case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
        case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
        case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
        case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
        case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
        case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
        default: std::cout << "You cancelled.\n";
        }
    }
    SetCurrentDirectory(Buffer);

    return (String)filename;
};


void PutText(Mat& image, string text) {

    string word;

    ifstream file("../menu/recipes/" + text + ".txt");

    string str;
    int i = 0;
    while (getline(file, str))
    {
        cv::putText(image, str, cv::Point(70, 70 + i * 20), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0), 1, 2, false);
        i++;
    }
}

struct MouseCallbackState
{
    bool clicked;
    bool flag;
    Rect button;
    String recipeURL;
};

void OnMouse(int event, int x, int y, int flags, void* param)
{
    MouseCallbackState* mouse = (MouseCallbackState*)param;

    switch (event)
    {
    case EVENT_LBUTTONDOWN:

        if (mouse->button.contains(Point(x, y)))
        {
            mouse->clicked = true;
            cout << "Clicked!" << endl;
            if (mouse->flag)
                openURL(mouse->recipeURL);
        }
        break;
    case EVENT_LBUTTONUP:

        if (mouse->button.contains(Point(x, y)))
        {
            mouse->clicked = false;
            cout << "Clicked!" << endl;
        }
        break;

    }

}

MouseCallbackState p;
void on_trackbar(int numb, void* param)
{
    if (numb == 0) {
        p.flag = false;
        imshow("Receptor", src1);

    }
    if (numb == 1) {
        p.flag = false;
        imshow("Receptor", src2);
    }
    if (numb == 2) {
        imshow("Receptor", src3);
        p.flag = true;
        p.button = Rect(0, 0, src3.cols, src3.rows);
    }

}

int main(int argc, char** argv) {


    // Parse command line arguments.
    CommandLineParser parser(argc, argv, cmdOptions);
    parser.about(cmdAbout);

    // Load image and init parameters
    String imgName;
    String modelPath = parser.get<String>("model_path");
    String configPath = parser.get<String>("config_path");
    String labelsPath = parser.get<string>("label_path");
    int width = parser.get<int>("width");
    int height = parser.get<int>("heigth");
    Scalar mean = parser.get<Scalar>("mean");
    bool swapRB = parser.get<bool>("swap");
    double scale = 1;
    DnnDetector detector(modelPath, configPath, labelsPath, width, height, mean, swapRB, scale);

    //Fil loading
    p.clicked = false;
    p.flag = false;
    Mat background = imread("../images/button.jpg", 1);
    //Mat background(200, 400, CV_8UC3, Scalar(255, 255, 255));
    p.button = Rect(0, 0, background.cols, background.rows);
    background(p.button) = Vec3b(255, 255, 255);
    rectangle(background(p.button), p.button, Scalar(204, 153, 0), 3);
    String buttonText = "Open file";
    putText(background(p.button), buttonText, Point(p.button.width * 0.35, p.button.height * 0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
    imshow("start screen", background);
    setMouseCallback("start screen", OnMouse, &p);
    while (imgName.empty()) {
        waitKey(1);
        if (p.clicked)
            imgName = openFile();
    }
    destroyWindow("start screen");


    Mat image = imread(imgName);
    vector<DetectedObject> res;
    res = detector.Detect(image);
    Mat tmp;
    vector<string> detectedObjects;

    for (int i = 0; i < res.size(); i++) {
        string objClass = std::to_string(res[i].uuid) + " " + res[i].classname;
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

    image.copyTo(src1);
    string menuPath = parser.get<String>("menu_path");
    string recipePath = parser.get<String>("recipe_path");
    string imgPath = parser.get<String>("img_path");

    Recipes data(menuPath, recipePath, imgPath);

    data.FindDish(detectedObjects, 1);
    data.SetImg(data.result);

    src2 = data.result[0].img;


    data.Show(data.result, 0);
    imwrite("../save/" + std::to_string(vers) + std::to_string(time(0)) + ".jpg", image);

    Mat text = imread("../images/frame.jpg", 1);
    PutText(text, data.result[0].name);
    src3 = text;

    p.recipeURL = "https://www.google.com";
    p.flag = false;
    namedWindow("Receptor", WINDOW_AUTOSIZE);
    setMouseCallback("Receptor", OnMouse, &p);
    createTrackbar("Photo", "Receptor", &alpha_slider, alpha_slider_max, on_trackbar);
    on_trackbar(alpha_slider, 0);
    waitKey(0);

    return 0;
}