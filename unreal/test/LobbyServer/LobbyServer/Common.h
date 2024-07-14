#pragma once
#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <Windows.h>
#include "pch.h"

#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <mutex>
#include <fstream>
using namespace std;


// 클라이언트와 서버가 통신하기 위한 기본적인 통신 규칙
#include "protocol.h"
#include "LStruct.pb.h"

// iocp 서버의 몸통이 되는 가장 핵심적인 클래스의 선언이 들어 있다.
#include "iocpServerClass.h"

#include "LoginManager.h"