#pragma once
#include"Common.h"
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <iostream>

using namespace std;


// db 연동 테스트용
//int main() {
//    try {
//        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
//        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "root", "비밀번호"));
//        con->setSchema("logindata");
//
//        std::unique_ptr<sql::Statement> stmt(con->createStatement());
//        stmt->execute("CREATE TABLE IF NOT EXISTS users (id INT PRIMARY KEY AUTO_INCREMENT, username VARCHAR(50), password VARCHAR(255), email VARCHAR(100))");
//
//        std::cout << "Connected to MySQL successfully!" << std::endl;
//    }
//    catch (sql::SQLException& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//    return 0;
//}

int main() {
	_wsetlocale(LC_ALL, L"korean");

	// IOCP 서버 class
	IOCP_CORE iocpServer;
}