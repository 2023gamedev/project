#pragma once
#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <Windows.h>
#include "pch.h"

#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <mutex>
#include <random>
using namespace std;


// 클라이언트와 서버가 통신하기 위한 기본적인 통신 규칙
#include "protocol.h"
#include "GStruct.pb.h"