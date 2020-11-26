#include "Matrix.h"
#include <iostream>
#include <future>
using namespace std;
bool Matrix::isEnabledMultithreading = false;
Matrix::Matrix(initializer_list<initializer_list<int>> list)
{
	m_height = list.size();
	m_width = (list.begin())->size();
	m_arr = new int[m_width * m_height];
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
	for (int i = 0; i < result.m_height; ++i)
	{
		for (int j = 0; j < result.m_width; ++j)
		{
			*result.index(i, j) = 0;
			for (int k = 0; k < m_width; ++k)
			{
				*result.index(i, j) += *((*this).index(i, k)) * *(obj.index(k, j));
			}
			
		}
	}
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
void Matrix::multiParallel(const Matrix& obj,  Matrix& result, int a, int b) const
{
	int width_res = result.m_width;
	int width_this = m_width;
	int width_obj = obj.m_width;
	for (int i = a; i < b; ++i)
	{
		for (int j = 0; j < result.m_width; ++j)
		{
			*(result.m_arr + i * (width_res)+j) = 0;
			for (int k = 0; k < m_width; ++k)
			{
				*(result.m_arr + i * (width_res)+j) += *(m_arr + i * (width_this)+k) * *(obj.m_arr + k * (width_obj)+j);
			}
			//cout << i << " " << j <<" "<< *result.index(i, j) << endl;
		}
	}
}

void multiParallel_arrs(int width_res, int width_this, int width_obj, int* result_m_arr, int* this_m_arr, int* obj_m_arr,  int a, int b) 
{
	for (int i = a; i < b; ++i)
	{
		for (int j = 0; j < width_res; ++j)
		{
			*(result_m_arr + i * (width_res)+j) = 0;
			for (int k = 0; k < width_this; ++k)
			{
				*(result_m_arr + i * (width_res)+j) += *(this_m_arr + i * (width_this)+k) * *(obj_m_arr + k * (width_obj)+j);
			}
		}
	}
}

Matrix Matrix::multiplyMultiThread(const Matrix& obj) const
{

	int num_threads = 5;
	Matrix result(m_height, obj.m_width);
	if (m_width != obj.m_height)
		return result;
	vector<future<void>> futures;
	int width_res = result.m_width;
	int width_this = m_width;
	int width_obj = obj.m_width;
	int a, b;
	for (int i = 0; i < num_threads - 1; ++i)
	{
		a = result.m_height * i / num_threads;
		b = result.m_height * (i + 1) / num_threads;
		futures.push_back(std::async(launch::async, multiParallel_arrs, width_res, width_this, width_obj, result.m_arr, this->m_arr, obj.m_arr, a, b));
	}
	a = result.m_height * (num_threads - 1) / num_threads;
	b = result.m_height;
	futures.push_back(std::async(launch::async, multiParallel_arrs, width_res, width_this, width_obj, result.m_arr, this->m_arr, obj.m_arr, a, b));
	
	for (int i = 0; i < num_threads ; ++i)
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
int* Matrix::index(int i, int j) const
{
	return &m_arr[i * (m_width) +  j];
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