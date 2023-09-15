#pragma once
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock2.h>
#include<ws2tcpip.h>
#include<iostream>
#include <thread>

#pragma comment(lib,"ws2_32.lib")

namespace HalfPeople
{
	namespace Net
	{
		namespace UDP
		{
			class H_IPV4
			{
			public:
				bool Init()
				{

					if (WSAStartup(MAKEWORD(1, 1), &wd) != 0)
					{
						std::cout << "WSAStartup  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					if (LOBYTE(wd.wVersion) != 1 || HIBYTE(wd.wVersion) != 1) {
						WSACleanup();
						return 0;
					}

					s = socket(AF_INET, SOCK_DGRAM, 0);

					return 1;
				}
				bool Server_CreateListen(u_short Pots = 8000)
				{
					addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
					addr.sin_family = AF_INET;
					addr.sin_port = htons(Pots);

					int retVal = bind(s, (SOCKADDR*)&addr, sizeof(SOCKADDR));
					if (retVal == SOCKET_ERROR) {
						//printf("Failed bind:%d\n", WSAGetLastError());
						std::cout << "\nFailed bind: " << WSAGetLastError();
						return 0;
					}


					return 1;
				}
				bool Client_Connect(PCSTR ServerAddr = "0.0.0.0", u_short Pots = 8000)
				{
					addr.sin_port = htons(Pots);

					inet_pton(AF_INET, ServerAddr, &addr.sin_addr);

					addr.sin_family = AF_INET;

					return 1;
				}

				// return true is error
				bool Send(SOCKADDR_IN TargetSocket, const char* buf)
				{
					ret = sendto(s, buf, strlen(buf)+1, 0, (SOCKADDR*) &TargetSocket,sizeof(SOCKADDR));
					return (ret != SOCKET_ERROR && ret != 0);
				}
				// return true is error
				bool Recv(SOCKADDR_IN TargetSocket,int &fromlen, char* buf, size_t size = 1024)
				{
					ret = recvfrom(s, buf, size, 0, (SOCKADDR*)&TargetSocket, &fromlen);
					return (ret != SOCKET_ERROR && ret != 0);
				}
				void GetAddrIP(SOCKADDR_IN addr_, char* ip, size_t leng = 100)
				{
					inet_ntop(AF_INET6, (LPVOID)&addr_.sin_addr, ip, leng);
				}
				void disconnect(SOCKET Target)
				{
					closesocket(Target);
				}
				bool Server_IsSocketClosed(SOCKET clientSocket) {
					bool ret = false;
					HANDLE closeEvent = WSACreateEvent();
					WSAEventSelect(clientSocket, closeEvent, FD_CLOSE);

					DWORD dwRet = WaitForSingleObject(closeEvent, 0);

					if (dwRet == WSA_WAIT_EVENT_0)
						ret = true;
					else if (dwRet == WSA_WAIT_TIMEOUT)
						ret = false;

					WSACloseEvent(closeEvent);
					return ret;
				}
				~H_IPV4()
				{
					closesocket(s);
				}
				SOCKET s;
				SOCKADDR_IN  addr;
			private:
				WSADATA  wd;
				int ret = 0;
			};
			class H_IPV6
			{
			public:
				bool Init()
				{

					if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
					{
						std::cout << "WSAStartup  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
					if (s == INVALID_SOCKET)
					{
						std::cout << "socket  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					return 1;
				}
				bool Server_CreateListen(u_short Pots = 8000)
				{
					memset(&addr, 0, sizeof(sockaddr_in6));
					addr.sin6_port = htons(Pots);
					addr.sin6_family = AF_INET6;
					addr.sin6_addr = in6addr_any;
					//inet_pton(AF_INET6, "fe80::ce6:3cc:f93a:4203%5", &addr.sin6_addr);

					int len = sizeof(sockaddr_in6);
					if (bind(s, (sockaddr*)&addr, len) == SOCKET_ERROR)
					{
						std::cout << "bind  error:" << WSAGetLastError() << std::endl;
						return 0;
					}
					return 1;
				}
				bool Client_Connect(PCSTR ServerAddr = "fe80::ce6:3cc:f93a:4203%5", u_short Pots = 8000)
				{
					memset(&addr, 0, sizeof(sockaddr_in6));
					addr.sin6_port = htons(Pots);

					inet_pton(AF_INET6, ServerAddr, &addr.sin6_addr);

					addr.sin6_family = AF_INET6;
					return 1;
				}

				// return true is error
				bool Send(sockaddr_in6 TargetSocket, const char* buf)
				{
					ret = sendto(s, buf, strlen(buf), 0,(SOCKADDR*)& TargetSocket,sizeof(TargetSocket));
					return (ret != SOCKET_ERROR && ret != 0);
				}
				// return true is error
				bool Recv(SOCKET TargetSocket,int& fromlen, char* buf, size_t size = 1024)
				{
					ret = recvfrom(s, buf, size, 0, (struct sockaddr*)&TargetSocket,&fromlen);
					return (ret != SOCKET_ERROR && ret != 0);
				}
				void GetAddrIP(sockaddr_in6 addr_, char* ip, size_t leng = 100)
				{
					inet_ntop(AF_INET6, (LPVOID)&addr_.sin6_addr, ip, leng);
				}
				void disconnect(SOCKET Target)
				{
					closesocket(Target);
				}
				bool Server_IsSocketClosed(SOCKET clientSocket) {
					bool ret = false;
					HANDLE closeEvent = WSACreateEvent();
					WSAEventSelect(clientSocket, closeEvent, FD_CLOSE);

					DWORD dwRet = WaitForSingleObject(closeEvent, 0);

					if (dwRet == WSA_WAIT_EVENT_0)
						ret = true;
					else if (dwRet == WSA_WAIT_TIMEOUT)
						ret = false;

					WSACloseEvent(closeEvent);
					return ret;
				}
				~H_IPV6()
				{
					closesocket(s);
				}
				SOCKET s;
				sockaddr_in6  addr;
			private:
				WSADATA  wd;
				int ret = 0;
			};
		}

