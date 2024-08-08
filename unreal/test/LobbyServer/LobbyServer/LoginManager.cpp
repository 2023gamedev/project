#include "Common.h"

LoginManager::LoginManager()
{
    LoadFromFile("users.txt");
}

LoginManager::~LoginManager()
{
}

// 회원가입
bool LoginManager::RegisterName(const string& username, const string& password)
{
    LoadFromFile("users.txt");
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "Username already exists." << endl;
            return false;
        }
    }
    // 새 사용자 추가
    SaveToUser("users.txt", username, password);
    return true;
}

// 로그인
bool LoginManager::Login(const string& username, const string& password)
{
    LoadFromFile("users.txt");
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            return true;
        }
    }

    cout << "등록되지 않은 아이디거나 비밀번호가 틀렸습니다." << endl;
    return false;
}

// 유저 정보 저장
void LoginManager::SaveToUser(const string& filename, const string& username, const string& password)
{
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << username << " " << password << endl;
        file.close();
    }
}

// 파일 불러오기
void LoginManager::LoadFromFile(const string& filename)
{
    ifstream file(filename);
    if (file.is_open()) {
        string username, password;
        while (file >> username >> password) {
            users.push_back({ username, password });
        }
        file.close();
    }
}