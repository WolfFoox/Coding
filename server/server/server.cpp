#include <stdio.h>
#include <stdlib.h>
//#include "stdafx.h"
#include "afxres.h"
#include <winsock2.h> // winsock2的头文件
#include <iostream>
#include <map>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

map<SOCKET, string> client; // 存储socket和昵称对应关系
DWORD WINAPI Threadfun(LPVOID lpParameter);
DWORD WINAPI ThreadSend(LPVOID lpParameter);

int main()
{

	//system("chcp 65001"); // 设置中文
	// 加载winsock环境
	WSAData wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "加载网络环境失败" << endl;
		return 0;
	}
	else
		cout << "加载网络环境成功" << endl;

	// 创建套接字
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "创建套接字失败" << endl;
		WSACleanup();
	}
	else
		cout << "创建套接字成功" << endl;

	// 给套接字绑定ip地址和端口：bind函数
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("192.168.56.1");

	int len = sizeof(sockaddr_in);
	if (bind(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
		cout << "服务器绑定端口和ip失败" << endl;
		WSACleanup();
	}
	else
		cout << "server绑定端口和Ip成功" << endl;

	// 监听端口
	if (listen(s, 5) != 0) {
		cout << "设置监听状态失败！" << endl;
		WSACleanup();
	}
	else
		cout << "设置监听状态成功！" << endl;

	cout << "服务器监听连接中，请稍等......" << endl;

	// 发送消息线程
	CloseHandle(CreateThread(NULL, 0, ThreadSend, (LPVOID)s, 0, NULL));

	// 循环接受：客户端发来的连接
	while (true) {
		sockaddr_in addrClient;
		len = sizeof(sockaddr_in);
		SOCKET c = accept(s, (sockaddr*)&addrClient, &len);
		if (c == INVALID_SOCKET) { // 一个失败我们就撤退，也可以去掉clean和return
			cout << "与客户端连接失败" << endl;
			WSACleanup();
			return 0;
		}

		HANDLE hthread = CreateThread(NULL, 0, Threadfun, (LPVOID)c, 0, NULL);
		CloseHandle(hthread); // 关闭句柄，没用
	}

	// 关闭连接，释放资源
	closesocket(s);
	WSACleanup();
	return 0;
}

DWORD WINAPI ThreadSend(LPVOID lpParameter) {
	SOCKET c = (SOCKET)lpParameter;

	int ret = 0;
	do {
		char bufsend[100] = { 0 };
		cin.getline(bufsend, 100);

		// 发送给所有用户端
		string str = "[Server]: " + string(bufsend);
		for (auto i : client)
			ret = send(i.first, str.data(), 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0); //如果连接被关闭，返回0，否则返回socket_error

	return 0;
}
DWORD WINAPI Threadfun(LPVOID lpParameter) {
	SOCKET c = (SOCKET)lpParameter;

	//连接成功，开始发送消息
	char bufrecv[100] = { 0 }; //用来接受和发送数据
	int ret;
	ret = recv(c, bufrecv, 100, 0);
	client[c] = string(bufrecv);

	string bufsend;
	bufsend = "欢迎[" + client[c] + "]加入聊天室";
	cout << bufsend << endl;

	for (auto i : client)
		send(i.first, bufsend.data(), 100, 0);

	ret = 0;
	do {
		char buf[100] = { 0 };
		ret = recv(c, buf, 100, 0);
		cout << "[" << client[c] << "]: " << buf << endl << endl;

		// 将接受到的信息广播
		string str1 = "[" + client[c] + "]: " + string(buf);
		for (auto i : client)
			send(i.first, str1.data(), 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0); //如果连接被关闭，返回0，否则返回socket_error

	string str2 = "[" + client[c] + "]离开聊天室！";
	cout << str2 << endl;
	for (auto i : client)
		send(i.first, str2.data(), 100, 0);

	return 0;
}