#include "Recipes.hpp"

Recipes::Recipes()
{
}


Recipes::~Recipes()
{
}

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

void Recipes::Show(vector<Dish> dishes)
{
    for (auto it: dishes) {
        it.Show();
    }
}



vector<string> Recipes::FindDish(vector<string> dis, int count)
{

    vector<string> res;

    for (int i = 0; i < menu.size(); i++) {
        menu[i].count = 0;
        for (int j = 0; j < menu[i].discription.size(); j++) {
            for (int k = 0; k < dis.size(); k++) {
                if (menu[i].discription[j] == dis[k]) {
                    menu[i].count++;
                }
            }
        }

    }

    std::sort(menu.begin(), menu.end(),
        [](const Dish& a, const Dish& b) { return a.count > b.count; });

    if (count > menu.size()) count = menu.size();

    for (int i = 0; i < count; i++) {
        result.push_back(menu[i]);
        res.push_back(menu[i].name);
    }

    return res;
}



Dish::Dish()
{
}

Dish::~Dish()
{
}

void Dish::Show()
{
    cout << "\t\t|name:| " << this->name << endl;
    cout << "\t\t|disc:| ";
    for (int j = 0; j < this->discription.size(); j++) {
        cout << this->discription[j] << " ";
    }
    cout << endl;
    cout << endl;

}
