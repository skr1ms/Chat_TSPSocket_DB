#include "Chat.H"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
int main()
{
	Chat chat;
	locale::global(locale(""));
	User *currentUser = nullptr;
	string curr_user_login = "";
	int option = 1;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed." << std::endl;
		return 1;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed." << std::endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

	if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Connection failed." << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	while (option)
	{
		cout << "WARNING!!! YOU CAN'T USE MORE THAN 60 SYMBOLS AND (*) SYMBOL IN LOGIN OR PASSWORD!!!!\n ALSO YOU CAN'T USE * SUMBOLS IN YOUR MESSAGES\n";
		cout << "1. Register" << endl;
		cout << "2. Login" << endl;
		cout << "3. Send message to user" << endl;
		cout << "4. Send message for all" << endl;
		cout << "5. Get out of the account" << endl;
		cout << "6. Show list of users" << endl;
		cout << "7. Show private messages" << endl;
		cout << "8. Show messages for all" << endl;
		cout << "9. Exit" << endl;
		cout << "10. TCP chat function(if u are on linux)" << endl;
		cout << "Choose 1 of the options " << endl;
		int choice;
		string server_str;
		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			string login, password;
			cout << "Enter login: ";
			cin >> login;
			cout << "Enter password: ";
			cin >> password;
			std::cout << "";
			server_str = "*L" + login + "*P" + password;
			try
			{
				chat.registerUser(login, password);
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				cout << buffer << endl;
			}
			catch (const int &exception)
			{
				cout << "length of ur login or password or name is longer than 60 symbols or you are using * letter!\n";
				break;
			}
			cout << "Registration successful!" << endl;
			break;
		}

		case 2:
		{
			if (curr_user_login == "")
			{
				string login, password;
				cout << "Enter login: ";
				cin >> login;
				cout << "Enter password: ";
				cin >> password;
				server_str = "L*" + login + "P*" + password;
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				string tmp = buffer;
				if (tmp == "Succ")
				{
					curr_user_login = login;
				}
				if (curr_user_login == login)
				{
					cout << "Login successful!" << endl;
				}
				else
				{
					cout << "Invalid login or password!" << endl;
				}
				break;
			}
			else
			{
				cout << "You need to logout\n";
				break;
			}
		}

		case 3:
		{

			if (curr_user_login != "")
			{
				string recipientLogin, recipientMessage;
				cout << "Enter recipient login: ";
				cin >> recipientLogin;
				cout << "Enter a message: ";
				cin >> recipientMessage;
				int check = recipientMessage.find("*");
				if (check != -1)
				{
					cout << "* symbol has been detected";
					break;
				}
				server_str = "*P" + recipientLogin + "*M" + recipientMessage + "*S" + curr_user_login;
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				cout << buffer << endl;
				break;
			}
			cout << "You aren't logged in\n";
			break;
		}

		case 4:
		{
			if (curr_user_login != "")
			{
				string message;
				cout << "Enter message: ";
				cin >> message;
				int check = message.find("*");
				if (check != -1)
				{
					cout << "* symbol has been detected";
					break;
				}
				server_str = "**" + curr_user_login + "*L" + message;
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				cout << buffer << endl;
				break;
			}
			cout << "You aren't logged in\n";
			break;
		}

		case 5:
		{
			if (curr_user_login != "")
			{
				curr_user_login = "";
				break;
			}
			cout << "You aren't logged in\n";
			break;
		}

		case 6:
		{
			if (curr_user_login != "")
			{
				server_str = "*U";
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				cout << buffer << endl;
				break;
			}
			cout << "You aren't logged in\n";
			break;
		}

		case 7:
		{
			if (curr_user_login != "")
			{
				server_str = "*8" + curr_user_login;
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				cout << buffer << endl;
				break;
			}
			cout << "You aren't logged in\n";
			break;
		}

		case 8:
		{
			if (curr_user_login != "")
			{
				server_str = "!*";
				send(clientSocket, server_str.c_str(), server_str.size(), 0);
				int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				buffer[bytesReceived] = '\0';
				cout << buffer << endl;
				break;
			}
			cout << "You aren't logged in\n";
			break;
		}

		case 9:
		{
			option = 0;
			cout << "Goodbye!";
			string end = "*!!!*";
			send(clientSocket, end.c_str(), end.size(), 0);
			closesocket(clientSocket);
			WSACleanup();
			chat.end();
			break;
		}
		case 10:
		{
#if defined(_WIN32) || (_WIN64)
			cout << "u are on windows!\n";
			break;

#elif defined(__linux__)
			if (currentUser != nullptr)
			{
				cout << "You need write 'end' to finish the server connection\n";
				chat.tcpCommunication();
				break;
			}
			else
			{
				cout << "You aren't logged in\n";
				break;
			}
#endif
		}

		default:
		{
			cout << "Invalid choice!" << endl;
			break;
		}
		}
	}
	return 0;
}
