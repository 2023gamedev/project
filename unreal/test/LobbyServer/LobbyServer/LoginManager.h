#pragma once

class LoginManager {
public:
    LoginManager();
    ~LoginManager();

private:
    struct User {
        string username;
        string password;
        int wincount;
    };

    vector<User> users; // ����� ������ ����

public:
    bool RegisterName(const string& username, const string& password);
    bool Login(const string& username, const string& password);
    void SaveToUser(const string& filename, const string& username, const string& password);
    void LoadFromFile(const string& filename);

    // ��Ÿ ����� ���� ��� ����
};
