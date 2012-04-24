#include "TQM.h"
#include "TraderSpi.h"
#include "../COMM/COMM.h"
using namespace std;
extern CLightThreadMutex g_ThreadMutex;
//全局文件流
ofstream TdDebugLog;
//temp
char *strtemp[600];
int numofstr=0;
/*
// USER_API参数
extern CThostFtdcTraderApi* pTraderUserApi;

// 配置参数
extern char TRADE_FRONT_ADDR[];	// 前置地址
extern char BROKER_ID[];		// 经纪公司代码
extern char INVESTOR_ID[];		// 投资者代码
extern char PASSWORD[];			// 用户密码
//extern char INSTRUMENT_ID[];	// 合约代码
extern TThostFtdcInstrumentIDType INSTRUMENT_ID;	// 合约代码
extern TThostFtdcPriceType	LIMIT_PRICE;	// 价格
extern TThostFtdcDirectionType	DIRECTION;	// 买卖方向
*/
// 请求编号
extern int iRequestID;

bool isTdOnRspOrderInsert;

//tydef strct
//{
//DCE 大连商品交易所
std::vector<std::string>  DCE_A; ///黄大豆1号
std::vector<std::string>  DCE_B; ///黄大豆2号
std::vector<std::string>  DCE_M; ///豆粕
std::vector<std::string>  DCE_Y; ///豆油
std::vector<std::string>  DCE_C; ///玉米
std::vector<std::string>  DCE_L; ///LLDPE
std::vector<std::string>  DCE_P; ///棕榈油
std::vector<std::string>  DCE_V; ///PVC
std::vector<std::string>  DCE_J; ///焦炭
//CZCE 郑州商品交易所
std::vector<std::string>  CZCE_SR; ///白糖
std::vector<std::string>  CZCE_TA; ///PTA
std::vector<std::string>  CZCE_CF; ///棉花
std::vector<std::string>  CZCE_WS; ///强麦
std::vector<std::string>  CZCE_WT; ///硬麦
std::vector<std::string>  CZCE_ER; ///籼稻
std::vector<std::string>  CZCE_RO; ///菜籽
std::vector<std::string>  CZCE_SPD; ///SPD
//SHFE 上海商品交易所
std::vector<std::string>  SHFE_CU; ///铜
std::vector<std::string>  SHFE_AL; ///铝
std::vector<std::string>  SHFE_ZN; ///锌
std::vector<std::string>  SHFE_RU; ///天胶
std::vector<std::string>  SHFE_FU; ///燃油
std::vector<std::string>  SHFE_AU; ///黄金
std::vector<std::string>  SHFE_RB; ///螺纹钢
std::vector<std::string>  SHFE_WR; ///线材
std::vector<std::string>  SHFE_PB; ///铅
//CFFE 中国金融期货交易所
std::vector<std::string>  CFFE_IF; ///股指期货
//} CurrentActiveInstrument;
//CurrentActiveInstrument ActInstru;

// 会话参数
TThostFtdcFrontIDType	FRONT_ID;	//前置编号
TThostFtdcSessionIDType	SESSION_ID;	//会话编号
TThostFtdcOrderRefType	ORDER_REF;	//报单引用

// 流控判断
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	printf("%s",__FUNCTION__);
	///用户登录请求
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	/**/CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pTraderUserApi->ReqUserLogin(&req, ++iRequestID);
	printf("Send User login %s",((iResult == 0) ? "Successful" : "failed"));
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// 保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///获取当前交易日
		printf("Current trading day = %s", pTraderUserApi->GetTradingDay());
		///投资者结算结果确认
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pTraderUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	printf("ReqSettlementInfoConfirm %s",((iResult == 0) ? "Successful" : "failed"));
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询合约
		ReqQryInstrument(NULL);
	}
}

