#include "CTrader.h"
#include "../COMM/lightthread.h"

CTrader::CTrader(void)
{
    strcpy(traderinfo.frontaddr, "tcp://gwf-front1.financial-trading-platform.com:41205");
    strcpy(traderinfo.brokerid, "8080");
    strcpy(traderinfo.investorid, "11800387");
    strcpy(traderinfo.password, "118403");

    /*strcpy(traderinfo.frontaddr, "tcp://asp-sim2-dx-front1.financial-trading-platform.com:26205");
    strcpy(traderinfo.brokerid, "2030");
    strcpy(traderinfo.investorid, "352589");
    strcpy(traderinfo.password, "123321");*/

    requestid = 0;
    iInstrumentID = 0;
    tradephase = E_TD_None;
    isrsperror = false;
    issettlmentconfirmed = false;
    rsptimeout = 2000000;
    rspcheckclip = 20000;
    pTraderUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    pTraderUserSpi = new CTraderSpi();
    pTraderUserSpi->SetTrader(this);
    isstarted = false;
}

CTrader::~CTrader(void)
{
    if (pTraderUserSpi!=NULL)
    {
        delete pTraderUserSpi;
    }
}

CTrader::CTrader(STraderInfo info)
{
    traderinfo = info;
    requestid = 0;
    iInstrumentID = 0;
    tradephase = E_TD_None;
    isrsperror = false;
    issettlmentconfirmed = false;
    rsptimeout = 5000000;
    rspcheckclip = 20000;
    pTraderUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./td");
    pTraderUserSpi = new CTraderSpi();
    pTraderUserSpi->SetTrader(this);
    isstarted = false;
}

void* CTrader::Agent(void *p)
{
    CTrader *agt = (CTrader *)p;
    agt->RunTrader();
    return p;
}

int CTrader::StartTrader(void)
{
    isstarted = true;
	pTraderUserApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderUserSpi);
	pTraderUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
	pTraderUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	pTraderUserApi->RegisterFront(traderinfo.frontaddr);
	pTraderUserApi->Init();
	CLightThread::CreateThread(Agent, (void *)this);
	int rtn = WaitRsp(E_TD_OnFrontConnected);
    if(rtn>0)
    {
        rtn = ReqUserLogin();
        if(rtn>0)
        {
            rtn = ReqQrySettlementInfoConfirm();
            if((rtn>0)&&(!issettlmentconfirmed))
            {
                rtn = ReqSettlementInfoConfirm();
            }
            else
            {
                cout<<"today no need comfirm again!"<<endl;
            }
        }
    }
    return rtn;
}

void CTrader::RunTrader(void)
{
	pTraderUserApi->Join();
}

void CTrader::StopTrader(void)
{
    pTraderUserApi->Release();
}


// Á÷¿ØÅÐ¶Ï
bool CTrader::IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

int CTrader::WaitRsp(ETradePhase rspphase)
{
    int count = rsptimeout/rspcheckclip;
    while((tradephase != rspphase)&&( count>0 ))
    {
        usleep(rspcheckclip);
        count--;
    }
    if (tradephase == rspphase)
    {
        if(isrsperror)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

void CTrader::OnFrontConnected()
{
    tradephase = E_TD_OnFrontConnected;
}

int CTrader::ReqUserLogin()
{
    tradephase = E_TD_ReqUserLogin;
    isrsperror = false;
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, traderinfo.brokerid);
	strcpy(req.UserID, traderinfo.investorid);
	strcpy(req.Password,traderinfo.password);
	int iResult = pTraderUserApi->ReqUserLogin(&req, ++requestid);
	printf("Send User login %s \n",((iResult == 0) ? "Successful" : "failed"));
	return WaitRsp(E_TD_OnRspUserLogin);
}

void CTrader::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
	    if(!isrsperror)
	    {
            // ±£´æ»á»°²ÎÊý
            sessioninfo.frontid = pRspUserLogin->FrontID;
            sessioninfo.sessionid = pRspUserLogin->SessionID;
            int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
            iNextOrderRef++;
            sprintf(sessioninfo.orderref, "%d", iNextOrderRef);

            ///»ñÈ¡µ±Ç°½»Ò×ÈÕ
            printf("currentdate = %s \n", pTraderUserApi->GetTradingDay());
            //cout<<pTraderUserApi->GetTradingDay()<<endl;
            strcpy(currentdate, pTraderUserApi->GetTradingDay());
	    }
	    tradephase = E_TD_OnRspUserLogin;
	}
}

