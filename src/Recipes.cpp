#include "Recipes.hpp"

Recipes::Recipes()
{

}

Recipes::Recipes(string fileNameMenu, string fileNameRecipe)
{
    menuPath = fileNameMenu;
    recipePath = fileNameRecipe;
    SetMenu(menuPath);
    SetRecipe(menu);
}


Recipes::~Recipes()
{
}

bool Recipes::SetMenu(string fileName)
{
    menuPath = fileName;
    std::ifstream file;
    file.open(fileName);


    if (!file.is_open()) return false;

    string word;
    vector<string> tmp;
    Dish tmpDish;

    int k = 0;

    while (file >> word)
    {

        if (word[0] == '<') {

            word.erase(word.find('<'), 1);
            tmpDish.name = word;
        }

        if (word[0] == '#') {
            word.erase(word.find('#'), 1);
            tmpDish.discription.push_back(word);
        }

        if (word[0] == '>') {
            menu.push_back(tmpDish);
            tmpDish.discription.clear();
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

void Recipes::SetRecipe(vector<Dish> some)
{
    for (int i = 0; i < some.size(); i++) {
        std::ifstream myFile(recipePath + "/" + some[i].name + ".txt");
        if (!myFile.is_open()) {
            std::cout << "ERROR:";
        }
        myFile.seekg(0, std::ios::end);
        size_t size = myFile.tellg();
        std::string myKernelCode(size, ' ');
        myFile.seekg(0);
        myFile.read(&myKernelCode[0], size);
        myFile.close();

        some[i].recipe =  myKernelCode;
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
    cout << "\t\t|reci:| " << this->recipe;
    cout << endl;
    cout << endl;

}
