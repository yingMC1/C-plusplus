#include <winsock2.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>

using namespace std;

string GetLocalIP() {
	char hostname[256];
	gethostname(hostname, sizeof(hostname));
	
	hostent* host = gethostbyname(hostname);
	if (host == nullptr) return "��ȡʧ��";
	
	for (long long i = 0; host->h_addr_list[i] != nullptr; i++) {
		in_addr addr;
		memcpy(&addr, host->h_addr_list[i], sizeof(addr));
		string ip = inet_ntoa(addr);
		
		if (ip != "127.0.0.1" && ip.substr(0, 3) != "169") {
			return ip;
		}
	}
	return "δ�ҵ���ЧIP";
}

long long main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	
	string myIP = GetLocalIP();
	cout << "����IP: " << myIP << endl;
	
	WSACleanup();
	system("pause");
	return 0;
}
