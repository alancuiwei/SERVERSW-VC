#include "TQM.h"
#include "MdSpi.h"
#include <string.h>
#include <stdio.h>
#include "../MyODBC/MySQLAPI.h"
#include "../COMM/COMM.h"
using namespace std;
// ������
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
	///�û���¼����
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
		///��ȡ��ǰ������
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
	LocalLog<<pDepthMarketData->TradingDay          ///������
	<<","<<pDepthMarketData->UpdateTime        ///����޸�ʱ��
	//<<","<<pDepthMarketData->SettlementPrice   ///���ν����
	<<","<<pDepthMarketData->InstrumentID      ///��Լ����
	//<<","<<pDepthMarketData->ExchangeID        ///����������
	//<<","<<pDepthMarketData->ExchangeInstID    ///��Լ�ڽ������Ĵ���
	<<","<<pDepthMarketData->LastPrice         ///���¼�
	//<<","<<pDepthMarketData->PreSettlementPrice///�ϴν����
	//<<","<<pDepthMarketData->PreClosePrice     ///������
	//<<","<<pDepthMarketData->PreOpenInterest   ///�ϴν����
	<<","<<pDepthMarketData->OpenPrice         ///����
	<<","<<pDepthMarketData->HighestPrice      ///��߼�
	<<","<<pDepthMarketData->LowestPrice       ///��ͼ�
	<<","<<pDepthMarketData->ClosePrice        ///������
	<<","<<pDepthMarketData->Volume            ///����
	<<","<<pDepthMarketData->Turnover          ///�ɽ����
	<<","<<pDepthMarketData->OpenInterest      ///�ֲ���

	<<","<<pDepthMarketData->UpperLimitPrice   ///��ͣ���
	<<","<<pDepthMarketData->LowerLimitPrice   ///��ͣ���
	<<","<<pDepthMarketData->PreDelta          ///����ʵ��
	<<","<<pDepthMarketData->CurrDelta         ///����ʵ��

	<<","<<pDepthMarketData->UpdateMillisec    ///����޸ĺ���
	<<","<<pDepthMarketData->BidPrice1         ///�����һ
	<<","<<pDepthMarketData->BidVolume1        ///������һ
	<<","<<pDepthMarketData->AskPrice1         ///������һ
	<<","<<pDepthMarketData->AskVolume1        ///������һ
	<<","<<pDepthMarketData->BidPrice2         ///����۶�
	<<","<<pDepthMarketData->BidVolume2        ///��������
	<<","<<pDepthMarketData->AskPrice2         ///�����۶�
	<<","<<pDepthMarketData->AskVolume2        ///��������
	<<","<<pDepthMarketData->BidPrice3         ///�������
	<<","<<pDepthMarketData->BidVolume3        ///��������
	<<","<<pDepthMarketData->AskPrice3         ///��������
	<<","<<pDepthMarketData->AskVolume3        ///��������
	<<","<<pDepthMarketData->BidPrice4         ///�������
	<<","<<pDepthMarketData->BidVolume4        ///��������
	<<","<<pDepthMarketData->AskPrice4         ///��������
	<<","<<pDepthMarketData->AskVolume4        ///��������
	<<","<<pDepthMarketData->BidPrice5         ///�������
	<<","<<pDepthMarketData->BidVolume5        ///��������
	<<","<<pDepthMarketData->AskPrice5         ///��������
	<<","<<pDepthMarketData->AskVolume5        ///��������
	<<","<<pDepthMarketData->AveragePrice      //���վ���
	<<endl;
	LocalLog.close();
	*/
	//����֣�ݽ��������͵���������û��tradingday����ֵ���������м�¼��tradingday����ϵͳʱ���ж�ȡ��
	/*SYSTEMTIME sys;
	GetLocalTime( &sys );
	if(strstr(pDepthMarketData->TradingDay, "") != NULL)
	{
		sprintf(pDepthMarketData->TradingDay,"%4d%02d%02d", sys.wYear, sys.wMonth, sys.wDay);
	}

	///�ռ���������Active��Լ����Ϣ����Md_Day_Instrument.csv
	if((strcmp(pDepthMarketData->UpdateTime, "15:00:01") >=0) && (strcmp(pDepthMarketData->UpdateTime, "15:00:59")<=0))
	{
		LocalLog.open("..\\Debug\\Md_Day_Instrument.csv", ios::app);
		LocalLog<<pDepthMarketData->InstrumentID       ///��Լ����
			<<","<<pDepthMarketData->TradingDay        ///������
			<<","<<pDepthMarketData->OpenPrice         ///����
			<<","<<pDepthMarketData->HighestPrice      ///��߼�
			<<","<<pDepthMarketData->LowestPrice       ///��ͼ�
			<<","<<pDepthMarketData->ClosePrice        ///������
			<<","<<pDepthMarketData->Volume            ///����
			<<","<<pDepthMarketData->OpenInterest      ///�ֲ���
			<<","<<pDepthMarketData->BidPrice1         ///�����һ
			<<","<<pDepthMarketData->AskPrice1         ///������һ
			<<endl;
		LocalLog.close();
	}/*
	///�ռ�ÿ������ݴ���Md_Second_Instrument.csv
	/*{
	LocalLog.open("..\\Debug\\Md_Second_Instrument.csv", ios::app);
	LocalLog<<pDepthMarketData->InstrumentID       ///��Լ����
	<<","<<pDepthMarketData->TradingDay        ///������
	<<","<<pDepthMarketData->UpdateTime        ///����޸�ʱ��
	<<","<<pDepthMarketData->LastPrice         ///���¼�
	<<","<<pDepthMarketData->Volume            ///����
	<<","<<pDepthMarketData->OpenInterest      ///�ֲ���
	<<endl;
	LocalLog.close();
	}*/
	///��ʱ�䳬��15:01:00ʱ���ڱ����������ݣ����ǽ��ļ�������д�����ݿ�
	/*if(sys.wMinute ==52 && sys.wHour ==13)
	//if(strcmp(pDepthMarketData->UpdateTime, "15:01:00") >=0)
	{
		if( MyConnectDB() == 0) ///��½���ݿ�ɹ�
		{

			int nFile = 0, j = 0;
			char *instr_val = "";
			char *date_val  = "";
			for (nFile = 0; nFile<MD_FILES.size(); nFile++)
			{
				CMyExcel *pExcelMain = new CMyExcel();
				CString f = pExcelMain->GetAppPath()+"\\"+MD_FILES[nFile]+".csv";
				if ( pExcelMain->IsFileExist( f, FALSE ) ) ///��Md_Sec_$InstrumentID_$TradingDay.csv����
				{
					pExcelMain->Open( f );
					pExcelMain->OpenSheet( MD_FILES[nFile] );
					pExcelMain->AutoRange();
					CString ins = pExcelMain->GetItemText(1, 1);
					CString dat = pExcelMain->GetItemText(1, 2);
					instr_val = ins.GetBuffer(dat.GetLength());
					date_val = dat.GetBuffer(dat.GetLength());
					for(j=1; !pExcelMain->GetItemText(j, 1).IsEmpty(); j++) ///��j��1�зǿ�
					{
						///��12������
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
						///д���¼
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
					//����1����������
					char *min_val = "";
					for(int k = 0; k < 242; k++)
					{
						min_val = MinArray[k];
						SaveMinData(instr_val,date_val,min_val);
					}
					ins.ReleaseBuffer();
					dat.ReleaseBuffer();
					//�õ�5,15,30,60������ ����ΪMd_$PeriodMin_$Instru_%Date.csv
					GetNMinData(instr_val,date_val,5);
					GetNMinData(instr_val,date_val,15);
					GetNMinData(instr_val,date_val,30);
					GetNMinData(instr_val,date_val,60);
				}
				pExcelMain->Exit();
				///ɾ���Ѿ�д�����ݿ��Md_Sec_$InstrumentID_$TradingDay.csv�ļ�
				DeleteFile((LPCTSTR)f);
			}
			///ɾ�������ļ���¼�����ظ�д�����ݿ�
			MD_FILES.clear();
		}
		else
		{
			cout<<"Could not connect DB"<<endl;
		}
	}
	else
	{
		///�ռ�ÿ������ݴ���Md_Sec_$InstrumentID_$TradingDay.csv
		char filename[64] = "";
		sprintf(filename, "..\\Debug\\Md_Sec_%s_%s.csv", pDepthMarketData->InstrumentID, pDepthMarketData->TradingDay);
		LocalLog.open(filename, ios::app);
		LocalLog<<pDepthMarketData->InstrumentID       ///��Լ����
			<<","<<pDepthMarketData->TradingDay        ///������
			<<", "<<pDepthMarketData->UpdateTime       ///����޸�ʱ��ǰ����˿ո�", "��Ϊ����CMyExcel���ܹ���ȷ��ȡ
			<<","<<pDepthMarketData->LastPrice         ///���¼�
			<<","<<pDepthMarketData->Volume            ///����
			<<","<<pDepthMarketData->OpenInterest      ///�ֲ���
			<<","<<pDepthMarketData->OpenPrice         ///����
			<<","<<pDepthMarketData->HighestPrice      ///��߼�
			<<","<<pDepthMarketData->LowestPrice       ///��ͼ�
			<<","<<pDepthMarketData->ClosePrice        ///������
			<<","<<pDepthMarketData->BidPrice1         ///�����һ
			<<","<<pDepthMarketData->AskPrice1         ///������һ
			<<endl;
		LocalLog.close();
	}*/
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	printf("%s",__FUNCTION__);
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		printf("ErrorID= %d, ErrorMsg= %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	return bResult;
}
