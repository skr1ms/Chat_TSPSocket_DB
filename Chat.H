#include "User.h"
class Chat
{
private:
	vector<User> users;
	map<string, User *> userIdent;
	map<string, string> privateMessages;
	vector<string> publicMessages;

public:
	void registerUser(string login, string password);

	auto loginUser(string login, string password) -> User *;

	void lsMessage(User *sender, string recipientLogin);

	void broadcastMessage(User *sender, string message);

	void logout(User *&currentUser);

	void showListOfUsers();

	void showLsMessage(string log);

	void showMessageForAll();
	void tcpCommunication();

	void end();
	~Chat();
	Chat();
};
