#pragma once
#ifdef STG010001_EXPORTS
#define STG010001_API __declspec(dllexport)
#else
#define STG010001_API __declspec(dllimport)
#endif
#include "CTradeStru.h"

class STG010001_API CStrategyRun
{
public:
    CStrategyRun(void);
	~CStrategyRun(void);
	CStrategyRun(std::string strategyid);
    //virtual void initialization(void)=0;
    virtual void Run(void)=0;

    /* ������(��UI�ṩ) */
    std::string strategyid;
    /* ����ò��������Ե��µ�����㣬������Ȳ����� */
    std::vector<CTradeStru*> runtradeparameters;
};

