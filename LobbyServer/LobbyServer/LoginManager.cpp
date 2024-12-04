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
    std::lock_guard<std::mutex> lock(login_mutex);
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "Username already exists." << endl;
            return false;
        }
    }
    cout << username << " " << password << endl;
    // 새 사용자 추가
    SaveToUser("users.txt", username, password);
    return true;
}

// 로그인
bool LoginManager::Login(const string& username, const string& password)
{
    if (logged_in_users[username]) {
        cout << "이미 로그인된 사용자입니다." << endl;
        return false;
    }

    std::lock_guard<std::mutex> lock(login_mutex);
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            logged_in_users[username] = true;
            return true;
        }
    }
    cout << username << " " << password << endl;
    cout << "등록되지 않은 아이디거나 비밀번호가 틀렸습니다." << endl;
    return false;
}

bool LoginManager::Logout(const string& username)
{
    if (logged_in_users.find(username) != logged_in_users.end() && logged_in_users[username]) {
        logged_in_users[username] = false;
        cout << username << "님이 로그아웃했습니다." << endl;
        return true;
    }
    cout << "로그인 상태가 아닙니다." << endl;
    return false;
}

// 유저 정보 저장
void LoginManager::SaveToUser(const string& filename, const string& username, const string& password)
{
    users.push_back({ username, password });
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << username << " " << password << endl;
        file.close();
    }
}

// 파일 불러오기
void LoginManager::LoadFromFile(const string& filename)
{
    users.clear();
    ifstream file(filename);
    if (file.is_open()) {
        string username, password;
        while (file >> username >> password) {
            users.push_back({ username, password });
        }
        file.close();
    }
}