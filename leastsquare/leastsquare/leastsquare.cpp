// leastsquare.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<cmath>
using namespace std;

int main()
{
    int n;
    cout << "请输入数据个数:";
    cin >> n;
    double* x, * y;
    x = new double[n];
    y = new double[n];
    cout << "请输入数据" << endl;
    double sumx = 0, sumy = 0, sumxy = 0, sumxx = 0;
    for (int i = 0; i < n; i++) {
        cin >> x[i] ;
        sumx += x[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> y[i];
        sumy += y[i];
        sumxy += x[i] * y[i];
        sumxx += x[i] * x[i];
    }
    double avex = sumx / n, avey = sumy / n;
    double k = (n * sumxy - sumx * sumy) / (n * sumxx - sumx * sumx);
    double b = avey - k * avex;
    cout << "最小二乘法拟合直线为：y=" << k << "x+" << b << endl;
    cout << "平均值x:" << avex << endl;
    cout << "平均值y:" << avey << endl;

    for (int i = 0; i < n; i++) {
        cout << "delta" << i + 1 << ":"<<(y[i] - (k * x[i] + b)) << endl;
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
