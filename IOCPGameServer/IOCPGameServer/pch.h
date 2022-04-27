#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <unordered_map>
#include <map>
#include <array>
#include "protocol.h"
#include "define.h"
#include "global.h"
#include "func.h"

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")
using namespace std;