		namespace TCP
		{
			class H_IPV6
			{
			public:
				bool Init()
				{

					if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
					{
						std::cout << "WSAStartup  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					s = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
					if (s == INVALID_SOCKET)
					{
						std::cout << "socket  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					return 1;
				}
				bool Server_CreateListen(u_short Pots = 8000)
				{
					memset(&addr, 0, sizeof(sockaddr_in6));
					addr.sin6_port = htons(Pots);
					addr.sin6_family = AF_INET6;
					addr.sin6_addr = in6addr_any;
					//inet_pton(AF_INET6, "fe80::ce6:3cc:f93a:4203%5", &addr.sin6_addr);

					int len = sizeof(sockaddr_in6);
					if (bind(s, (sockaddr*)&addr, len) == SOCKET_ERROR)
					{
						std::cout << "bind  error:" << WSAGetLastError() << std::endl;
						return 0;
					}


					if (listen(s, 5) == SOCKET_ERROR)
					{
						std::cout << "listen  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					return 1;
				}
				bool Server_Accept(sockaddr_in6& ClientAddr_Return, SOCKET& ClientSocket_Return)
				{
					memset(&ClientAddr_Return, 0, sizeof(sockaddr_in6));
					int len = sizeof(sockaddr_in6);
					ClientSocket_Return = accept(s, (sockaddr*)&ClientAddr_Return, &len);
					if (ClientSocket_Return == INVALID_SOCKET)
					{
						std::cout << "accept  error:" << WSAGetLastError() << std::endl;
						return 0;
					}
					return 1;
				}
				bool Client_Connect(PCSTR ServerAddr = "fe80::ce6:3cc:f93a:4203%5", u_short Pots = 8000)
				{
					memset(&addr, 0, sizeof(sockaddr_in6));
					addr.sin6_port = htons(Pots);

					inet_pton(AF_INET6, ServerAddr, &addr.sin6_addr);

					addr.sin6_family = AF_INET6;
					int len = sizeof(sockaddr_in6);

					if (connect(s, (sockaddr*)&addr, len) == SOCKET_ERROR)
					{
						std::cout << "connect  error:" << WSAGetLastError() << std::endl;
						return 0;
					}
					return 1;
				}

				// return true is error
				bool Send(SOCKET TargetSocket, const char* buf)
				{
					ret = send(TargetSocket, buf, strlen(buf), 0);
					return (ret != SOCKET_ERROR && ret != 0);
				}
				// return true is error
				bool Recv(SOCKET TargetSocket, char* buf, size_t size = 1024)
				{
					ret = recv(TargetSocket, buf, size, 0);
					return (ret != SOCKET_ERROR && ret != 0);
				}
				void GetAddrIP(sockaddr_in6 addr_, char* ip, size_t leng = 100)
				{
					inet_ntop(AF_INET6, (LPVOID)&addr_.sin6_addr, ip, leng);
				}
				void disconnect(SOCKET Target)
				{
					closesocket(Target);
				}
				void GetSelfIP()
				{
					char hostname[MAX_PATH];
					memset(hostname, 0, MAX_PATH);
					gethostname(hostname, MAX_PATH);
					struct hostent FAR* lpHostEnt = gethostbyname(hostname);
					if (lpHostEnt == NULL)
					{
						return;
					}

					LPSTR lpAddr = lpHostEnt->h_addr_list[0];

					std::cout << lpAddr;
				}
				bool Server_IsSocketClosed(SOCKET clientSocket) {
					bool ret = false;
					HANDLE closeEvent = WSACreateEvent();
					WSAEventSelect(clientSocket, closeEvent, FD_CLOSE);

					DWORD dwRet = WaitForSingleObject(closeEvent, 0);

					if (dwRet == WSA_WAIT_EVENT_0)
						ret = true;
					else if (dwRet == WSA_WAIT_TIMEOUT)
						ret = false;

					WSACloseEvent(closeEvent);
					return ret;
				}
				~H_IPV6()
				{
					closesocket(s);
				}
				SOCKET s;
				sockaddr_in6  addr;
			private:
				WSADATA  wd;
				std::thread* ServerNetThread = 0;
				int ret = 0;
			};
			class H_IPV4
			{
			public:
				bool Init()
				{

					if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
					{
						std::cout << "WSAStartup  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					s = socket(AF_INET, SOCK_STREAM, 0);
					if (s == INVALID_SOCKET)
					{
						std::cout << "socket  error:" << WSAGetLastError() << std::endl;
						return 0;
					}

					return 1;
				}
				bool Server_CreateListen(u_short Pots = 8000)
				{
					addr.sin_family = AF_INET;
					addr.sin_port = htons(Pots); //1024以上的端口号
					addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

					int retVal = bind(s, (LPSOCKADDR)&addr, sizeof(SOCKADDR_IN));
					if (retVal == SOCKET_ERROR) {
						//printf("Failed bind:%d\n", WSAGetLastError());
						std::cout << "\nFailed bind: " << WSAGetLastError();
						return 0;
					}

					if (listen(s, 10) == SOCKET_ERROR) {
						//printf("Listen failed:%d", WSAGetLastError());
						std::cout << "\nListen failed: " << WSAGetLastError();
						return 0;
					}

					return 1;
				}
				bool Server_Accept(sockaddr& ClientAddr_Return, SOCKET& ClientSocket_Return)
				{
					memset(&ClientAddr_Return, 0, sizeof(sockaddr));
					int len = sizeof(sockaddr);
					ClientSocket_Return = accept(s, &ClientAddr_Return, &len);
					if (ClientSocket_Return == INVALID_SOCKET)
					{
						std::cout << "accept  error:" << WSAGetLastError() << std::endl;
						return 0;
					}
					return 1;
				}
				bool Client_Connect(PCSTR ServerAddr = "0.0.0.0", u_short Pots = 8000)
				{
					addr.sin_port = htons(Pots);

					inet_pton(AF_INET, ServerAddr, &addr.sin_addr);

					addr.sin_family = AF_INET;
					int len = sizeof(sockaddr);

					if (connect(s, (sockaddr*)&addr, len) == SOCKET_ERROR)
					{
						std::cout << "connect  error:" << WSAGetLastError() << std::endl;
						return 0;
					}
					return 1;
				}

				// return true is error
				bool Send(SOCKET TargetSocket, const char* buf)
				{
					ret = send(TargetSocket, buf, strlen(buf), 0);
					return (ret != SOCKET_ERROR && ret != 0);
				}
				// return true is error
				bool Recv(SOCKET TargetSocket, char* buf, size_t size = 1024)
				{
					ret = recv(TargetSocket, buf, size, 0);
					return (ret != SOCKET_ERROR && ret != 0);
				}
				void GetAddrIP(SOCKADDR_IN addr_, char* ip, size_t leng = 100)
				{
					inet_ntop(AF_INET6, (LPVOID)&addr_.sin_addr, ip, leng);
				}
				void disconnect(SOCKET Target)
				{
					closesocket(Target);
				}
				void GetSelfIP()
				{
					char hostname[MAX_PATH];
					memset(hostname, 0, MAX_PATH);
					gethostname(hostname, MAX_PATH);
					struct hostent FAR* lpHostEnt = gethostbyname(hostname);
					if (lpHostEnt == NULL)
					{
						return;
					}

					LPSTR lpAddr = lpHostEnt->h_addr_list[0];

					std::cout << lpAddr;
				}
				bool Server_IsSocketClosed(SOCKET clientSocket) {
					bool ret = false;
					HANDLE closeEvent = WSACreateEvent();
					WSAEventSelect(clientSocket, closeEvent, FD_CLOSE);

					DWORD dwRet = WaitForSingleObject(closeEvent, 0);

					if (dwRet == WSA_WAIT_EVENT_0)
						ret = true;
					else if (dwRet == WSA_WAIT_TIMEOUT)
						ret = false;

					WSACloseEvent(closeEvent);
					return ret;
				}
				~H_IPV4()
				{
					closesocket(s);
				}
				SOCKET s;
				SOCKADDR_IN  addr;
			private:
				WSADATA  wd;
				int ret = 0;
			};
			void Cleanup()
			{
				WSACleanup();
			}
		}
	}
}




#endif