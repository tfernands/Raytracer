#include <iostream>
#include <algorithm>

using namespace std;

void printArray(char *arr, int size){
	int i;
	for (i = 0; i < size-1; i++) 
		std::cout << arr[i] << ", ";
	std::cout << arr[i] << std::endl;
}

void printArray(int *arr, int size){
	int i;
	for (i = 0; i < size-1; i++) 
		std::cout << arr[i] << ", ";
	std::cout << arr[i] << std::endl;
}

int main(){
	int arr_size = 100;
	int arr[arr_size];
	for (int i = 0; i < arr_size; i++) arr[i] = i;
	random_shuffle(&arr[0], &arr[arr_size]);
	printArray(arr, arr_size);

	return 0;
}