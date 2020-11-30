#pragma once
#include "Matrix.h"
#include <cassert>
#include <iostream>
#include <chrono>
using namespace std;
int main()
{
	Matrix matrixA(
		{
			{1,2},
			{3,4},
			{5,6}
		});

	Matrix matrixB(
		{
			{1,2,3,4},
			{5,6,7,8},
		});


	Matrix matrixC(
		{
			{11,14,17,20},
			{23,30,37,44},
			{35,46,57,68},
		});
	cout << "matrixA* matrixB == matrixC" << (matrixA * matrixB == matrixC) << "passed"<<endl;
	assert(matrixA * matrixB == matrixC);


	//std::async()

		// THE SECOND PART
	int matrix_height = 8000;
	int matrix_width = 4000;
	Matrix matrixX(matrix_height, matrix_width);
	matrixX.fillRandom(0, 100);
	Matrix matrixY(matrix_width, matrix_height);
	matrixY.fillRandom(0, 100);
	auto t1 = std::chrono::high_resolution_clock::now();
	auto matrixZ = matrixX * matrixY;
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "execution time is " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "msec" << std::endl;
	Matrix::enableMultithreading();
	t1 = std::chrono::high_resolution_clock::now();
	auto matrixZ2 = matrixX * matrixY;
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "execution time is " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "msec" << std::endl;
	assert(matrixZ == matrixZ2);
}