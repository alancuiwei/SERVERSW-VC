#include <string.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "TraderSpi.h"
using namespace std;
///this type is used for
struct STraderInfo
{
    char                                 frontaddr[255];
    TThostFtdcBrokerIDType	             brokerid;
    TThostFtdcInvestorIDType             investorid;
    TThostFtdcPasswordType               password;
};
struct SSessionInfo
{
    TThostFtdcFrontIDType	             frontid;
    TThostFtdcSessionIDType	             sessionid;
    TThostFtdcOrderRefType	             orderref;
};

enum ETradePhase
{
    E_TD_None = 0,
	E_TD_OnFrontConnected,
	E_TD_ReqUserLogin,
	E_TD_OnRspUserLogin,
	E_TD_OnRspQrySettlementInfo,
	E_TD_OnRspQrySettlementInfoConfirm,
	E_TD_ReqSettlementInfoConfirm,
	E_TD_OnRspSettlementInfoConfirm,
	E_TD_ReqQryInstrument,
	E_TD_OnRspQryInstrument,
	E_TD_OnRspOrderInsert,
	E_TD_OnRspQryInstrumentMarginRate,
	E_TD_OnRspQryInstrument_Done
};

/// this class is used to trade order
class  CTrader : public CThostFtdcTraderSpi
{
public:
    int                                        requestid;
    STraderInfo                                traderinfo;
    SSessionInfo                               sessioninfo;
    char                                       pInstrumentID[500][10];
    int                                        iInstrumentID;
    CThostFtdcTraderApi*                       pTraderUserApi;
    CTraderSpi*                                pTraderUserSpi;
    ETradePhase                                tradephase;
    CThostFtdcRspInfoField                     rspinfo;
    CThostFtdcOrderField                       orderinfo;
    vector<CThostFtdcOrderField>               quyorder;
    vector<CThostFtdcInstrumentField>          instrumentfield;
    vector<CThostFtdcSettlementInfoField>      settlementinfo;
    char                                       currentdate[9];

    int                                        rsptimeout;
    int                                        rspcheckclip;
    bool                                 isrsperror;
    bool                                 issettlmentconfirmed;
    bool                                 isstarted;
    static void* Agent(void *p);
    int  StartTrader(void);
    void RunTrader(void);
    void StopTrader(void);

	CTrader(void);
    CTrader(STraderInfo info);
	~CTrader(void);

	int ExtOrderinsert(char* orderstr, int price);

	///µ±¿Í»§¶ËÓë½»Ò×ºóÌ¨½¨Á¢ÆðÍ¨ÐÅÁ¬½ÓÊ±£¨»¹Î´µÇÂ¼Ç°£©£¬¸Ã·½·¨±»µ÷ÓÃ¡£
	virtual void OnFrontConnected();
	///µÇÂ¼ÇëÇóÏìÓ¦
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///Í¶×ÊÕß½áËã½á¹ûÈ·ÈÏÏìÓ¦
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯×Ê½ðÕË»§ÏìÓ¦
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÏìÓ¦
	//virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///±¨µ¥Â¼ÈëÇëÇóÏìÓ¦
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///±¨µ¥²Ù×÷ÇëÇóÏìÓ¦
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///´íÎóÓ¦´ð
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///µ±¿Í»§¶ËÓë½»Ò×ºóÌ¨Í¨ÐÅÁ¬½Ó¶Ï¿ªÊ±£¬¸Ã·½·¨±»µ÷ÓÃ¡£µ±·¢ÉúÕâ¸öÇé¿öºó£¬API»á×Ô¶¯ÖØÐÂÁ¬½Ó£¬¿Í»§¶Ë¿É²»×ö´¦Àí¡£
	virtual void OnFrontDisconnected(int nReason);
	///ÐÄÌø³¬Ê±¾¯¸æ¡£µ±³¤Ê±¼äÎ´ÊÕµ½±¨ÎÄÊ±£¬¸Ã·½·¨±»µ÷ÓÃ¡£
	virtual void OnHeartBeatWarning(int nTimeLapse);
	///±¨µ¥Í¨Öª
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	///³É½»Í¨Öª
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///ÇëÇó²éÑ¯±¨µ¥ÏìÓ¦
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯³É½»ÏìÓ¦
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÏìÓ¦
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÃ÷Ï¸ÏìÓ¦
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///ÇëÇó²éÑ¯ºÏÔ¼ÏìÓ¦
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯Í¶×ÊÕß½áËã½á¹ûÏìÓ¦
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯½áËãÐÅÏ¢È·ÈÏÏìÓ¦
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///ÇëÇó²éÑ¯ºÏÔ¼±£Ö¤½ðÂÊÏìÓ¦
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///ÓÃ»§µÇÂ¼ÇëÇó
	int ReqUserLogin();
	///±¨µ¥Â¼ÈëÇëÇó
	int ReqOrderInsert(CThostFtdcInputOrderField req);
	///ÇëÇó²éÑ¯±¨µ¥
	int ReqQryOrder(CThostFtdcQryOrderField req);
	///ÇëÇó²éÑ¯³É½»
	int ReqQryTrade(char *param);
	///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²Ö
	int ReqQryInvestorPosition(char *param);
	///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÃ÷Ï¸
	int ReqQryInvestorPositionDetail(char *param);
	///ÇëÇó²éÑ¯ºÏÔ¼
	int ReqQryInstrument(char *param);
	//ÇëÇó²éÑ¯Í¶×ÊÕßÕË»§½ð¶î
	int ReqQryTradingAccount(char *param);
	///Í¶×ÊÕß½áËã½á¹ûÈ·ÈÏ
	int ReqQrySettlementInfoConfirm();
	int ReqSettlementInfoConfirm();
	int ReqQrySettlementInfo();
	///ÇëÇó²éÑ¯×Ê½ðÕË»§
	//void ReqQryTradingAccount();
	///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²Ö
	//void ReqQryInvestorPosition();
	///±¨µ¥²Ù×÷ÇëÇó
	void ReqOrderAction(CThostFtdcOrderField *pOrder);
	///ÇëÇó²éÑ¯ºÏÔ¼±£Ö¤½ðÂÊ
    int ReqQryInstrumentMarginRate();


private:
	// TODO: add your methods here.



    int WaitRsp(ETradePhase rspphase);
	// ÊÇ·ñÊÕµ½³É¹¦µÄÏìÓ¦
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// ÊÇ·ñÎÒµÄ±¨µ¥»Ø±¨
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// ÊÇ·ñÕýÔÚ½»Ò×µÄ±¨µ¥
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);
	bool IsFlowControl(int iResult);
};
