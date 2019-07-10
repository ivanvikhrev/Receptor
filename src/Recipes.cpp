#include "Recipes.hpp"



bool Recipes::SetMenu(string file_name)
{
    std::ifstream file;
    file.open(file_name);


    if (!file.is_open()) return false;

    string word;
    vector<string> tmp;
    Dish tmp_dish;

    int k = 0;

    while (file >> word)
    {
        
        if (word[0] == '<') {
             
            word.erase(word.find('<'), 1);
            tmp_dish.name = word;
        }

        if (word[0] == '#') {
            word.erase(word.find('#'), 1);
            tmp_dish.discription.push_back(word);
        }

        if (word[0] == '>') {
            menu.push_back(tmp_dish);
            tmp_dish.discription.clear();
        }
    }

    return true;
}

void Recipes::ShowMenu()
{
    for (int i = 0; i < menu.size(); i++) {
        cout << "Name: " << menu[i].name<< endl; 
        cout << "Discription: ";
        for (int j = 0; j < menu[i].discription.size(); j++) {
            cout << menu[i].discription[j] <<" ";
        }
        cout << endl;
    }
}

int compare(const void * x1, const void * x2)   // функция сравнения элементов массива
{
    return (*(int*)x1 - *(int*)x2);              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}

string Recipes::FindDish(vector<string> dis, int count)
{
    
    vector<int> repeat;

    vector<string> res;


    for (int i = 0; i < menu.size(); i++) {
        repeat.push_back(0);
        for (int j = 0; j < menu[i].discription.size(); j++) {
            for (int k = 0; k < dis.size(); k++) {
                if (menu[i].discription[i] == dis[k]) {
                    repeat[i]++;
                }

                  }
       }

    }

    qsort(&repeat, repeat.size(), sizeof(int), compare);

    
    return string();
}

Recipes::Recipes()
{
}


Recipes::~Recipes()
{
}

Dish::Dish()
{
}

Dish::~Dish()
{
}