int CTrader::ReqQrySettlementInfoConfirm()
{
	CThostFtdcQrySettlementInfoConfirmField req;
	isrsperror = false;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, traderinfo.brokerid);
	strcpy(req.InvestorID, traderinfo.investorid);
	int iResult = pTraderUserApi->ReqQrySettlementInfoConfirm(&req, ++requestid);
	printf("ReqQrySettlementInfoConfirm %s \n",((iResult == 0) ? "Successful" : "failed"));
	return WaitRsp(E_TD_OnRspQrySettlementInfoConfirm);
}

void CTrader::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
                                     CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
	    if((!isrsperror)&&(pSettlementInfoConfirm != NULL))
	    {
	        cout <<"last SettlementInfoConfirm:"<<pSettlementInfoConfirm->ConfirmDate<<endl;
	        issettlmentconfirmed = true;
	    }
	    tradephase = E_TD_OnRspQrySettlementInfoConfirm;
	}
}

int CTrader::ReqQrySettlementInfo()
{
	CThostFtdcQrySettlementInfoField req;
	isrsperror = false;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, traderinfo.brokerid);
	strcpy(req.InvestorID, traderinfo.investorid);
	int iResult = pTraderUserApi->ReqQrySettlementInfo(&req, ++requestid);
	printf("ReqQrySettlementInfo %s \n",((iResult == 0) ? "Successful" : "failed"));
	return WaitRsp(E_TD_OnRspQrySettlementInfo);
}

void CTrader::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    settlementinfo.push_back(*pSettlementInfo);
	if (bIsLast)
	{
	    if(isrsperror)
	    {

	    }
	    tradephase = E_TD_OnRspQrySettlementInfo;
	}
}

int CTrader::ReqSettlementInfoConfirm()
{
    tradephase = E_TD_ReqSettlementInfoConfirm;
    isrsperror = false;
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, traderinfo.brokerid);
	strcpy(req.InvestorID, traderinfo.investorid);
	int iResult = pTraderUserApi->ReqSettlementInfoConfirm(&req, ++requestid);
	printf("ReqSettlementInfoConfirm %s \n",((iResult == 0) ? "Successful" : "failed"));
	return WaitRsp(E_TD_OnRspSettlementInfoConfirm);
}

void CTrader::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    //tradephase = E_TD_OnRspSettlementInfoConfirm;
	if (bIsLast)
	{
	    if(isrsperror)
	    {

	    }
	    tradephase = E_TD_OnRspSettlementInfoConfirm;
	}
}


