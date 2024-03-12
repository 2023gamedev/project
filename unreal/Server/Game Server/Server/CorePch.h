#pragma once

#include <windows.h>
#include <iostream>
using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <vector>
#include <cstdlib>

#include "Types.h"
#include "protocol.h"
#include "GStruct.pb.h"

#include"iocpServerClass.h"
#include "Session.h"