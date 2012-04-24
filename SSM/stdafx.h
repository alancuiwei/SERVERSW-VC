// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning(disable: 4251)

//#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
//#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include<algorithm>

#include "../COMM/COMM.h"
// TODO: reference additional headers your program requires here
#include "../MyODBC/MySQLAPI.h"
#include "../TQM/CRTMarketData.h"
//#pragma comment(lib,"..\\Debug\\MyODBC.lib")
//#include "..\\futuresdata\\futuresdata.h"
//#pragma comment(lib,"..\\Debug\\futuresdata.lib")
//#include "../TQM/TQM.h"

//#pragma comment(lib,"..\\Debug\\TQM.lib")
#include "SSM.h"
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