/*
void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pTraderUserApi->ReqQryTradingAccount(&req, ++pTrader->requestid);
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

void CTrader::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	/*printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		strcpy(TradingAccount_t.BrokerID,pTradingAccount->BrokerID);
		TradingAccount_t.PositionProfit = pTradingAccount->PositionProfit;
		FlagTradingAccout = false;
		return;
	}*/
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
		int iResult = pTraderUserApi->ReqQryInvestorPosition(&req, ++pTrader->requestid);
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
		///±¨µ¥Â¼ÈëÇëÇó
		//ReqOrderInsert();
	}
}
*/
int CTrader::ReqOrderInsert(CThostFtdcInputOrderField req)
{
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, traderinfo.brokerid);
	strcpy(req.InvestorID, traderinfo.investorid);
	strcpy(req.UserID, traderinfo.investorid);

	///±¨µ¥ÒýÓÃ
	strcpy(req.OrderRef, sessioninfo.orderref);
    int iNextOrderRef = atoi(sessioninfo.orderref);
    iNextOrderRef++;
    sprintf(sessioninfo.orderref, "%d", iNextOrderRef);

    ///none
	///ºÏÔ¼´úÂë
	//strcpy(req.InstrumentID, "cu1209");
	///±¨µ¥¼Û¸ñÌõ¼þ: ÏÞ¼Û
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///ÂòÂô·½Ïò:
	req.Direction = THOST_FTDC_D_Buy;
	///×éºÏ¿ªÆ½±êÖ¾: ¿ª²Ö
	memset(req.CombOffsetFlag, 0, sizeof(req.CombOffsetFlag));
	req.CombOffsetFlag[0] = THOST_FTDC_RA_Trade;
	///×éºÏÍ¶»úÌ×±£±êÖ¾
	memset(req.CombHedgeFlag, 0, sizeof(req.CombHedgeFlag));
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///¼Û¸ñ
	//req.LimitPrice = 50000;
	///ÊýÁ¿: 1
	req.VolumeTotalOriginal = 1;
	///ÓÐÐ§ÆÚÀàÐÍ: µ±ÈÕÓÐÐ§
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTDÈÕÆÚ
	strcpy(req.GTDDate, currentdate);
	///³É½»Á¿ÀàÐÍ: ÈÎºÎÊýÁ¿
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///×îÐ¡³É½»Á¿: 1
	req.MinVolume = 1;
	///´¥·¢Ìõ¼þ: Á¢¼´
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///Ö¹Ëð¼Û
	req.StopPrice = 0;
	///Ç¿Æ½Ô­Òò: ·ÇÇ¿Æ½
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///×Ô¶¯¹ÒÆð±êÖ¾: ·ñ
	req.IsAutoSuspend = 0;
	///ÒµÎñµ¥Ôª
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///ÇëÇó±àºÅ
	req.RequestID = requestid;
	///ÓÃ»§Ç¿ÆÀ±êÖ¾: ·ñ
	req.UserForceClose = 0;


    isrsperror = false;
	int iResult = pTraderUserApi->ReqOrderInsert(&req, ++requestid);
	printf("ReqOrderInsert %s \n",((iResult == 0) ? "Successful" : "failed"));

	return WaitRsp(E_TD_OnRspOrderInsert);

}

void CTrader::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
	    if(isrsperror)
	    {

	    }
	    tradephase = E_TD_OnRspOrderInsert;
	}
}

void CTrader::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//ÊÇ·ñ·¢ËÍÁË±¨µ¥
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///¾­¼Í¹«Ë¾´úÂë
	strcpy(req.BrokerID, pOrder->BrokerID);
	///Í¶×ÊÕß´úÂë
	strcpy(req.InvestorID, pOrder->InvestorID);
	///±¨µ¥²Ù×÷ÒýÓÃ
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///±¨µ¥ÒýÓÃ
	strcpy(req.OrderRef, pOrder->OrderRef);
	///ÇëÇó±àºÅ
	//	TThostFtdcRequestIDType	RequestID;
	///Ç°ÖÃ±àºÅ
	req.FrontID = sessioninfo.frontid;
	///»á»°±àºÅ
	req.SessionID = sessioninfo.sessionid;
	///½»Ò×Ëù´úÂë
	//	TThostFtdcExchangeIDType	ExchangeID;
	///±¨µ¥±àºÅ
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///²Ù×÷±êÖ¾
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///¼Û¸ñ
	//	TThostFtdcPriceType	LimitPrice;
	///ÊýÁ¿±ä»¯
	//	TThostFtdcVolumeType	VolumeChange;
	///ÓÃ»§´úÂë
	//	TThostFtdcUserIDType	UserID;
	///ºÏÔ¼´úÂë
	strcpy(req.InstrumentID, pOrder->InstrumentID);

	int iResult = pTraderUserApi->ReqOrderAction(&req, ++requestid);
	printf("ReqOrderAction %s \n",((iResult == 0) ? "Successful" : "failed"));
	ORDER_ACTION_SENT = true;
}

