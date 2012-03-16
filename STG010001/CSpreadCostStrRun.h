#pragma once
#ifdef STG010001_EXPORTS
#define STG010001_API __declspec(dllexport)
#else
#define STG010001_API __declspec(dllimport)
#endif
#include "CStrategyRun.h"
class STG010001_API CSpreadCostStrRun :
	public CStrategyRun
{
public:
    /* ������û�������ʾ��������Լ��class */
	CSpreadCostStrRun(std::string strategyid);
	CSpreadCostStrRun(void);
	~CSpreadCostStrRun(void);
    static void Agent(void *p);
    virtual void Run(void);
	void AddPair(CSpreadCostPair * pair);
    void Start();
	void SaveXML();
	char xmlpath[200];
    HANDLE GetThread();
    HANDLE hThread;
	vector<CSpreadCostPair*> runpairs;
};

