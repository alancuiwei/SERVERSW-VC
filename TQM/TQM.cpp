#include "TQM.h"
#include "../COMM/COMM.h"
using namespace std;


/*��������*/
std::map<std::string, CRTMarketData*> tqm_prtmarketdatamap;

/*�Ƿ�loginok*/
bool tqm_isloginok = false;
// ������
int iRequestID = 0;

/*Trader�߳�*/
//HANDLE hTraderThread;
//DWORD TraderThreadId;
CLightThread g_TraderThread;
//DWORD WINAPI Login(LPVOID m  );
/*Market�߳�*/
//HANDLE hMdThread;
//DWORD MdThreadId;
CLightThread g_MdThread;
//DWORD WINAPI TraderThreadProcess(LPVOID  m );
// UserApi����
CThostFtdcMdApi* pMdUserApi;
CMdSpi* pMdUserSpi;
CThostFtdcTraderApi* pTraderUserApi;
CTraderSpi* pTraderUserSpi;

/*Trader�߳�*/
//HANDLE g_hMdReadyEvent=CreateEvent(NULL, FALSE, FALSE, NULL);
CLightThreadMutex g_ThreadMutex;
//HANDLE g_hSubcribeReadyEvent=CreateEvent(NULL, FALSE, FALSE, NULL);;

char *pInstrumentID[1024];	 // ���鶩���б�
int iInstrumentID=0;	 // ���鶩������
char tqm_startdate[20]; //����ʼִ������
char tqm_starttime[20]; //����ʼִ��ʱ��

void SetSubscribeMarketData( void );


/*ʹ�ò������ݿ������ʵ���ݿ�*/
#if USETESTDATA
char MARKET_FRONT_ADDR[] = "tcp://asp-sim2-dx-md1.financial-trading-platform.com:26213";    // ǰ�õ�ַ
char  TRADE_FRONT_ADDR[] = "tcp://asp-sim2-dx-front1.financial-trading-platform.com:26205";	// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "352589";		// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "123321";			// �û�����
#else
char MARKET_FRONT_ADDR[] = "tcp://gwf-md1.financial-trading-platform.com:41213";	// ǰ�õ�ַ
char  TRADE_FRONT_ADDR[] = "tcp://gwf-front1.financial-trading-platform.com:41205";	// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "8080";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "11800387";		// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "118403";			// �û�����
#endif

//DWORD WINAPI   TraderThreadProcess( LPVOID  m)
void*  TraderThreadProcess( void * )
{
	//numofstr=0;
	g_ThreadMutex.Lock();
	pTraderUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("td");		// ����UserApi
	pTraderUserSpi = new CTraderSpi();
	pTraderUserApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderUserSpi);		// ע���¼���
	pTraderUserApi->SubscribePublicTopic(THOST_TERT_QUICK);					// ע�ṫ����
	pTraderUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// ע��˽����
	pTraderUserApi->RegisterFront(TRADE_FRONT_ADDR);						// connect
	pTraderUserApi->Init();
	//LPMSG pMessage = (LPMSG)new char [sizeof(MSG)];
	//Trader_MsgQData_t* pTmp = NULL;
	pTraderUserApi->Join();

	/*while(true)
	{

		//��msg
		if(GetMessage(pMessage, NULL, 0, 0))
		{
			if(WM_MSG_TRADER == pMessage->message)
			{
				pTmp = (Trader_MsgQData_t*)pMessage->wParam;
				switch(pTmp->MessageId)
				{
				case Trader_InputOrder_Req:
					//ʹ��SPI����Ӧ�Ĵ�����������
					pTraderUserSpi->ReqOrderInsert((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					break;
				case Trader_ReqQryOrder_Req:
					pTraderUserSpi->ReqQryOrder((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					break;
				case Trader_ReqQryTrade_Req:
					pTraderUserSpi->ReqQryTrade((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					break;
				case Trader_ReqQryInvestorPosition_Req:
					pTraderUserSpi->ReqQryInvestorPosition((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					break;
				case Trader_ReqQryInvestorPositionCombineDetail_Req:
					pTraderUserSpi->ReqQryInvestorPositionDetail((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					break;
				case Trader_ReqQryInstrument_Req:
					pTraderUserSpi->ReqQryInstrument((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					return;
					break;
				case Trader_ReqQryTradingAccount_Req:
					pTraderUserSpi->ReqQryTradingAccount((char*)&pTmp->MsgParameter.Parameters);
					delete [] pTmp;
					break;

					//����case
				default:
					break;
				}
			}
		}
	}*/
	delete pTraderUserSpi;
	//return 0;
}


void* MdThreadProcess( void *)
{
	// ��ʼ��UserApi
	pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi("md");		// ����UserApi
	pMdUserSpi = new CMdSpi();

	pMdUserApi->RegisterSpi(pMdUserSpi);						// ע���¼���
	pMdUserApi->RegisterFront(MARKET_FRONT_ADDR);			    // connect
	pMdUserApi->Init();
	pMdUserApi->Join();
	delete pMdUserSpi;

}

void SetSubscribeMarketData( )
{
	 iInstrumentID = 0;
     std::map<std::string, CRTMarketData*>::iterator iter;
	 for(iter=tqm_prtmarketdatamap.begin();
		 iter!=tqm_prtmarketdatamap.end();
		 ++iter)
	 {
	     pInstrumentID[iInstrumentID]= new char [10];
	     memcpy(pInstrumentID[iInstrumentID],iter->first.c_str(),10);
		 iInstrumentID++;
	 }
}


void StartTraderThreadProcess(  )
{
	//_beginthread(TraderThreadProcess, 0, 0);
	// g_TraderThread.CreateThread(TraderThreadProcess, NULL);
	CLightThread::CreateThread(TraderThreadProcess, NULL);
}

void StartMdThreadProcess(  )
{
	//CTime time = CTime::GetCurrentTime();   ///����CTime����
	//gtodaydate = time.Format("%Y-%m-%d");
	//SetSubscribeMarketData( );
    //hMdThread=CreateThread(NULL, 0, Login, 0, 0, &MdThreadId);
	//SYSTEMTIME sys;
	//GetLocalTime( &sys );
	//sprintf(tqm_startdate,"%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay);
	//sprintf(tqm_starttime,"%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);

    struct tm *nowtm;
	time_t nowtime;
    struct tm exittm;
	time_t exittime;
	double secnum;

	nowtime=time(NULL);
	nowtm=localtime(&nowtime);
	sprintf(tqm_startdate,"%4d-%02d-%02d", 1900+nowtm->tm_year, 1+nowtm->tm_mon, nowtm->tm_mday);
	sprintf(tqm_starttime,"%02d:%02d:%02d", nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec);

	//_beginthread(MdThreadProcess, 0, 0);
	//g_MdThread.CreateThread(MdThreadProcess, NULL);
	CLightThread::CreateThread(MdThreadProcess, NULL);

}


// This is an example of an exported variable
int nTQM=0;

// This is an example of an exported function.
int fnTQM(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see TQM.h for the class definition
CTQM::CTQM()
{
	return;
}


