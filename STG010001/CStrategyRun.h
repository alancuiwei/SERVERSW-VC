#pragma once
#include "CTradeStru.h"

class  CStrategyRun
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

