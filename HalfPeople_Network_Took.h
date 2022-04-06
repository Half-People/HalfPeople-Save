#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <Windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define All -1

namespace HalfPeopleTook
{
	namespace UDP
	{

	}

	namespace TCP
	{
		namespace Server
		{
			namespace SaveData
			{
				SOCKET sockSrv;
				SOCKET sockConn[1];
				SOCKADDR_IN addrClient[1];
				HANDLE hThread[1];
				int ClientID;
				void(*Error)(int ID,const char *ErrorMessage);
				void (*Ms)(int ClientIndex);

				void TimeoutDisconnected(int ID, int WaitingTime)
				{
					Sleep(WaitingTime);
					closesocket(SaveData::sockConn[ID]);
					//closesocket(SaveData::sockSrv);
					ClientID--;
					CloseHandle(SaveData::hThread[ID]);
					Error(ID, " Connection timed out");
					return;
				}
			}
			int port;

			std::thread WaitDisconnect[1];

			DWORD WINAPI SandAndGetMs(LPVOID lpParam)
			{
				SaveData::Ms((int)lpParam);
				return 0;
			}

			void initialization(int Port = 5099)
			{
				port = Port;
				WSADATA wsaData;
				//SaveData::ClientID = -1;

				if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				{
					//printf("Failed to load Winsock");
					SaveData::Error(-1,"Failed to load Winsock");
					return;
				}

				//创建用于监听的套接字
				SaveData::sockSrv = socket(AF_INET, SOCK_STREAM, 0);

				SOCKADDR_IN addrSrv;
				addrSrv.sin_family = AF_INET;
				addrSrv.sin_port = htons(port); //1024以上的端口号
				addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

				int retVal = bind(SaveData::sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
				if (retVal == SOCKET_ERROR) {
					//printf("Failed bind:%d\n", WSAGetLastError());
					SaveData::Error(-1,"Failed bind:%d" + WSAGetLastError());
					return;
				}

				if (listen(SaveData::sockSrv, 10) == SOCKET_ERROR) {
					//printf("Listen failed:%d", WSAGetLastError());
					SaveData::Error(-1,"Listen failed:%d" + WSAGetLastError());
					return;
				}
			}

			void WaitingClientConnect(void (*MessageJudgment)(int ClientIndex), void(*ErrorReceiving)(int ClientIndex,const char* ErrorMessage), void(*ClientEnters)(int ClientIndex))
			{
				SaveData::Error = ErrorReceiving;
				SaveData::Ms = MessageJudgment;
				while (true)
				{

					int len = sizeof(SOCKADDR);
					//等待客户请求到来    
					SaveData::sockConn[SaveData::ClientID] = accept(SaveData::sockSrv, (SOCKADDR*)&SaveData::addrClient, &len);
					if (SaveData::sockConn[SaveData::ClientID] == SOCKET_ERROR) {
						//printf("\nAccept failed:%d", WSAGetLastError());
						SaveData::Error(SaveData::ClientID,"\nAccept failed:%d" + WSAGetLastError());
					}
					ClientEnters(SaveData::ClientID);
					//printf("\nAccept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

					SaveData::hThread[SaveData::ClientID] = CreateThread(NULL, 0, SandAndGetMs, (LPVOID)SaveData::ClientID, 0, 0);
					SaveData::ClientID++;
				}

			}

			bool Send(int ClientIndex, const char * SendText)
			{
				int iSend = send(SaveData::sockConn[ClientIndex], SendText, strlen(SendText) + 1, 0);
				if (iSend == SOCKET_ERROR) {
					//printf("send failed");
					SaveData::Error(ClientIndex, "Failed to send message");
					return false;
				}
				return true;
			}

			void Receive(int ClientIndex, char * ReceiveText ,int Size)
			{
				recv(SaveData::sockConn[ClientIndex], ReceiveText, Size, 0);
				return;
			}

			void Disconnect(int ClientIndex)
			{
				if (ClientIndex == -1)
				{
					for (size_t i = 0; i < SaveData::ClientID; i++)
					{
						closesocket(SaveData::sockConn[i]);
						CloseHandle(SaveData::hThread[i]);
					}
					closesocket(SaveData::sockSrv);
					SaveData::ClientID = 0;
					WSACleanup();
				}
				else
				{
					closesocket(SaveData::sockConn[ClientIndex]);
					CloseHandle(SaveData::hThread[ClientIndex]);
					SaveData::ClientID--;
				}
			}

			void ClientTimedOutToDisconnect(int ClientIndex, int WaitingTime)
			{
				WaitDisconnect[ClientIndex] = std::thread(SaveData::TimeoutDisconnected, ClientIndex, WaitingTime);
			}//function under test

			void CancelClientTimedOutToDisconnect(int ClientIndex)
			{
				WaitDisconnect[ClientIndex].detach();
			}//function under test

			char* GetIP_Addr(int ClientIndex)
			{
				return inet_ntoa(SaveData::addrClient[ClientIndex].sin_addr);
			}
		}
		namespace Client
		{
			namespace SaveData
			{
				SOCKADDR_IN addrSrv;
				SOCKET sockClient;
				void (*ER)(const char* ErrorMessage);
			}
			void initialization(int Port, const char* IP_Addr,void (*ErrorReceiving)(const char* ErrorMessage))
			{
#if Port==0
				Port = 5099;
#endif
				SaveData::ER = ErrorReceiving;
				WSADATA wsaData;
				char buff[1024];
				//memset(buff, 0, sizeof(buff));

				if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				{
					SaveData::ER("\nFailed to load Winsock");
					return ;
				}

				SaveData::addrSrv.sin_family = AF_INET;
				SaveData::addrSrv.sin_port = htons(Port);
				SaveData::addrSrv.sin_addr.S_un.S_addr = inet_addr(IP_Addr);

				SaveData::sockClient = socket(AF_INET, SOCK_STREAM, 0);
				if (SOCKET_ERROR == SaveData::sockClient) {
					//printf("\nSocket() error:%d", WSAGetLastError());
					SaveData::ER("\nSocket() error:%d" + WSAGetLastError());
					return ;
				}
				if (connect(SaveData::sockClient, (struct  sockaddr*)&SaveData::addrSrv, sizeof(SaveData::addrSrv)) == INVALID_SOCKET) {
					SaveData::ER("\nConnect failed:%d" + WSAGetLastError());
					return ;
				}
				return;
			}

			void Send(const char* SendText)
			{
				send(SaveData::sockClient, SendText, strlen(SendText) + 1, 0);
			}
			void Receive(char* ReceiveText, int Size)
			{
				recv(SaveData::sockClient, ReceiveText, Size, 0);
			}
			void Disconnect(bool CloseAll)
			{
				closesocket(SaveData::sockClient);
				if (CloseAll)
					WSACleanup();
			}
		}
	}
}
