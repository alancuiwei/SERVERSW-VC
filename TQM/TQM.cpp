// TQM.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TQM.h"
using namespace std;


/*行情数据*/
std::map<std::string, CRTMarketData*> tqm_prtmarketdatamap;

/*是否loginok*/
bool tqm_isloginok = false;
// 请求编号
int iRequestID = 0;

/*Trader线程*/
HANDLE hTraderThread; 
DWORD TraderThreadId;
//DWORD WINAPI Login(LPVOID m  );
/*Market线程*/
HANDLE hMdThread; 
DWORD MdThreadId;
//DWORD WINAPI TraderThreadProcess(LPVOID  m );
// UserApi对象
CThostFtdcMdApi* pMdUserApi;
CMdSpi* pMdUserSpi;
CThostFtdcTraderApi* pTraderUserApi;
CTraderSpi* pTraderUserSpi;

/*Trader线程*/
HANDLE g_hMdReadyEvent=CreateEvent(NULL, FALSE, FALSE, NULL);
//HANDLE g_hSubcribeReadyEvent=CreateEvent(NULL, FALSE, FALSE, NULL);;

char *pInstrumentID[1024];	 // 行情订阅列表	
int iInstrumentID=0;	 // 行情订阅数量
char tqm_startdate[20]; //程序开始执行日期
char tqm_starttime[20]; //程序开始执行时间

void SetSubscribeMarketData( void );


/*使用测试数据库或者真实数据库*/
#if USETESTDATA
char MARKET_FRONT_ADDR[] = "tcp://asp-sim2-dx-md1.financial-trading-platform.com:26213";    // 前置地址
char  TRADE_FRONT_ADDR[] = "tcp://asp-sim2-dx-front1.financial-trading-platform.com:26205";	// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "352589";		// 投资者代码
TThostFtdcPasswordType  PASSWORD = "123321";			// 用户密码
#else
char MARKET_FRONT_ADDR[] = "tcp://gwf-md1.financial-trading-platform.com:41213";	// 前置地址
char  TRADE_FRONT_ADDR[] = "tcp://gwf-front1.financial-trading-platform.com:41205";	// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "8080";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "11800387";		// 投资者代码
TThostFtdcPasswordType  PASSWORD = "118403";			// 用户密码
#endif

//DWORD WINAPI   TraderThreadProcess( LPVOID  m)
void  TraderThreadProcess( LPVOID )
{
	//numofstr=0;
	pTraderUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("td");		// 创建UserApi
	pTraderUserSpi = new CTraderSpi();
	pTraderUserApi->RegisterSpi((CThostFtdcTraderSpi*)pTraderUserSpi);		// 注册事件类
	pTraderUserApi->SubscribePublicTopic(THOST_TERT_QUICK);					// 注册公有流
	pTraderUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// 注册私有流
	pTraderUserApi->RegisterFront(TRADE_FRONT_ADDR);						// connect
	pTraderUserApi->Init();
	LPMSG pMessage = (LPMSG)new char [sizeof(MSG)];
	Trader_MsgQData_t* pTmp = NULL;
	//pTraderUserApi->Join();
	
	while(true)
	{

		//收msg
		if(GetMessage(pMessage, NULL, 0, 0))
		{
			if(WM_MSG_TRADER == pMessage->message)
			{
				pTmp = (Trader_MsgQData_t*)pMessage->wParam;
				switch(pTmp->MessageId)
				{
				case Trader_InputOrder_Req:
					//使用SPI类相应的处理函数来处理
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

					//其他case
				default:
					break;
				}
			}/**/
		}
	}
	delete pTraderUserSpi;
	//return 0;
}


void MdThreadProcess( LPVOID )
{
	// 初始化UserApi
	pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi("md");		// 创建UserApi
	pMdUserSpi = new CMdSpi();

	pMdUserApi->RegisterSpi(pMdUserSpi);						// 注册事件类
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
	_beginthread(TraderThreadProcess, 0, 0);
}

void StartMdThreadProcess(  )
{
	//CTime time = CTime::GetCurrentTime();   ///构造CTime对象
	//gtodaydate = time.Format("%Y-%m-%d"); 
	//SetSubscribeMarketData( );
    //hMdThread=CreateThread(NULL, 0, Login, 0, 0, &MdThreadId);
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	sprintf(tqm_startdate,"%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay);
	sprintf(tqm_starttime,"%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
	_beginthread(MdThreadProcess, 0, 0);

}


// This is an example of an exported variable
TQM_API int nTQM=0;

// This is an example of an exported function.
TQM_API int fnTQM(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see TQM.h for the class definition
CTQM::CTQM()
{
	return;
}


