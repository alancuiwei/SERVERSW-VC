#include "TQM.h"
#include "MdSpi.h"
#include <string.h>
#include <stdio.h>
#include "../MyODBC/MySQLAPI.h"
#include "../COMM/COMM.h"
using namespace std;
// 请求编号
extern int iRequestID;
extern bool tqm_isloginok;
extern CLightThreadMutex g_ThreadMutex;
CMySQLAPI MDDatabase;
bool tqm_issavetodb;

void CMdSpi::GetMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (tqm_issavetodb)
	{
		char str[1024*4];
		sprintf(str,"insert markettodaydata_t \
				   values('%s','%s','%s',%10.2lf,%10.2lf,%10.2lf,%10.2lf,%10.2lf,%10ld,%10.2lf,%10.2lf,%10.2lf,%10.2lf)",
			pDepthMarketData->InstrumentID,
			// pDepthMarketData->TradingDay,
			tqm_startdate,
			pDepthMarketData->UpdateTime,
			pDepthMarketData->LastPrice,
			pDepthMarketData->OpenPrice,
			pDepthMarketData->HighestPrice,
			pDepthMarketData->LowestPrice,
			pDepthMarketData->ClosePrice,
			pDepthMarketData->Volume,
			pDepthMarketData->OpenInterest,
			pDepthMarketData->SettlementPrice,
			pDepthMarketData->AskPrice1,
			pDepthMarketData->BidPrice1
			);
		//MDDatabase.Open();
		MDDatabase.ExecuteNonQuery(str);
	}
	if(tqm_prtmarketdatamap[pDepthMarketData->InstrumentID] != NULL)
	{
		//tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->constractname= pDepthMarketData->InstrumentID;
		strcpy(tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->constractname, pDepthMarketData->InstrumentID);
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->bidprice1 = pDepthMarketData->BidPrice1;
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->askprice1 = pDepthMarketData->AskPrice1;
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->openprice = pDepthMarketData->OpenPrice;
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->highestprice = pDepthMarketData->HighestPrice;
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->lowestprice = pDepthMarketData->LowestPrice;
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->volume = pDepthMarketData->Volume;
		//tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->time = pDepthMarketData->UpdateTime;
		strcpy(tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->time, pDepthMarketData->UpdateTime);
		tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]->rtprice = pDepthMarketData->LastPrice;
		CCOMMServer::getinstance()->SendToAllClient((char *)tqm_prtmarketdatamap[pDepthMarketData->InstrumentID],
			sizeof(*tqm_prtmarketdatamap[pDepthMarketData->InstrumentID]));
	}
	//MDDatabase.Close();
}




void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
	int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{

	printf("%s",__FUNCTION__);
	printf("Reason = %d", nReason);
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{

	printf("%s",__FUNCTION__);
	printf("nTimerLapse = %d", nTimeLapse);
}

void CMdSpi::OnFrontConnected()
{
	printf("%s",__FUNCTION__);
	///用户登录请求
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pMdUserApi->ReqUserLogin(&req, ++iRequestID);
	printf("User login %s",((iResult == 0) ? "Successful" : "failed"));
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		printf("Current trading day = %s", pMdUserApi->GetTradingDay());
		if(!tqm_isloginok)
		{
			//tqm_isloginok = true;
			//WaitForSingleObject(g_hMdReadyEvent, INFINITE);
			g_ThreadMutex.TryLock(20000);
			CCOMMServer::getinstance();
			CCOMMServer::getinstance()->Start();
		}
		pMdUserSpi->SubscribeMarketData(pInstrumentID, iInstrumentID);
		//Sleep(5000);
	}
}

