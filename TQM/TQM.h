#pragma once
#include <map>
//#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
//#include <wtypes.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <sys/errno.h>

#include "MdSpi.h"
#include "TraderSpi.h"
#include "CRTMarketData.h"

#define USETESTDATA  0       //�Ƿ�ʹ�ò������ݿ⣬1:��/ 0:��
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
// extern  HANDLE g_hMdReadyEvent;

//extern  HANDLE g_hSubcribeReadyEvent;
/*external*/
extern  bool tqm_isloginok;
extern  void StartMdThreadProcess( );
extern  void StartTraderThreadProcess(  );
extern  std::map<std::string, CRTMarketData*> tqm_prtmarketdatamap;
extern  bool SetSubscribeMarketData(std::map<std::string, CRTMarketData*> marketdatamap);
extern  char tqm_startdate[20];
extern  char tqm_starttime[20];
extern  bool tqm_issavetodb;

// This class is exported from the TQM.dll
class  CTQM {
public:
	CTQM(void);
	// TODO: add your methods here.
};

extern  int nTQM;

int fnTQM(void);


//extern FUTURESDATA_API void  Login(void);

typedef  struct
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


typedef  struct
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
typedef struct
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
}Trader_ReqQryInvestorPosition_t;


typedef struct
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
}Trader_ReqQryInvestorPositionCombineDetail_t;

///��ѯ��Լ
typedef struct
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

typedef struct
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
} Trader_ReqQryTraderAccount_t;

typedef  struct
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

typedef  struct
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
