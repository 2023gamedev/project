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


// Ŭ���̾�Ʈ�� ������ ����ϱ� ���� �⺻���� ��� ��Ģ ( ���� �� Ŭ�󿡼� ���� ��, Ŭ���̾�Ʈ�� ����ȭ�Ǿ� ������ ��ģ�� )
#include"protocol.h"

// iocp ������ ������ �Ǵ� ���� �ٽ����� Ŭ������ ������ ��� �ִ�.
#include"iocpServerClass.h"