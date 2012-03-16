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

    /* 策略名(由UI提供) */
    std::string strategyid;
    /* 保存该策略套利对的下单买入点，卖出点等参数。 */
    std::vector<CTradeStru*> runtradeparameters;
};

