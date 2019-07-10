#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include "stdlib.h"

using namespace std;

class Dish
{

public:

    string name;
    vector<string> discription;

    Dish();

    ~Dish();
};

class Recipes
{
public:
    vector<Dish> menu;

    Recipes();
    ~Recipes();

    bool SetMenu(string file_name);
    void ShowMenu();
    string FindDish(vector<string> dis, int count);

};