/*
void CTraderSpi::ReqQryInstrument()
{

	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
//	strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInstrument %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInstrument %d, FlowControl",iResult);
			Sleep(1000);
		}
	} // while

}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

        if (pInstrument == NULL)
                return;

        std::string exchange(pInstrument->ExchangeID);
        std::string instru(pInstrument->InstrumentID);

        LOG4CXX_DEBUG(logger, "OnRspQryInstrument: '" << exchange << "' contains " << instru);
        if (exchange == "CFFEX") {
                apr_thread_rwlock_wrlock(rwLock4);
                cffex.insert(instru);
                apr_thread_rwlock_unlock(rwLock4);
                return;
        }
        if (exchange == "CZCE") {
                apr_thread_rwlock_wrlock(rwLock4);
                czce.insert(instru);
                apr_thread_rwlock_unlock(rwLock4);
                return;
        }
        if (exchange == "DCE") {
                apr_thread_rwlock_wrlock(rwLock4);
                dce.insert(instru);
                apr_thread_rwlock_unlock(rwLock4);
                return;
        }
        if (exchange == "SHFE") {
                apr_thread_rwlock_wrlock(rwLock4);
                shfe.insert(instru);
                apr_thread_rwlock_unlock(rwLock4);
        }

}
*/
/*
void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryTradingAccount %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryTradingAccount %d, FlowControl",iResult);
			Sleep(1000);
		}
	} // while
}*/

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		strcpy(TradingAccount_t.BrokerID,pTradingAccount->BrokerID);
		TradingAccount_t.PositionProfit = pTradingAccount->PositionProfit;
		FlagTradingAccout = false;
		return;
	}
}

/*
void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInvestorPosition %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInvestorPosition %d, FlowControl",iResult);
			Sleep(1000);
		}
	} // while
}
*/

/*
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///报单录入请求
		//ReqOrderInsert();
	}
}
*/
void CTraderSpi::ReqOrderInsert(char *param)
{
	/*isTdOnRspOrderInsert = false;
	Trader_InputOrderReq_t *tmp = (Trader_InputOrderReq_t*)param;
	char filename[64] = "";
	sprintf(filename, "TdDebug.csv" );
	TdDebugLog.open(filename, ios::app);
	TdDebugLog<<tmp->InstrumentID
		<<','<<tmp->Direction
		<<','<<tmp->CombOffsetFlag
		<<','<<tmp->VolumeTotalOriginal
		<<','<<tmp->LimitPrice<<endl;
	TdDebugLog.close();

	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	///合约代码
	strcpy(req.InstrumentID, tmp->InstrumentID);
	///报单引用
	strcpy(req.OrderRef, ORDER_REF);
	///用户代码
	//TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///买卖方向:
	req.Direction = tmp->Direction;
	///组合开平标志: 开仓
	req.CombOffsetFlag[0] = tmp->CombOffsetFlag[0];
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = tmp->LimitPrice;
	///数量: 1
	req.VolumeTotalOriginal = tmp->VolumeTotalOriginal;
	///有效期类型: 当日有效
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	req.UserForceClose = 0;

	int iResult = pTraderUserApi->ReqOrderInsert(&req, ++iRequestID);
	printf("ReqOrderInsert %s",((iResult == 0) ? "Successful" : "failed"));
	*/
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	isTdOnRspOrderInsert =true;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//是否发送了报单
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, pOrder->BrokerID);
	///投资者代码
	strcpy(req.InvestorID, pOrder->InvestorID);
	///报单操作引用
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
	strcpy(req.OrderRef, pOrder->OrderRef);
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///前置编号
	req.FrontID = FRONT_ID;
	///会话编号
	req.SessionID = SESSION_ID;
	///交易所代码
	//	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///价格
	//	TThostFtdcPriceType	LimitPrice;
	///数量变化
	//	TThostFtdcVolumeType	VolumeChange;
	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///合约代码
	strcpy(req.InstrumentID, pOrder->InstrumentID);

	int iResult = pTraderUserApi->ReqOrderAction(&req, ++iRequestID);
	printf("ReqOrderAction %s",((iResult == 0) ? "Successful" : "failed"));
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	printf("%s",__FUNCTION__);
	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
			ReqOrderAction(pOrder);
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			printf("Cancel Order Successful");
	}
}