void CTrader::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//IsErrorRspInfo(pRspInfo);
}

///±¨µ¥Í¨Öª
void CTrader::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (IsMyOrder(pOrder))
	{
	    orderinfo = *pOrder;
	    printf("MyOrder: %d \n", pOrder->OrderStatus);
		if (IsTradingOrder(pOrder))
        {
            printf("IsTradingOrder \n");
			//ReqOrderAction(pOrder);
        }
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			printf("Cancel Order Successful \n");
	}
}

///³É½»Í¨Öª
void CTrader::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
}

void CTrader:: OnFrontDisconnected(int nReason)
{
}

void CTrader::OnHeartBeatWarning(int nTimeLapse)
{
}

void CTrader::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}

bool CTrader::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// Èç¹ûErrorID != 0, ËµÃ÷ÊÕµ½ÁË´íÎóµÄÏìÓ¦
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		printf("ErrorID= %d, ErrorMsg= %s \n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}

bool CTrader::IsMyOrder(CThostFtdcOrderField *pOrder)
{
    cout << sessioninfo.orderref << endl;
	return ((pOrder->FrontID == sessioninfo.frontid) &&
		(pOrder->SessionID == sessioninfo.sessionid));// &&
		//(strcmp(pOrder->OrderRef, pTrader->sessioninfo.orderref) == 0));
}

bool CTrader::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}


///ÇëÇó²éÑ¯±¨µ¥
int CTrader::ReqQryOrder(CThostFtdcQryOrderField req)
{
	strcpy(req.BrokerID, traderinfo.brokerid);
	strcpy(req.InvestorID, traderinfo.investorid);
    quyorder.clear();
	int iResult = pTraderUserApi->ReqQryOrder(&req, ++requestid);
	if(iResult == 0)
	{
		cout<<"Send ReqQryOrder Successful"<<endl;
	}
	else
	{
		cout<<"Send ReqQryOrder failed"<<endl;
	}
	return iResult;

}
///ÇëÇó²éÑ¯³É½»
int CTrader::ReqQryTrade(char *param)
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

	int iResult = pTraderUserApi->ReqQryOrder(&req, ++pTrader->requestid);
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
///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²Ö
int CTrader::ReqQryInvestorPosition(char *param)
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
		int iResult = pTraderUserApi->ReqQryInvestorPosition(&req, ++pTrader->requestid);
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
///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÃ÷Ï¸
int CTrader::ReqQryInvestorPositionDetail(char *param)
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
		int iResult = pTraderUserApi->ReqQryInvestorPositionDetail(&req, ++pTrader->requestid);
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

int CTrader::ReqQryTradingAccount(char *param)
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
		int iResult = pTraderUserApi->ReqQryTradingAccount(&req, ++pTrader->requestid);
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

///ÇëÇó²éÑ¯±¨µ¥ÏìÓ¦
void CTrader::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pOrder!=NULL)
    {
        cout<<pOrder->InstrumentID
            <<','<<pOrder->LimitPrice
            <<','<<pOrder->VolumeTotalOriginal
            <<endl;
        quyorder.push_back(*pOrder);
    }
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//char filename[64] = "";
		//sprintf(filename, "TdDebug.csv" );
		//TdDebugLog.open(filename, ios::app);

		//TdDebugLog.close();
	}
}

///ÇëÇó²éÑ¯³É½»ÏìÓ¦
void CTrader::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//char filename[64] = "";
		//sprintf(filename, "TdDebug.csv" );
		//TdDebugLog.open(filename, ios::app);
		//TdDebugLog<<pTrade->InstrumentID
		//	<<','<<pTrade->TradeID
		//	<<','<<pTrade->OrderSysID
		//	<<','<<pTrade->Volume
		//	<<endl;
		//TdDebugLog.close();
	}

}

