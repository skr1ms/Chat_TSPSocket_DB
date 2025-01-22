#include "logger.h"
#include <sstream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "mysql.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;
SOCKET clientSocket;
string response_to_client;
void showPersonalMessages(string &mes, SOCKET clientSocket)
{
    int user_id = 0;
    string user_login = mes.substr(2);
    string temp;
    string from_query = "SELECT id FROM users_list WHERE login = '" + user_login + "'";
    mysql_query(&mysql, from_query.c_str());
    res = mysql_store_result(&mysql);
    if (mysql_num_rows(res) > 0)
    {
        row = mysql_fetch_row(res);
        temp = row[0];
        user_id = stoi(temp);
    }
    else
    {
        response_to_client = "unknow error";
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        return;
    }
    mysql_free_result(res);
    string main_query = "SELECT tb1.login, tb2.message, tb2.sent_date FROM users_list tb1 join messages_storage tb2 on tb1.id=tb2.from_id AND tb2.to_id='" + to_string(user_id) + "'";
    if (mysql_query(&mysql, main_query.c_str()))
    {
        response_to_client = mysql_error(&mysql);
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        return;
    }
    ostringstream oss;
    if (res = mysql_store_result(&mysql))
    {
        if (mysql_num_rows(res) == 0)
        {
            string response_to_client = "You don't have messages";
            send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            for (int i = 0; i < mysql_num_fields(res); ++i)
            {
                oss << row[i] << "\t";
            }
            oss << "\n";
        }
        string response_to_client = oss.str();
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    }
}
void showListOfUsers(SOCKET clientSocket)
{
    response_to_client = "";
    string tmp;
    mysql_query(&mysql, "SELECT login FROM users_list");
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (int i = 0; i < mysql_num_fields(res); i++)
            {
                if (row[i] != "")
                {
                    tmp = row[i];
                    response_to_client += tmp + "\n";
                }
            }
        }
    }
    send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    mysql_free_result(res);
}
void printPublicChatMes(SOCKET clientSocket)
{
    response_to_client = "";
    string tmp;
    mysql_query(&mysql, "SELECT * FROM all_chat");
    if (res = mysql_store_result(&mysql))
    {
        if (mysql_num_rows(res) == 0)
        {
            string response_to_client = "There is no messages yet";
            send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
            return;
        }
        while (row = mysql_fetch_row(res))
        {
            for (int i = 0; i < mysql_num_fields(res); i++)
            {
                if (row[i] != "")
                {
                    tmp = row[i];
                    response_to_client += tmp + "\n";
                }
            }
        }
    }
    send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    mysql_free_result(res);
}
void sendPublicMessage(string &mes, SOCKET clientSocket)
{
    int pos, end_pos;
    pos = mes.find("**");
    end_pos = mes.find("*L");
    string input = "from: " + mes.substr(pos + 2, end_pos - 2) + ", message: " + mes.substr(end_pos + 2);
    string query = "INSERT INTO all_chat(message) values('" + input + "')";
    if (mysql_query(&mysql, query.c_str()))
    {
        response_to_client = mysql_error(&mysql);
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        return;
    }
    response_to_client = "message successfully delivered";
    send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    Logger logger("log.txt", input);
}
void personalMess(string &mes, SOCKET clientSocket)
{
    std::time_t now = std::time(nullptr);
    std::tm *local_time = std::localtime(&now);
    char date_buffer[80];
    std::strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d %H:%M:%S", local_time);
    int pos, end_pos, mes_pos;
    pos = mes.find("*P");
    mes_pos = mes.find("*S");
    end_pos = mes.find("*M");
    string from_query = "SELECT id FROM users_list WHERE login = '" + mes.substr(mes_pos + 2) + "'";
    string to_query = "SELECT id FROM users_list WHERE login = '" + mes.substr(pos + 2, end_pos - 2) + "'";
    string temp;
    int from_id = 0;
    int to_id = 0;
    mysql_query(&mysql, from_query.c_str());
    res = mysql_store_result(&mysql);
    if (mysql_num_rows(res) > 0)
    {
        row = mysql_fetch_row(res);
        temp = row[0];
        from_id = stoi(temp);
        cout << from_id << endl;
    }
    mysql_free_result(res);
    mysql_query(&mysql, to_query.c_str());
    res = mysql_store_result(&mysql);
    if (mysql_num_rows(res) > 0)
    {
        row = mysql_fetch_row(res);
        temp = row[0];
        to_id = stoi(temp);
        cout << to_id << endl;
    }
    else
    {
        response_to_client = "this user doesn't exist";
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        return;
    }
    mysql_free_result(res);
    string query = "INSERT INTO messages_storage(id, from_id, to_id, message, sent_date) values(default, '" + to_string(from_id) + "','" + to_string(to_id) + "','" + mes.substr(end_pos + 2, mes.length() - end_pos - mes.substr(mes_pos).length() - 2) + "','" + string(date_buffer) + "')";
    if (mysql_query(&mysql, query.c_str()))
    {
        response_to_client = mysql_error(&mysql);
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        return;
    }
    response_to_client = "message successfully delivered";
    string log_str = to_string(from_id) + "','" + to_string(to_id) + "','" + mes.substr(end_pos + 2, mes.length() - end_pos - mes.substr(mes_pos).length() - 2) + "','" + string(date_buffer);
    send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    Logger logger("log.txt", log_str);
}
void regUser(string &mes, SOCKET clientSocket)
{
    int pos, end_pos;
    pos = mes.find("*L");
    end_pos = mes.find("*P");
    std::string input_str = "INSERT INTO users_list(id, login, password) values(default, '" + mes.substr(pos + 2, end_pos - 2) +
                            "', '" + mes.substr(end_pos + 2) + "')";
    if (mysql_query(&mysql, input_str.c_str()))
    {
        if (mysql_errno(&mysql) == 1062)
        {
            response_to_client = "this user already exist";
            send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
        }
    }
    else
    {
        std::cout << mes.substr(pos + 2, end_pos - 2) << endl;
        ;
        std::cout << mes.substr(end_pos + 2) << endl;
        response_to_client = "Succsessfully registered";
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    }
}
void loginUser(string &mes, SOCKET clientSocket)
{
    int pos, end_pos;
    pos = mes.find("L*");
    end_pos = mes.find("P*");
    mysql_query(&mysql, "SELECT login FROM users_list");
    int flag = 0;

    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (int i = 0; i < mysql_num_fields(res); i++)
            {
                if (row[i] == mes.substr(pos + 2, end_pos - 2))
                {
                    flag++;
                }
            }
        }
    }
    mysql_free_result(res);
    mysql_query(&mysql, "SELECT password FROM users_list");
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (int i = 0; i < mysql_num_fields(res); i++)
            {
                if (row[i] == mes.substr(end_pos + 2))
                {
                    flag++;
                }
            }
        }
    }
    mysql_free_result(res);
    if (flag == 2)
    {
        response_to_client = "Succ";
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    }
    else
    {
        response_to_client = "wrong login or password";
        send(clientSocket, response_to_client.c_str(), response_to_client.size(), 0);
    }
    flag = 0;
}
void handleClient(SOCKET clientSocket)
{
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        mysql_init(&mysql);
        if (&mysql == NULL)
        {
            cout << "Error: can't create MySQL-descriptor" << endl;
        }

        if (!mysql_real_connect(&mysql, "localhost", "root", "Got_milk3", "chatdb", 0, NULL, 0))
        {
            cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        }
        else
        {

            string mess = buffer;
            if (mess.substr(0, 2) == "*L")
            {
                regUser(mess, clientSocket);
            }
            if (mess.substr(0, 2) == "L*")
            {
                loginUser(mess, clientSocket);
            }
            if (mess.substr(0, 2) == "*P")
            {
                personalMess(mess, clientSocket);
            }
            if (mess.substr(0, 2) == "**")
            {
                sendPublicMessage(mess, clientSocket);
            }
            if (mess.substr(0, 2) == "!*")
            {
                printPublicChatMes(clientSocket);
            }
            if (mess.substr(0, 2) == "*U")
            {
                showListOfUsers(clientSocket);
            }
            if (mess.substr(0, 2) == "*8")
            {
                showPersonalMessages(mess, clientSocket);
            }
            if (mess == "*!!!*")
            {
                exit(1);
            }
        }
    }
}
int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    cout << "server is waiting\n";
    clientSocket = accept(listenSocket, NULL, NULL);
    while (clientSocket)
    {
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Accept failed." << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        handleClient(clientSocket);
    }

    return 0;
}