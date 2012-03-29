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

#define USETESTDATA  0        //�Ƿ�ʹ�ò������ݿ⣬1:��/ 0:��
/*internal*/
extern char *pInstrumentID[1024];	 // ���鶩���б�	
extern int iInstrumentID;	 // ���鶩������
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
	///��Լ����
	char InstrumentID[31];
	///��������	
	char Direction;
	///��Ͽ�ƽ��־
	char CombOffsetFlag[5];
	///����
	int VolumeTotalOriginal;
	///�۸�
 	double LimitPrice;
} Trader_InputOrderReq_t;


typedef TQM_API struct 
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
} Trader_ReqQryOrder_t,Trader_ReqQryTrade_t;


///��ѯͶ���ֲ߳���ϸ
typedef struct TQM_API  
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
}Trader_ReqQryInvestorPosition_t;


typedef struct TQM_API
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
}Trader_ReqQryInvestorPositionCombineDetail_t;

///��ѯ��Լ
typedef struct TQM_API
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
} Trader_ReqQryInstrument_t;

typedef struct TQM_API
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
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
	//������Ϣ�ṹ
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