///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÏìÓ¦
void CTrader::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//char filename[64] = "";
		//sprintf(filename, "TdDebug.csv" );
		//TdDebugLog.open(filename, ios::app);
		//TdDebugLog<<pInvestorPosition->InstrumentID
		//	<<','<<pInvestorPosition->OpenVolume
		//	<<','<<pInvestorPosition->CloseVolume
		//	<<','<<pInvestorPosition->OpenAmount
		//	<<','<<pInvestorPosition->CloseAmount
		//	<<endl;
		//TdDebugLog.close();
	}

}

///ÇëÇó²éÑ¯Í¶×ÊÕß³Ö²ÖÃ÷Ï¸ÏìÓ¦
void CTrader::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//char filename[64] = "";
		//sprintf(filename, "TdDebug.csv" );
		//TdDebugLog.open(filename, ios::app);
		//TdDebugLog<<pInvestorPositionDetail->InstrumentID
		//	<<','<<pInvestorPositionDetail->OpenPrice
		//	<<','<<pInvestorPositionDetail->CombInstrumentID
		//	<<endl;
		//TdDebugLog.close();
	}

}

int CTrader::ReqQryInstrument(char *param)
{
    tradephase = E_TD_ReqQryInstrument;
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	//strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = -1;
	while (true)
	{
		iResult = pTraderUserApi->ReqQryInstrument(&req, ++requestid);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInstrument %s \n",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInstrument %d, FlowControl \n",iResult);
			sleep(1);
		}
	} // while
	return iResult;
}

void CTrader::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    tradephase = E_TD_OnRspQryInstrument;
	//if last Rsp, Sort and save
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
        tradephase = E_TD_OnRspQryInstrument_Done;
	}
	else
	{
		if((strstr(pInstrument->InstrumentID, "SP") == NULL)&&
			(strstr(pInstrument->InstrumentID, "efp") == NULL))
		{
		     instrumentfield.push_back(*pInstrument);
			 memcpy(pInstrumentID[iInstrumentID],pInstrument->InstrumentID,sizeof(pInstrument->InstrumentID));
			 iInstrumentID++;
		}
	}
}


int CTrader::ReqQryInstrumentMarginRate()
{
    isrsperror = false;
	CThostFtdcQryInstrumentMarginRateField req;
	memset(&req, 0, sizeof(req));
	//strcpy(req.BrokerID, traderinfo.brokerid);
	//strcpy(req.InvestorID, traderinfo.investorid);
	//strcpy(req.InstrumentID ,"a1205");
	//req.HedgeFlag = '1';

	int iResult = -1;
	while (true)
	{
		iResult = pTraderUserApi->ReqQryInstrumentMarginRate(&req, ++requestid);
		if (!IsFlowControl(iResult))
		{
			printf("ReqQryInstrumentMarginRate %s \n",((iResult == 0) ? "Successful" : "failed"));
			break;
		}
		else
		{
			printf("ReqQryInstrumentMarginRate %d, FlowControl \n",iResult);
			sleep(1);
		}
	} // while
	return WaitRsp(E_TD_OnRspQryInstrumentMarginRate);
}

void CTrader::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pInstrumentMarginRate !=NULL)
       cout<<pInstrumentMarginRate->InstrumentID<<":"<<pInstrumentMarginRate->ShortMarginRatioByMoney<<endl;
	if (bIsLast)
	{
	    if(isrsperror)
	    {

	    }
	    tradephase = E_TD_OnRspQryInstrumentMarginRate;
	}
}

int CTrader::ExtOrderinsert(char* orderstr, int price)
{
    if(!isstarted)
    {
        StartTrader();
    }
    CThostFtdcInputOrderField req;
    strcpy(req.InstrumentID,orderstr);
    req.LimitPrice = price;
    int rtn = ReqOrderInsert(req);
    StopTrader();
    return rtn;
}
