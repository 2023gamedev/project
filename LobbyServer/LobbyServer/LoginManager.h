#pragma once

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>

class LoginManager {
public:
    LoginManager();
    ~LoginManager();

private:
    struct User {
        string username;
        string password;
    };

    sql::mysql::MySQL_Driver* driver;
    std::unique_ptr<sql::Connection> conn;

    std::unordered_map<std::string, bool> logged_in_users;
    std::mutex login_mutex;

    vector<User> users; // 사용자 데이터 저장

public:
    bool RegisterName(const string& username, const string& password);
    bool Login(const string& username, const string& password);
    bool Logout(const string& username);
    void SaveToUser(const string& filename, const string& username, const string& password);
    void LoadFromFile(const string& filename);

    void LoadFromDB();

    // 기타 사용자 관리 기능 구현
};
