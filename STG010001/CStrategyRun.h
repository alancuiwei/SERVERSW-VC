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

    /* 策略名(由UI提供) */
    std::string strategyid;
    /* 保存该策略套利对的下单买入点，卖出点等参数。 */
    std::vector<CTradeStru*> runtradeparameters;
};

