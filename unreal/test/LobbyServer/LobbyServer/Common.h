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


// Ŭ���̾�Ʈ�� ������ ����ϱ� ���� �⺻���� ��� ��Ģ
#include "protocol.h"
#include "LStruct.pb.h"

// iocp ������ ������ �Ǵ� ���� �ٽ����� Ŭ������ ������ ��� �ִ�.
#include "iocpServerClass.h"

#include "LoginManager.h"