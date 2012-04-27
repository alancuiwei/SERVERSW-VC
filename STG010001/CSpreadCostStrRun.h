#pragma once

#include "CStrategyRun.h"
class  CSpreadCostStrRun :
	public CStrategyRun
{
public:
    /* 保存该用户可以显示的套利合约对class */
	CSpreadCostStrRun(std::string strategyid);
	CSpreadCostStrRun(void);
	~CSpreadCostStrRun(void);
    static void* Agent(void *p);
    virtual void Run(void);
	void AddPair(CSpreadCostPair * pair);
    void Start();
	void SavePairsXML( char *);
	char xmlpath[200];
    HANDLE GetThread();
    HANDLE hThread;
	vector<CSpreadCostPair*> runpairs;
};

