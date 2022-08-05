#include <iostream>
#include <cmath>
#define num1 2000000000000000000000000000000
int main()
	{
	using namespace std;
	char x = 'M';//双引号是字符串哈
	int y = 'M';
	cout << x<<endl;
	cout << oct << y << endl;//八进制
	cout << hex << y << endl ;//十六进制
	cout << dec <<y << endl;//十进制
	//const unsigned long long num2 = 2000000000000000000000000000000;
	const int million = 1000000;
	//different significant figure
	double f1 = sqrt(5);
	float f2 = sqrt(5);
	cout.setf(ios_base::fixed, ios_base::floatfield);
	cout << f1 << ' ' << f2 << endl;
	cout << f1 * million << ' ' << f2 * million << endl;
	//signed 
	unsigned long long add1 = LLONG_MAX;
	signed int add2= -1;
	cout <<add1<<' ' << add1 + add2 << endl;
	cin.get();
	return 0;
	}