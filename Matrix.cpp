#include "Matrix.h"
#include <iostream>
#include <future>
using namespace std;
bool Matrix::isEnabledMultithreading = false;
int Matrix::m_num_threads = max(int(std::thread::hardware_concurrency()), 1);
Matrix::Matrix(const std::initializer_list<std::initializer_list<int>>& list):Matrix(list.size(), (list.begin())->size())
{
	int i = 0;
	for (auto it1 : list) 
	{
		for (auto it2 : it1) 
		{
			m_arr[i] = it2;
			++i;
		}
	}
}
Matrix::Matrix(int height, int width ) :m_width(width), m_height(height)
{
	m_arr = new int[m_width * m_height];
};
bool  Matrix::operator ==(const Matrix& lhs) const
{
	if ((m_width != lhs.m_width) || m_height != lhs.m_height) 
	{
		return false;
	}
	for (int i = 0; i < m_width * m_height; ++i) 
	{
		if (m_arr[i] != lhs.m_arr[i])
		{
			return false;
		}
	}
	return true;
}
Matrix Matrix::multiplyOneThread(const Matrix& obj) const
{
	Matrix result(m_height, obj.m_width);
	if (m_width != obj.m_height)
		return result;
	int width_this = m_width;
	int a, b;
	multiplySubmatrix(obj, &result, 0, result.m_height, width_this, this->m_arr);
	return result;
}
void Matrix::enableMultithreading()
{
	isEnabledMultithreading = true;
}
void Matrix::disableMultithreading()
{
	isEnabledMultithreading = false;
}
Matrix Matrix::operator * (const Matrix& obj) const
{ 
	if (!isEnabledMultithreading)
		return multiplyOneThread(obj);
	else
		return multiplyMultiThread(obj);
}
void Matrix::multiplySubmatrix(const Matrix& obj,  Matrix* result, int a, int b, int width_this, int* arr)
{
	int width_res = result->m_width;
	//int width_this = m_width;
	int width_obj = obj.m_width;
	for (int i = a; i < b; ++i)
	{
		for (int j = 0; j < result->m_width; ++j)
		{
			*(result->m_arr + i * (width_res)+j) = 0;
			for (int k = 0; k < width_this; ++k)
			{
				*(result->m_arr + i * (width_res)+j) += *(arr + i * (width_this)+k) * *(obj.m_arr + k * (width_obj)+j);
			}
		}
	}
}

Matrix Matrix::multiplyMultiThread(const Matrix& obj) const
{

	Matrix result(m_height, obj.m_width); 
	if (m_width != obj.m_height)
		return result;
	vector<future<void>> futures;
	int width_res = result.m_width;
	int width_this = m_width;
	int width_obj = obj.m_width;
	int a, b;
	for (int i = 0; i < Matrix::m_num_threads; ++i)
	{
		a = result.m_height * i / m_num_threads;
		b = result.m_height * (i + 1) / m_num_threads;
		futures.push_back(std::async(launch::async, multiplySubmatrix, obj, &result, a, b, width_this, this->m_arr));
	}
	for (int i = 0; i < m_num_threads; ++i)
	{
		futures[i].get();
	}
	return result;
};
Matrix::Matrix(const Matrix& other):Matrix(other.m_height, other.m_width)
{
	for (int i = 0; i < m_width * m_height; ++i)
	{
		m_arr[i] = other.m_arr[i];
	}
}
int& Matrix::index(int i, int j) const
{
	return m_arr[i * (m_width) +  j];
}
Matrix::~Matrix()
{
	delete[] m_arr;
}
void Matrix::fillRandom(int a, int b)
{
	for (int i = 0; i < m_width * m_height; ++i) 
	{
		m_arr[i] = rand() % (b - a) + a;
	}
}
std::string Matrix::to_string() const 
{
	string res = "";
	for (int i = 0; i < m_height; ++i) 
	{
		for (int j = 0; j < m_width; ++j) 
		{
			res += std::to_string(m_arr[i * j]) + " ";
		}
		res += "\n";
	}
	return res;
}
void Matrix::setNumThreads(int num_threads)
{
	Matrix::m_num_threads = num_threads;
}