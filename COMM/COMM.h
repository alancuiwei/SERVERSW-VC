// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef COMM_EXPORTS
#define COMM_API __declspec(dllexport)
#else
#define COMM_API __declspec(dllimport)
#endif

#include <process.h>
#include <WINSOCK2.H>
#include <vector>
#include <map>
#include <iostream>
#include "CRTMarketData.h"
using namespace std;


// This class is exported from the COMM.dll
class COMM_API CCOMM {
public:
	CCOMM(void){};
	~CCOMM(void);
	CCOMM(char *pserveraddr, int serverportid);
	int commtype; //连接的种类 1代表server，2代表client
	static const int msgsize = 1024;
	char sMessage[msgsize];
	SOCKADDR_IN serveraddr;
	SOCKET   localsocket; //监听套接字
    HANDLE hThreadSend;
	HANDLE hThreadReceive;
	HANDLE hThreadAccept;
	static void AgentAccept(void *p); //线程代理
    static void AgentReceive(void *p); //线程代理
	static void AgentSend(void *p); //线程代理
	virtual void RunAccept(void) = 0;
	virtual void RunReceive(void) = 0;
	virtual void RunSend(void) = 0;
	virtual void Start(void) = 0; //启动
};

class COMM_API CCOMMServer :
	public CCOMM
{
public:
	CCOMMServer(char *pserveraddr, int serverportid);
	CCOMMServer(void);
	~CCOMMServer(void);
	virtual void RunAccept(void);
	virtual void RunReceive(void);
	virtual void RunSend(void){};
	virtual void Start(void); //启动
	void SendToAllClient(char * buf, int len);
    HANDLE hThreadAccept;
	static const int clientmaxnum = 10; //所支持的客户端的最大个数
	int clientnum;
	SOCKADDR_IN clientaddr[clientmaxnum];
	SOCKET   clientsocket[clientmaxnum];
	// TODO: add your methods here.
    static CCOMMServer * getinstance();
	vector<SOCKET> vclientsocket;
    vector<SOCKADDR_IN> vclientaddr;

private:
	static CCOMMServer * instance;
};

class COMM_API CCOMMClient :
	public CCOMM
{
public:
	CCOMMClient(void){};
	CCOMMClient(char *pserveraddr, int serverportid);
	bool isnewsocket;
	virtual void RunAccept(void){};
	virtual void RunReceive(void);
	virtual void RunSend(void){};
	virtual void Start(void); //启动
};

extern COMM_API std::map<std::string, CRTMarketData*> comm_prtmarketdatamap;