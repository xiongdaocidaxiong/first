#ifndef __COMDEFINE__
#define  __COMDEFINE__

#pragma  once
#include <condition_variable>
#include <unordered_map>
#include <functional>
#include <thread>
#include <vector>
#include <string>
#include <queue>
#include <mutex>
#include <list>
#include <windows.h>
#include <iostream>

#define  SAFEDELETE(point) if(point){delete point;point = nullptr;}

extern int g_SerStart;
#endif