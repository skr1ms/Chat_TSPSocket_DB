#include "Chat.H"
#if defined(__linux__)
#define MESSAGE_LENGTH 500
#define PORT 8000
int sockt_file_descriptor, connection;
struct sockaddr_in serveraddress, client;
#endif
char message[MESSAGE_LENGTH];
void Chat::registerUser(string login, string password)
{
	int pos1 = login.find("*");
	int pos2 = password.find("*");

	if (login.length() > 60 || password.length() > 60 || pos1 != -1 || pos2 != -1)
	{
		throw 2;
	}
}
auto Chat::loginUser(string login, string password) -> User *
{
	if (userIdent.find(login) != userIdent.end() && userIdent[login]->getPassword() == password)
	{
		return userIdent[login];
	}
	return nullptr;
}
Chat::~Chat() {}
Chat::Chat() {}
void Chat::lsMessage(User *sender, string recipientLogin)
{
	if (userIdent.find(recipientLogin) != userIdent.end())
	{
		string message;
		cout << "Enter message: ";
		cin >> message;
		privateMessages[userIdent[recipientLogin]->getLogin()] = sender->getLogin() + " sends private message to " + userIdent[recipientLogin]->getLogin() + ": " + message;
		cout << sender->getLogin() << " sends private message to " << userIdent[recipientLogin]->getLogin() << ": " << message << endl;
	}
	else
	{
		cout << "User not found!" << std::endl;
	}
}

void Chat::broadcastMessage(User *sender, string message)
{
	for (auto &user : users)
	{
		if (user.getLogin() == sender->getLogin())
		{
			publicMessages.push_back(message);
			cout << sender->getLogin() << " sends message to all: " << message << endl;
		}
	}
}

void Chat::logout(User *&currentUser)
{
	if (currentUser != nullptr)
	{
		cout << "you have logged out of your account\n";
		currentUser = nullptr;
	}
	else
	{
		cout << "you are not logged in\n";
	}
}

void Chat::showListOfUsers()
{
	for (auto &user : users)
	{
		cout << "Login: " << user.getLogin() << endl;
	}
}

void Chat::showLsMessage(string log)
{
	if (privateMessages.empty() || privateMessages.find(log) == privateMessages.end())
	{
		cout << "No messages yet." << endl;
	}
	else
	{
		cout << "Personal messages:" << endl;
		for (auto &message : privateMessages)
		{
			cout << message.second << endl;
		}
	}
}

void Chat::showMessageForAll()
{
	if (publicMessages.empty())
	{
		cout << "No messages yet." << endl;
	}
	else
	{
		cout << "Public messages:" << endl;
		for (const auto &message : publicMessages)
		{
			cout << message << endl;
		}
	}
}
void Chat::end()
{
	for (auto &user : userIdent)
	{
		delete user.second;
	}
}
#if defined(__linux__)
void Chat::tcpCommunication()
{
	sockt_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (sockt_file_descriptor == -1)
	{
		cout << "Creation of Socket failed!" << endl;
		exit(1);
	}

	serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	serveraddress.sin_port = htons(PORT);

	serveraddress.sin_family = AF_INET;

	connection = connect(sockt_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
	if (connection == -1)
	{
		cout << "Connection with the server failed.!" << endl;
		exit(1);
	}

	while (1)
	{
		bzero(message, sizeof(message));
		cout << "Enter the message you want to send to the server: " << endl;
		cin >> message;
		if ((strncmp(message, "end", 3)) == 0)
		{
			write(sockt_file_descriptor, message, sizeof(message));
			cout << "Client Exit." << endl;
			break;
		}
		ssize_t bytes = write(sockt_file_descriptor, message, sizeof(message));

		if (bytes >= 0)
		{
			cout << "Data send to the server successfully.!" << endl;
		}
		bzero(message, sizeof(message));

		read(sockt_file_descriptor, message, sizeof(message));
		cout << "Data received from server: " << message << endl;
	}

	close(sockt_file_descriptor);
}
#endif
