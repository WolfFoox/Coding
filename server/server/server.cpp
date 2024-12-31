#include <stdio.h>
#include <stdlib.h>
//#include "stdafx.h"
#include "afxres.h"
#include <winsock2.h> // winsock2��ͷ�ļ�
#include <iostream>
#include <map>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

map<SOCKET, string> client; // �洢socket���ǳƶ�Ӧ��ϵ
DWORD WINAPI Threadfun(LPVOID lpParameter);
DWORD WINAPI ThreadSend(LPVOID lpParameter);

int main()
{

	//system("chcp 65001"); // ��������
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
	if (bind(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR) {
		cout << "�������󶨶˿ں�ipʧ��" << endl;
		WSACleanup();
	}
	else
		cout << "server�󶨶˿ں�Ip�ɹ�" << endl;

	// �����˿�
	if (listen(s, 5) != 0) {
		cout << "���ü���״̬ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else
		cout << "���ü���״̬�ɹ���" << endl;

	cout << "���������������У����Ե�......" << endl;

	// ������Ϣ�߳�
	CloseHandle(CreateThread(NULL, 0, ThreadSend, (LPVOID)s, 0, NULL));

	// ѭ�����ܣ��ͻ��˷���������
	while (true) {
		sockaddr_in addrClient;
		len = sizeof(sockaddr_in);
		SOCKET c = accept(s, (sockaddr*)&addrClient, &len);
		if (c == INVALID_SOCKET) { // һ��ʧ�����Ǿͳ��ˣ�Ҳ����ȥ��clean��return
			cout << "��ͻ�������ʧ��" << endl;
			WSACleanup();
			return 0;
		}

		HANDLE hthread = CreateThread(NULL, 0, Threadfun, (LPVOID)c, 0, NULL);
		CloseHandle(hthread); // �رվ����û��
	}

	// �ر����ӣ��ͷ���Դ
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

		// ���͸������û���
		string str = "[Server]: " + string(bufsend);
		for (auto i : client)
			ret = send(i.first, str.data(), 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0); //������ӱ��رգ�����0�����򷵻�socket_error

	return 0;
}
DWORD WINAPI Threadfun(LPVOID lpParameter) {
	SOCKET c = (SOCKET)lpParameter;

	//���ӳɹ�����ʼ������Ϣ
	char bufrecv[100] = { 0 }; //�������ܺͷ�������
	int ret;
	ret = recv(c, bufrecv, 100, 0);
	client[c] = string(bufrecv);

	string bufsend;
	bufsend = "��ӭ[" + client[c] + "]����������";
	cout << bufsend << endl;

	for (auto i : client)
		send(i.first, bufsend.data(), 100, 0);

	ret = 0;
	do {
		char buf[100] = { 0 };
		ret = recv(c, buf, 100, 0);
		cout << "[" << client[c] << "]: " << buf << endl << endl;

		// �����ܵ�����Ϣ�㲥
		string str1 = "[" + client[c] + "]: " + string(buf);
		for (auto i : client)
			send(i.first, str1.data(), 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0); //������ӱ��رգ�����0�����򷵻�socket_error

	string str2 = "[" + client[c] + "]�뿪�����ң�";
	cout << str2 << endl;
	for (auto i : client)
		send(i.first, str2.data(), 100, 0);

	return 0;
}