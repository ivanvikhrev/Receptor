#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <algorithm>

using namespace std;

class Dish
{

public:
    string name;
    vector<string> discription;
    int count;
    string recipe;

    Dish();
    ~Dish();
    void Show();
};

class Recipes
{
public:
    vector<Dish> menu;
    vector<Dish> result;
    string recipePath;
    string menuPath;

    Recipes();
    ~Recipes();
    Recipes(string fileNameMenu, string fileNameRecipe);


    bool SetMenu(string file_name);
    void Show(vector<Dish> dishes);
    void SetRecipe(vector<Dish> some);

    vector<string> FindDish(vector<string> dis, int count);
 
};