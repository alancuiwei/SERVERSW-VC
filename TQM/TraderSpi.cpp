#include "TQM.h"
#include "TraderSpi.h"
#include "../COMM/COMM.h"
using namespace std;
extern CLightThreadMutex g_ThreadMutex;
//ȫ���ļ���
ofstream TdDebugLog;
//temp
char *strtemp[600];
int numofstr=0;
/*
// USER_API����
extern CThostFtdcTraderApi* pTraderUserApi;

// ���ò���
extern char TRADE_FRONT_ADDR[];	// ǰ�õ�ַ
extern char BROKER_ID[];		// ���͹�˾����
extern char INVESTOR_ID[];		// Ͷ���ߴ���
extern char PASSWORD[];			// �û�����
//extern char INSTRUMENT_ID[];	// ��Լ����
extern TThostFtdcInstrumentIDType INSTRUMENT_ID;	// ��Լ����
extern TThostFtdcPriceType	LIMIT_PRICE;	// �۸�
extern TThostFtdcDirectionType	DIRECTION;	// ��������
*/
// ������
extern int iRequestID;

bool isTdOnRspOrderInsert;

//tydef strct
//{
//DCE ������Ʒ������
std::vector<std::string>  DCE_A; ///�ƴ�1��
std::vector<std::string>  DCE_B; ///�ƴ�2��
std::vector<std::string>  DCE_M; ///����
std::vector<std::string>  DCE_Y; ///����
std::vector<std::string>  DCE_C; ///����
std::vector<std::string>  DCE_L; ///LLDPE
std::vector<std::string>  DCE_P; ///�����
std::vector<std::string>  DCE_V; ///PVC
std::vector<std::string>  DCE_J; ///��̿
//CZCE ֣����Ʒ������
std::vector<std::string>  CZCE_SR; ///����
std::vector<std::string>  CZCE_TA; ///PTA
std::vector<std::string>  CZCE_CF; ///�޻�
std::vector<std::string>  CZCE_WS; ///ǿ��
std::vector<std::string>  CZCE_WT; ///Ӳ��
std::vector<std::string>  CZCE_ER; ///�̵�
std::vector<std::string>  CZCE_RO; ///����
std::vector<std::string>  CZCE_SPD; ///SPD
//SHFE �Ϻ���Ʒ������
std::vector<std::string>  SHFE_CU; ///ͭ
std::vector<std::string>  SHFE_AL; ///��
std::vector<std::string>  SHFE_ZN; ///п
std::vector<std::string>  SHFE_RU; ///�콺
std::vector<std::string>  SHFE_FU; ///ȼ��
std::vector<std::string>  SHFE_AU; ///�ƽ�
std::vector<std::string>  SHFE_RB; ///���Ƹ�
std::vector<std::string>  SHFE_WR; ///�߲�
std::vector<std::string>  SHFE_PB; ///Ǧ
//CFFE �й������ڻ�������
std::vector<std::string>  CFFE_IF; ///��ָ�ڻ�
//} CurrentActiveInstrument;
//CurrentActiveInstrument ActInstru;

// �Ự����
TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
TThostFtdcOrderRefType	ORDER_REF;	//��������

// �����ж�
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	printf("%s",__FUNCTION__);
	///�û���¼����
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
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///��ȡ��ǰ������
		printf("Current trading day = %s", pTraderUserApi->GetTradingDay());
		///Ͷ���߽�����ȷ��
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
		///�����ѯ��Լ
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
		///����¼������
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
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, tmp->InstrumentID);
	///��������
	strcpy(req.OrderRef, ORDER_REF);
	///�û�����
	//TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������:
	req.Direction = tmp->Direction;
	///��Ͽ�ƽ��־: ����
	req.CombOffsetFlag[0] = tmp->CombOffsetFlag[0];
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = tmp->LimitPrice;
	///����: 1
	req.VolumeTotalOriginal = tmp->VolumeTotalOriginal;
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 1;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	req.IsAutoSuspend = 0;
	///ҵ��Ԫ
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
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
	static bool ORDER_ACTION_SENT = false;		//�Ƿ����˱���
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, pOrder->BrokerID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, pOrder->InvestorID);
	///������������
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	strcpy(req.OrderRef, pOrder->OrderRef);
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	req.FrontID = FRONT_ID;
	///�Ự���
	req.SessionID = SESSION_ID;
	///����������
	//	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///�۸�
	//	TThostFtdcPriceType	LimitPrice;
	///�����仯
	//	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///��Լ����
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

///����֪ͨ
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

///�ɽ�֪ͨ
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
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
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


///�����ѯ����
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
///�����ѯ�ɽ�
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
///�����ѯͶ���ֲ߳�
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
///�����ѯͶ���ֲ߳���ϸ
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

///�����ѯ������Ӧ
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

///�����ѯ�ɽ���Ӧ
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

///�����ѯͶ���ֲ߳���Ӧ
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

///�����ѯͶ���ֲ߳���ϸ��Ӧ
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
