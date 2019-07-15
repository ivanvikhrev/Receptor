#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Dish
{

public:
    string name;
    vector<string> discription;
    int count;
    string recipe;
    Mat img;

    Dish();
    ~Dish();
    void Show(int flag);
};

class Recipes
{
public:
    vector<Dish> menu;
    vector<Dish> result;
    string recipePath;
    string menuPath;
    string imgPath;

    Recipes();
    ~Recipes();
    Recipes(string fileNameMenu, string dirNameRecipe, string dirNameImg);


    bool SetMenu(string file_name);
    void Show(vector<Dish> dishes, int flag);
    void SetRecipe(vector<Dish>& some);
    void SetImg(vector<Dish>& some);

    vector<string> FindDish(vector<string> dis, int count);

};