#include <stdio.h>
#include <stdlib.h>
//#include "stdafx.h"
#include "afxres.h"
#include <winsock2.h> // winsock2��ͷ�ļ�
#include <iostream>
#include <map>
#include<string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

map<SOCKET, string> client;
DWORD WINAPI Threadfun(LPVOID lpParameter);

int main()
{
	// system("chcp 65001");

	// ����winsock����
	WSAData wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "�������绷��ʧ��" << endl;
		return 0;
	}
	else
		cout << "�������绷���ɹ�" << endl;

	// �����׽���
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "�����׽���ʧ��" << endl;
		WSACleanup();
	}
	else
		cout << "�����׽��ֳɹ�" << endl;

	// ���׽��ְ�ip��ַ�Ͷ˿ڣ�bind����
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("192.168.56.1");

	int len = sizeof(sockaddr_in);
	if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
		cout << "�ͻ�������ʧ��" << endl;
		WSACleanup();
		return 0;
	}
	else
		cout << "�ͻ������ӳɹ�" << endl;

	// ���ͺͽ������ݼ���
	string name;
	char bufrecv[100] = { 0 };
	cout << "����������ǳƣ�";
	getline(cin, name); // ����һ���У������пո�
	send(s, name.data(), 100, 0);
	int ret;

	// �������Ӻ󣬴����߳����ڽ������ݣ����߳�������������
	CloseHandle(CreateThread(NULL, 0, Threadfun, (LPVOID)s, 0, NULL));

	// whileѭ����������
	ret = 0;
	do {
		char bufrecv[100] = { 0 };
		cin.getline(bufrecv, 100);
		ret = send(s, bufrecv, 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0); //������ӱ��رգ�����0�����򷵻�socket_error

	// �ر����ӣ��ͷ���Դ
	closesocket(s);
	WSACleanup();
	return 0;
}

DWORD WINAPI Threadfun(LPVOID lpParameter) {
	SOCKET c = (SOCKET)lpParameter;
	int ret = 0;
	do {
		char buf[100] = { 0 };
		ret = recv(c, buf, 100, 0);
		cout << buf << endl << endl;
	} while (ret != SOCKET_ERROR && ret != 0); //������ӱ��رգ�����0�����򷵻�socket_error

	return 0;
}