#include "Common.h"

LoginManager::LoginManager()
{
    LoadFromFile("users.txt");
}

LoginManager::~LoginManager()
{
}

// ȸ������
bool LoginManager::RegisterName(const string& username, const string& password)
{
    LoadFromFile("users.txt");
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "Username already exists." << endl;
            return false;
        }
    }
    // �� ����� �߰�
    SaveToUser("users.txt", username, password);
    return true;
}

// �α���
bool LoginManager::Login(const string& username, const string& password)
{
    LoadFromFile("users.txt");
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            return true;
        }
    }

    cout << "��ϵ��� ���� ���̵�ų� ��й�ȣ�� Ʋ�Ƚ��ϴ�." << endl;
    return false;
}

// ���� ���� ����
void LoginManager::SaveToUser(const string& filename, const string& username, const string& password)
{
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << username << " " << password << endl;
        file.close();
    }
}

// ���� �ҷ�����
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