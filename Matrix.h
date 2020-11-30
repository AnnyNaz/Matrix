#include <initializer_list>
#include <string>
#include <cassert>

class Matrix
{
public:
	Matrix(int width, int height);
	Matrix(const std::initializer_list<std::initializer_list<int>>&);
	Matrix operator * (const Matrix& obj) const;
	bool operator ==(const Matrix& lhs) const;
	Matrix(const Matrix& other);
	int& index(int i, int j) const;
	void fillRandom(int a, int b);
	static bool isEnabledMultithreading;
	static void enableMultithreading();
	static void disableMultithreading();
	static void setNumThreads(int num_threads);
	std::string to_string() const;
	~Matrix();
	static int m_num_threads;
private:
	Matrix  multiplyOneThread(const Matrix& obj) const;
	Matrix  multiplyMultiThread(const Matrix& obj) const;
	static void multiplySubmatrix(const Matrix& obj, Matrix* result, int a, int b, int width_this, int* arr);
	int* m_arr = nullptr;
	int m_width = 0;
	int m_height = 0;
};