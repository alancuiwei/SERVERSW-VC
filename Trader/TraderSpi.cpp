#include "CTrader.h"
#include "TraderSpi.h"

using namespace std;


void CTraderSpi::SetTrader( CTrader * ptd )
{
	pTrader = ptd;
}


void CTraderSpi::OnFrontConnected()
{
	printf("%s \n",__FUNCTION__);
	pTrader->OnFrontConnected();
}


void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
    pTrader->OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

void CTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
    pTrader->OnRspQrySettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
}


void CTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    printf("%s \n",__FUNCTION__);
    IsErrorRspInfo(pRspInfo);
    pTrader->OnRspQrySettlementInfo(pSettlementInfo, pRspInfo, nRequestID, bIsLast);
}


void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
    pTrader->OnRspSettlementInfoConfirm(pSettlementInfoConfirm, pRspInfo, nRequestID, bIsLast);
}


void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryTradingAccount(pTradingAccount, pRspInfo, nRequestID, bIsLast);
}


void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
}


void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspOrderAction(pInputOrderAction, pRspInfo, nRequestID, bIsLast);
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	printf("%s \n",__FUNCTION__);
    pTrader->OnRtnOrder(pOrder);
}

///成交通知
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	printf("%s \n",__FUNCTION__);
	pTrader->OnRtnTrade(pTrade);
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	printf("%s : Reason = %d \n",__FUNCTION__, nReason);
	pTrader->OnFrontDisconnected(nReason);

}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	printf("%s \n",__FUNCTION__);
	printf("nTimerLapse = %d", nTimeLapse);
	pTrader->OnHeartBeatWarning(nTimeLapse);
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspError(pRspInfo, nRequestID, bIsLast);
}


///请求查询报单响应
void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
}

///请求查询成交响应
void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryTrade(pTrade, pRspInfo, nRequestID, bIsLast);

}

///请求查询投资者持仓响应
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryInvestorPosition(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
}

///请求查询投资者持仓明细响应
void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryInvestorPositionDetail(pInvestorPositionDetail, pRspInfo, nRequestID, bIsLast);
}


void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		char filename[64] = "";
		sprintf(filename, "TdRspError.log" );
		ofstream TdDebugLog;
		TdDebugLog.open(filename, ios::app);
		TdDebugLog<<"ErrorID="
			<<pRspInfo->ErrorID <<"; "
			<<"ErrorMsg="
			<<pRspInfo->ErrorMsg
			<<endl;
		TdDebugLog.close();
		pTrader->rspinfo = *pRspInfo;
		pTrader->isrsperror = true;
		printf("ErrorID= %d; ErrorMsg= %s \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	return bResult;
}

void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//printf("%s \n",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
	pTrader->OnRspQryInstrumentMarginRate(pInstrumentMarginRate, pRspInfo, nRequestID, bIsLast);
}
