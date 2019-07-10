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

    Dish();
    ~Dish();
    void Show();
};

class Recipes
{
public:
    vector<Dish> menu;
    vector<Dish> result;

    Recipes();
    ~Recipes();

    bool SetMenu(string file_name);
    void Show(vector<Dish> dishes);

    vector<string> FindDish(vector<string> dis, int count);
 
};