// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>//预处理器编译指令
#include <cmath>
#include <climits>
int main()
{
    int apples;
    using namespace std;//编译指令 放在外面所有函数都可以访问std
    //double x;
    //x = sqrt(6.25);
    cout << "How many apples do you have?";
    cin >> apples;
    //cout << endl;
    cout << "Hehe you have "<<apples <<"!\n" ;
    cout << "int max: " << INT_MAX;
    return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
