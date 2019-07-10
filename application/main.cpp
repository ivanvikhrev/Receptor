#include "test.hpp"
#include <opencv2/opencv.hpp>
#include "Recipes.hpp"

int main() {

	HELLO();
    
    Recipes data;

   data.SetMenu("C:\\Users\\temp2019\\Documents\\GitHub\\recipes.txt");
   data.ShowMenu();
    
	return 0;
}