///成交通知
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	printf("%s",__FUNCTION__);
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	printf("%s",__FUNCTION__);;
	printf("Reason = %d", nReason);
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	printf("%s",__FUNCTION__);
	printf("nTimerLapse = %d", nTimeLapse);
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		printf("ErrorID= %d, ErrorMsg= %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
		(pOrder->SessionID == SESSION_ID) &&
		(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}


///请求查询报单
int CTraderSpi::ReqQryOrder(char *param)
{
	/*Trader_ReqQryOrder_t *tmp = (Trader_ReqQryOrder_t*)param;
	char filename[64] = "";
	sprintf(filename, "TdDebug.csv" );
	TdDebugLog.open(filename, ios::app);
	TdDebugLog<<tmp->BrokerID
		<<','<<tmp->InvestorID
		<<','<<tmp->InstrumentID
		<<','<<tmp->ExchangeID
		<<','<<tmp->OrderSysID
		<<','<<tmp->InsertTimeStart
		<<','<<tmp->InsertTimeEnd
		<<endl;

	CThostFtdcQryOrderField req;
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, tmp->InstrumentID);
	strcpy(req.ExchangeID, tmp->ExchangeID);
	strcpy(req.OrderSysID, tmp->OrderSysID);
	strcpy(req.InsertTimeStart, tmp->InsertTimeStart);
	strcpy(req.InsertTimeEnd, tmp->InsertTimeEnd);

	int iResult = pTraderUserApi->ReqQryOrder(&req, ++iRequestID);
	printf("ReqQryOrder %s",((iResult == 0) ? "Successful" : "failed"));
	if(iResult == 0)
	{
		TdDebugLog<<"Send ReqQryOrder Successful"<<endl;
	}
	else
	{
		TdDebugLog<<"Send ReqQryOrder failed"<<endl;
	}
	TdDebugLog.close();
	return iResult;*/
	return 0;

}
///请求查询成交
int CTraderSpi::ReqQryTrade(char *param)
{
	/*Trader_ReqQryTrade_t *tmp = (Trader_ReqQryTrade_t*)param;
	char filename[64] = "";
	sprintf(filename, "TdDebug.csv" );
	TdDebugLog.open(filename, ios::app);
	TdDebugLog<<tmp->BrokerID
		<<','<<tmp->InvestorID
		<<','<<tmp->InstrumentID
		<<','<<tmp->ExchangeID
		<<','<<tmp->OrderSysID
		<<','<<tmp->InsertTimeStart
		<<','<<tmp->InsertTimeEnd
		<<endl;

	CThostFtdcQryOrderField req;
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.ExchangeID, tmp->ExchangeID);
	strcpy(req.OrderSysID, tmp->OrderSysID);
	strcpy(req.InsertTimeStart, tmp->InsertTimeStart);
	strcpy(req.InsertTimeEnd, tmp->InsertTimeEnd);

	int iResult = pTraderUserApi->ReqQryOrder(&req, ++iRequestID);
	printf("ReqQryTrade %s",((iResult == 0) ? "Successful" : "failed"));
	if(iResult == 0)
	{
		TdDebugLog<<"Send ReqQryTrade Successful"<<endl;
	}
	else
	{
		TdDebugLog<<"Send ReqQryTrade failed"<<endl;
	}
	TdDebugLog.close();
	return iResult;*/
	return 0;
}
///请求查询投资者持仓
int CTraderSpi::ReqQryInvestorPosition(char *param)
{
	/*Trader_ReqQryInvestorPosition_t *tmp = (Trader_ReqQryInvestorPosition_t*)param;
	char filename[64] = "";
	sprintf(filename, "TdDebug.csv" );
	TdDebugLog.open(filename, ios::app);
	TdDebugLog<<tmp->BrokerID
		<<','<<tmp->InvestorID
		<<','<<tmp->InstrumentID
		<<endl;
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, tmp->InstrumentID);

	int iResult = 0;
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInvestorPosition %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInvestorPosition %d, FlowControl",iResult);
			Sleep(1000);
		}
	}
	if(iResult == 0)
	{
		TdDebugLog<<"Send ReqQryInvestorPosition Successful"<<endl;
	}
	else
	{
		TdDebugLog<<"Send ReqQryInvestorPosition failed"<<endl;
	}
	TdDebugLog.close();
	return iResult;*/
	return 0;

}
///请求查询投资者持仓明细
int CTraderSpi::ReqQryInvestorPositionDetail(char *param)
{
	/*Trader_ReqQryInvestorPositionCombineDetail_t *tmp = (Trader_ReqQryInvestorPositionCombineDetail_t*)param;
	char filename[64] = "";
	sprintf(filename, "TdDebug.csv" );
	TdDebugLog.open(filename, ios::app);
	TdDebugLog<<tmp->BrokerID
		<<','<<tmp->InvestorID
		<<','<<tmp->CombInstrumentID
		<<endl;
	CThostFtdcQryInvestorPositionDetailField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, tmp->CombInstrumentID);

	int iResult = 0;
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryInvestorPositionDetail(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInvestorPositionDetail %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInvestorPositionDetail %d, FlowControl",iResult);
			Sleep(1000);
		}
	}
	if(iResult == 0)
	{
		TdDebugLog<<"Send ReqQryInvestorPositionDetail Successful"<<endl;
	}
	else
	{
		TdDebugLog<<"Send ReqQryInvestorPositionDetail failed"<<endl;
	}
	TdDebugLog.close();
	return iResult;
	*/
	return 0;
}

int CTraderSpi::ReqQryTradingAccount(char *param)
{
	//Trader_ReqQryTraderAccount_t *tmp = (Trader_ReqQryTraderAccount_t*)param;

/*
	char filename[64] = "";
	sprintf(filename, "TdDebug.csv" );
	TdDebugLog.open(filename, ios::app);
	TdDebugLog<<tmp->BrokerID
		<<','<<tmp->InvestorID
		<<endl;
*/
	/*FlagTradingAccout = true;
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = -1;
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryTradingAccount %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryTradingAccount %d, FlowControl",iResult);
			Sleep(1000);
		}
	} // while
	return iResult;*/
	return 0;
}

///请求查询报单响应
void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		char filename[64] = "";
		sprintf(filename, "TdDebug.csv" );
		TdDebugLog.open(filename, ios::app);
		TdDebugLog<<pOrder->InstrumentID
			<<','<<pOrder->LimitPrice
			<<','<<pOrder->VolumeTotalOriginal
			<<endl;
		TdDebugLog.close();
	}
}

