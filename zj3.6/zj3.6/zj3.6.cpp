#include<iostream>
#include<cmath>

#include <iostream>
using namespace std;

int main()
{
    const double finfint = 2.333;
    double x[9], f[9],y[9];
    for (int i = 0; i < 9; i++) {
        cin>> x[i]>>f[i];
    }
    for (int i = 0; i < 9; i++) {
        y[i] = log(f[i] - finfint);
        cout << "y" << i << "=" <<y[i] <<endl;
        
    }
    for (int i = 0; i < 9; i++) {
        cout << "delta" << i << "=" << y[i] - (-0.51 * x[i] - 1.53)<<endl;
    }
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
