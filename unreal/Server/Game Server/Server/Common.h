#pragma once
#pragma comment(lib, "ws2_32")

#include<WinSock2.h>
#include<Windows.h>
#include "pch.h"

#include<iostream>
#include<thread>
#include<vector>
#include<cstdlib>
using namespace std;


// Ŭ���̾�Ʈ�� ������ ����ϱ� ���� �⺻���� ��� ��Ģ
#include "protocol.h"
#include "GStruct.pb.h"

// iocp ������ ������ �Ǵ� ���� �ٽ����� Ŭ������ ������ ��� �ִ�.
#include"iocpServerClass.h"
#include "Session.h"