void CMdSpi::SubscribeMarketData(char *pInstrumentID[], int nCount)
{
	printf("%s",__FUNCTION__);

	if(!tqm_isloginok)
	{
		if(tqm_issavetodb)
		{
			MDDatabase.Init();
			MDDatabase.Open();
			std::string str(tqm_starttime);
			if(str<"08:30:00")
			{
				string sqlstr="";
				for(int k = 0; k < nCount; k++)
				{
					sqlstr=sqlstr+"('";
					sqlstr=sqlstr+pInstrumentID[k];
					sqlstr=sqlstr+"'),";
				}
				sqlstr=sqlstr.substr(0,sqlstr.length() - 1);
				sqlstr="insert into validcontracts_t values"+sqlstr+";";
				MDDatabase.ExecuteNonQuery("delete from validcontracts_t;");
				//MDDatabase.ExecuteNonQuery("delete from arbitragecontractpairs_t;");
				MDDatabase.ExecuteNonQuery(sqlstr.c_str());
				//MDDatabase.ExecuteNonQuery("CALL updatepairs_p();");
				//MDDatabase.ExecuteNonQuery("CALL updatecontractbeforemarket_p();");
			}
		}
		tqm_isloginok = true;
	}


	int iResult = pMdUserApi->SubscribeMarketData(pInstrumentID, nCount);


}
void CMdSpi::UnSubscribeMarketData(char *pInstrumentID[], int nCount)
{
	int iResult = pMdUserApi->UnSubscribeMarketData(pInstrumentID, nCount);
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("%s",__FUNCTION__);
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//printf("%s",__FUNCTION__);
	//printf("TradingDay:%s InstrumentID:%s LastPrice:%f UpdateTime: %s UpdateMillisec: %d\n",pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime,pDepthMarketData->UpdateMillisec);

	GetMarketData(pDepthMarketData);
	/*
	LocalLog.open(filename, ios::app);
	LocalLog<<pDepthMarketData->TradingDay          ///交易日
	<<","<<pDepthMarketData->UpdateTime        ///最后修改时间
	//<<","<<pDepthMarketData->SettlementPrice   ///本次结算价
	<<","<<pDepthMarketData->InstrumentID      ///合约代码
	//<<","<<pDepthMarketData->ExchangeID        ///交易所代码
	//<<","<<pDepthMarketData->ExchangeInstID    ///合约在交易所的代码
	<<","<<pDepthMarketData->LastPrice         ///最新价
	//<<","<<pDepthMarketData->PreSettlementPrice///上次结算价
	//<<","<<pDepthMarketData->PreClosePrice     ///昨收盘
	//<<","<<pDepthMarketData->PreOpenInterest   ///上次结算价
	<<","<<pDepthMarketData->OpenPrice         ///今开盘
	<<","<<pDepthMarketData->HighestPrice      ///最高价
	<<","<<pDepthMarketData->LowestPrice       ///最低价
	<<","<<pDepthMarketData->ClosePrice        ///今收盘
	<<","<<pDepthMarketData->Volume            ///数量
	<<","<<pDepthMarketData->Turnover          ///成交金额
	<<","<<pDepthMarketData->OpenInterest      ///持仓量

	<<","<<pDepthMarketData->UpperLimitPrice   ///涨停板价
	<<","<<pDepthMarketData->LowerLimitPrice   ///跌停板价
	<<","<<pDepthMarketData->PreDelta          ///昨虚实度
	<<","<<pDepthMarketData->CurrDelta         ///今虚实度

	<<","<<pDepthMarketData->UpdateMillisec    ///最后修改毫秒
	<<","<<pDepthMarketData->BidPrice1         ///申买价一
	<<","<<pDepthMarketData->BidVolume1        ///申买量一
	<<","<<pDepthMarketData->AskPrice1         ///申卖价一
	<<","<<pDepthMarketData->AskVolume1        ///申卖量一
	<<","<<pDepthMarketData->BidPrice2         ///申买价二
	<<","<<pDepthMarketData->BidVolume2        ///申买量二
	<<","<<pDepthMarketData->AskPrice2         ///申卖价二
	<<","<<pDepthMarketData->AskVolume2        ///申卖量二
	<<","<<pDepthMarketData->BidPrice3         ///申买价三
	<<","<<pDepthMarketData->BidVolume3        ///申买量三
	<<","<<pDepthMarketData->AskPrice3         ///申卖价三
	<<","<<pDepthMarketData->AskVolume3        ///申卖量三
	<<","<<pDepthMarketData->BidPrice4         ///申买价四
	<<","<<pDepthMarketData->BidVolume4        ///申买量四
	<<","<<pDepthMarketData->AskPrice4         ///申卖价四
	<<","<<pDepthMarketData->AskVolume4        ///申卖量四
	<<","<<pDepthMarketData->BidPrice5         ///申买价五
	<<","<<pDepthMarketData->BidVolume5        ///申买量五
	<<","<<pDepthMarketData->AskPrice5         ///申卖价五
	<<","<<pDepthMarketData->AskVolume5        ///申卖量五
	<<","<<pDepthMarketData->AveragePrice      //当日均价
	<<endl;
	LocalLog.close();
	*/
	//由于郑州交易所发送的行情数据没有tradingday的数值，所以所有记录中tradingday都从系统时间中读取。
	/*SYSTEMTIME sys;
	GetLocalTime( &sys );
	if(strstr(pDepthMarketData->TradingDay, "") != NULL)
	{
		sprintf(pDepthMarketData->TradingDay,"%4d%02d%02d", sys.wYear, sys.wMonth, sys.wDay);
	}

	///收集当日所有Active合约的信息存入Md_Day_Instrument.csv
	if((strcmp(pDepthMarketData->UpdateTime, "15:00:01") >=0) && (strcmp(pDepthMarketData->UpdateTime, "15:00:59")<=0))
	{
		LocalLog.open("..\\Debug\\Md_Day_Instrument.csv", ios::app);
		LocalLog<<pDepthMarketData->InstrumentID       ///合约代码
			<<","<<pDepthMarketData->TradingDay        ///交易日
			<<","<<pDepthMarketData->OpenPrice         ///今开盘
			<<","<<pDepthMarketData->HighestPrice      ///最高价
			<<","<<pDepthMarketData->LowestPrice       ///最低价
			<<","<<pDepthMarketData->ClosePrice        ///今收盘
			<<","<<pDepthMarketData->Volume            ///数量
			<<","<<pDepthMarketData->OpenInterest      ///持仓量
			<<","<<pDepthMarketData->BidPrice1         ///申买价一
			<<","<<pDepthMarketData->AskPrice1         ///申卖价一
			<<endl;
		LocalLog.close();
	}/*
	///收集每秒的数据存入Md_Second_Instrument.csv
	/*{
	LocalLog.open("..\\Debug\\Md_Second_Instrument.csv", ios::app);
	LocalLog<<pDepthMarketData->InstrumentID       ///合约代码
	<<","<<pDepthMarketData->TradingDay        ///交易日
	<<","<<pDepthMarketData->UpdateTime        ///最后修改时间
	<<","<<pDepthMarketData->LastPrice         ///最新价
	<<","<<pDepthMarketData->Volume            ///数量
	<<","<<pDepthMarketData->OpenInterest      ///持仓量
	<<endl;
	LocalLog.close();
	}*/
	///当时间超过15:01:00时不在保存秒钟数据，而是将文件的内容写入数据库
	/*if(sys.wMinute ==52 && sys.wHour ==13)
	//if(strcmp(pDepthMarketData->UpdateTime, "15:01:00") >=0)
	{
		if( MyConnectDB() == 0) ///登陆数据库成功
		{

			int nFile = 0, j = 0;
			char *instr_val = "";
			char *date_val  = "";
			for (nFile = 0; nFile<MD_FILES.size(); nFile++)
			{
				CMyExcel *pExcelMain = new CMyExcel();
				CString f = pExcelMain->GetAppPath()+"\\"+MD_FILES[nFile]+".csv";
				if ( pExcelMain->IsFileExist( f, FALSE ) ) ///若Md_Sec_$InstrumentID_$TradingDay.csv存在
				{
					pExcelMain->Open( f );
					pExcelMain->OpenSheet( MD_FILES[nFile] );
					pExcelMain->AutoRange();
					CString ins = pExcelMain->GetItemText(1, 1);
					CString dat = pExcelMain->GetItemText(1, 2);
					instr_val = ins.GetBuffer(dat.GetLength());
					date_val = dat.GetBuffer(dat.GetLength());
					for(j=1; !pExcelMain->GetItemText(j, 1).IsEmpty(); j++) ///第j行1列非空
					{
						///共12列数据
						CString record = " '" + pExcelMain->GetItemText(j, 1) + "',"
							+ "'" + pExcelMain->GetItemText(j, 2) + "',"
							+ "'" + pExcelMain->GetItemText(j, 3) + "',"
							+ pExcelMain->GetItemText(j, 4) + ","
							+ pExcelMain->GetItemText(j, 5) + ","
							+ pExcelMain->GetItemText(j, 6) + ","
							+ pExcelMain->GetItemText(j, 7) + ","
							+ pExcelMain->GetItemText(j, 8) + ","
							+ pExcelMain->GetItemText(j, 9) + ","
							+ pExcelMain->GetItemText(j, 10)+ ","
							+ pExcelMain->GetItemText(j, 11)+ ","
							+ pExcelMain->GetItemText(j, 12);
						char *val = record.GetBuffer(record.GetLength());
						///写入记录
						if(MyInsertRecord("md_sec_data", val) == 0)
						{
							cout<<"Insert one record successfully"<<endl;
						}
						else
						{
							cout<<"Insert one record failed"<<endl;
						}
						record.ReleaseBuffer();
					}
					//保存1分钟线数据
					char *min_val = "";
					for(int k = 0; k < 242; k++)
					{
						min_val = MinArray[k];
						SaveMinData(instr_val,date_val,min_val);
					}
					ins.ReleaseBuffer();
					dat.ReleaseBuffer();
					//得到5,15,30,60分钟线 保存为Md_$PeriodMin_$Instru_%Date.csv
					GetNMinData(instr_val,date_val,5);
					GetNMinData(instr_val,date_val,15);
					GetNMinData(instr_val,date_val,30);
					GetNMinData(instr_val,date_val,60);
				}
				pExcelMain->Exit();
				///删除已经写入数据库的Md_Sec_$InstrumentID_$TradingDay.csv文件
				DeleteFile((LPCTSTR)f);
			}
			///删除所有文件记录以免重复写入数据库
			MD_FILES.clear();
		}
		else
		{
			cout<<"Could not connect DB"<<endl;
		}
	}
	else
	{
		///收集每秒的数据存入Md_Sec_$InstrumentID_$TradingDay.csv
		char filename[64] = "";
		sprintf(filename, "..\\Debug\\Md_Sec_%s_%s.csv", pDepthMarketData->InstrumentID, pDepthMarketData->TradingDay);
		LocalLog.open(filename, ios::app);
		LocalLog<<pDepthMarketData->InstrumentID       ///合约代码
			<<","<<pDepthMarketData->TradingDay        ///交易日
			<<", "<<pDepthMarketData->UpdateTime       ///最后修改时间前面加了空格", "是为了让CMyExcel类能够正确读取
			<<","<<pDepthMarketData->LastPrice         ///最新价
			<<","<<pDepthMarketData->Volume            ///数量
			<<","<<pDepthMarketData->OpenInterest      ///持仓量
			<<","<<pDepthMarketData->OpenPrice         ///今开盘
			<<","<<pDepthMarketData->HighestPrice      ///最高价
			<<","<<pDepthMarketData->LowestPrice       ///最低价
			<<","<<pDepthMarketData->ClosePrice        ///今收盘
			<<","<<pDepthMarketData->BidPrice1         ///申买价一
			<<","<<pDepthMarketData->AskPrice1         ///申卖价一
			<<endl;
		LocalLog.close();
	}*/
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	printf("%s",__FUNCTION__);
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		printf("ErrorID= %d, ErrorMsg= %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}