///请求查询成交响应
void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		char filename[64] = "";
		sprintf(filename, "TdDebug.csv" );
		TdDebugLog.open(filename, ios::app);
		TdDebugLog<<pTrade->InstrumentID
			<<','<<pTrade->TradeID
			<<','<<pTrade->OrderSysID
			<<','<<pTrade->Volume
			<<endl;
		TdDebugLog.close();
	}

}

///请求查询投资者持仓响应
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		char filename[64] = "";
		sprintf(filename, "TdDebug.csv" );
		TdDebugLog.open(filename, ios::app);
		TdDebugLog<<pInvestorPosition->InstrumentID
			<<','<<pInvestorPosition->OpenVolume
			<<','<<pInvestorPosition->CloseVolume
			<<','<<pInvestorPosition->OpenAmount
			<<','<<pInvestorPosition->CloseAmount
			<<endl;
		TdDebugLog.close();
	}

}

///请求查询投资者持仓明细响应
void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		char filename[64] = "";
		sprintf(filename, "TdDebug.csv" );
		TdDebugLog.open(filename, ios::app);
		TdDebugLog<<pInvestorPositionDetail->InstrumentID
			<<','<<pInvestorPositionDetail->OpenPrice
			<<','<<pInvestorPositionDetail->CombInstrumentID
			<<endl;
		TdDebugLog.close();
	}

}

int CTraderSpi::ReqQryInstrument(char *param)
{

	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	//strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = -1;
	while (true)
	{
		iResult = pTraderUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInstrument %s",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInstrument %d, FlowControl",iResult);
			usleep(1000000);
		}
	} // while
	return iResult;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//if last Rsp, Sort and save
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//SetEvent(g_hMdReadyEvent);
		g_ThreadMutex.Unlock();
	}
	else
	{
		if((strstr(pInstrument->InstrumentID, "SP") == NULL)&&
			(strstr(pInstrument->InstrumentID, "efp") == NULL))
		{
			 pInstrumentID[iInstrumentID]= new char [10];
			 memcpy(pInstrumentID[iInstrumentID],pInstrument->InstrumentID,10);
			CRTMarketData *marketdata = new CRTMarketData( pInstrumentID[iInstrumentID]);
			//}
			tqm_prtmarketdatamap.insert( pair<std::string, CRTMarketData*>(pInstrumentID[iInstrumentID], marketdata) );
			 iInstrumentID++;
		}
	}
}
