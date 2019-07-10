#include "test.hpp"
#include <opencv2/opencv.hpp>
#include "Recipes.hpp"

int main() {

    HELLO();

    Recipes data;

    vector<string> tmp;
    tmp.push_back("dis1");
    tmp.push_back("dis2");
    data.SetMenu("C:\\Build\\Receptor-build\\recipes.txt");
    data.Show(data.menu);
    vector<string> res;
    res = data.FindDish(tmp, 2);

    for (auto tmp : res) {
        cout << tmp <<" ";
    }

    data.Show(data.result);
   
	return 0;
}