#pragma once

#include"Common.h"
#include"iocpServerClass.h"

using namespace std;


int main() {


	_wsetlocale(LC_ALL, L"korean");

	// IOCP ���� class
	IOCP_CORE* iocpServer = &IOCP_CORE::GetInstance();
}