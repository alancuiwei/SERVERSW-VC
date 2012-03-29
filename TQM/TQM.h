// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TQM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TQM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#ifdef TQM_EXPORTS
#define TQM_API __declspec(dllexport)
#else
#define TQM_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include "MdSpi.h"
#include "TraderSpi.h"
#include "CRTMarketData.h"

#define USETESTDATA  0        //是否使用测试数据库，1:是/ 0:否
/*internal*/
extern char *pInstrumentID[1024];	 // 行情订阅列表	
extern int iInstrumentID;	 // 行情订阅数量
extern  TThostFtdcBrokerIDType BROKER_ID;
extern  TThostFtdcInvestorIDType INVESTOR_ID;
extern  TThostFtdcPasswordType PASSWORD;
extern  CThostFtdcMdApi* pMdUserApi;
extern  CMdSpi* pMdUserSpi;
extern  CThostFtdcTraderApi* pTraderUserApi;
extern  CTraderSpi* pTraderUserSpi;
extern  HANDLE g_hMdReadyEvent;
//extern  HANDLE g_hSubcribeReadyEvent;
/*external*/
extern TQM_API bool tqm_isloginok;
extern TQM_API void StartMdThreadProcess( );
extern TQM_API void StartTraderThreadProcess(  );
extern TQM_API std::map<std::string, CRTMarketData*> tqm_prtmarketdatamap;
extern TQM_API bool SetSubscribeMarketData(std::map<std::string, CRTMarketData*> marketdatamap);
extern TQM_API char tqm_startdate[20]; 
extern TQM_API char tqm_starttime[20]; 
extern TQM_API bool tqm_issavetodb; 

// This class is exported from the TQM.dll
class TQM_API CTQM {
public:
	CTQM(void);
	// TODO: add your methods here.
};

extern TQM_API int nTQM;

TQM_API int fnTQM(void);


//extern FUTURESDATA_API void  Login(void);

typedef TQM_API struct 
{
	///合约代码
	char InstrumentID[31];
	///买卖方向	
	char Direction;
	///组合开平标志
	char CombOffsetFlag[5];
	///数量
	int VolumeTotalOriginal;
	///价格
 	double LimitPrice;
} Trader_InputOrderReq_t;


typedef TQM_API struct 
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
	///开始时间
	TThostFtdcTimeType	InsertTimeStart;
	///结束时间
	TThostFtdcTimeType	InsertTimeEnd;
} Trader_ReqQryOrder_t,Trader_ReqQryTrade_t;


///查询投资者持仓明细
typedef struct TQM_API  
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
}Trader_ReqQryInvestorPosition_t;


typedef struct TQM_API
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///组合持仓合约编码
	TThostFtdcInstrumentIDType	CombInstrumentID;
}Trader_ReqQryInvestorPositionCombineDetail_t;

///查询合约
typedef struct TQM_API
{
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///产品代码
	TThostFtdcInstrumentIDType	ProductID;
} Trader_ReqQryInstrument_t;

typedef struct TQM_API
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
} Trader_ReqQryTraderAccount_t;

typedef TQM_API struct
{
  union
  {
    Trader_InputOrderReq_t InputOrderReq_Parameters;
	Trader_ReqQryOrder_t ReqQryOrder_Parameters;
	Trader_ReqQryTrade_t ReqQryTrade_Parameters;
	Trader_ReqQryInvestorPosition_t ReqQryInvestorPosition_Parameters;
	Trader_ReqQryInvestorPositionCombineDetail_t ReqQryInvestorPositionCombineDetail_Parameters;
	Trader_ReqQryInstrument_t  ReqQryInstrument_Parameters;
	Trader_ReqQryTraderAccount_t ReqQryTraderAccount_Parameters;
	//其他消息结构
  } Parameters;
} Trader_MessageParameters_Req_t;

typedef TQM_API struct
{
    unsigned long MessageId;     
    Trader_MessageParameters_Req_t MsgParameter;
} Trader_MsgQData_t; 

#define Trader_InputOrder_Req                            0x1101
#define Trader_ReqQryOrder_Req                           0x1102
#define Trader_ReqQryTrade_Req                           0x1103
#define Trader_ReqQryInvestorPosition_Req                0x1104
#define Trader_ReqQryInvestorPositionCombineDetail_Req   0x1105
#define Trader_ReqQryInstrument_Req                      0x1106
#define Trader_ReqQryTradingAccount_Req					 0x1107

#define WM_MSG_TRADER                                    WM_USER + 1001
