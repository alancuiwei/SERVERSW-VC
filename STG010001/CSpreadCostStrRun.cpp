#include "stdafx.h"
#include "CSpreadCostStrRun.h"
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
//#include <process.h>
#include <iostream>
#include "../SSM/SSM.h"
#include "../COMM/COMM.h"
//HANDLE g_hEvent;

CSpreadCostStrRun::CSpreadCostStrRun(std::string strategyid):
CStrategyRun(strategyid)
{

}


CSpreadCostStrRun::~CSpreadCostStrRun()
{
  for(unsigned i=0; i<runtradeparameters.size();i++)
  {
      delete runtradeparameters[i];
  }
}


void CSpreadCostStrRun::Run(void)
{
  ofstream xmlfs;
  ostringstream xmlss;
  bool ishavedata=false;
  xmlss<<"<pairstrade>"<< endl;
  //xmldata<<"<pairstrade>"<< endl;
  //objectdata.open("..//Debug//SpreadCost//CSpreadCostStrRun.txt", ios::trunc);
  for(unsigned i=0; i<runtradeparameters.size();i++)
  {
      runtradeparameters[i]->startprice = runpairs[i]->computeannualrateofreturn( );
	  cout<<runpairs[i]->firstcontract->contractname<<":"<<runtradeparameters[i]->startprice<<endl;
	  // 保存到xml文件中
      if ((strlen(runpairs[i]->firstcontract->marketdata->time)==8)
          &&(strlen(runpairs[i]->secondcontract->marketdata->time)==8))
      {
          ishavedata=true;
          std::string time1(runpairs[i]->firstcontract->marketdata->time);
          std::string time2(runpairs[i]->secondcontract->marketdata->time);
          std::string timestr=((time1>time2)?time1:time2);
          xmlss<<"<pair>"<< endl;
          xmlss<<"<name>"<<runpairs[i]->firstcontract->contractname <<"-"
              <<runpairs[i]->secondcontract->contractname<<"</name>" <<endl;
          xmlss<<"<time>"<<timestr<<"</time>"<<endl;
          xmlss<<"<firstprice>"<<runpairs[i]->firstcontract->marketdata->rtprice<<"</firstprice>"<<endl;
          xmlss<<"<secondprice>"<<runpairs[i]->secondcontract->marketdata->rtprice<<"</secondprice>"<<endl;
          xmlss<<"<diffprice>"<<runpairs[i]->secondcontract->marketdata->rtprice-runpairs[i]->firstcontract->marketdata->rtprice<<"</diffprice>"<<endl;
          xmlss<<"<startprice>"<<runtradeparameters[i]->startprice<<"</startprice>"<<endl;
          xmlss<<"<computestoragefee>"<<runpairs[i]->computestoragefee()<<"</computestoragefee>"<<endl;
          xmlss<<"<computestoragedays>"<<runpairs[i]->computestoragedays()<<"</computestoragedays>"<<endl;
          xmlss<<"<computevatrate>"<<runpairs[i]->computevatrate()<<"</computevatrate>"<<endl;
          xmlss<<"<computefirstconstractmargin>"<<runpairs[i]->computefirstconstractmargin()<<"</computefirstconstractmargin>"<<endl;
          xmlss<<"<computesecondconstractmargin>"<<runpairs[i]->computesecondconstractmargin()<<"</computesecondconstractmargin>"<<endl;
          xmlss<<"<computetransfee>"<<runpairs[i]->computetransfee()<<"</computetransfee>"<<endl;
          xmlss<<"<computedeliverfee>"<<runpairs[i]->computedeliverfee()<<"</computedeliverfee>"<<endl;
          xmlss<<"<computedelivermarginfee>"<<runpairs[i]->computedelivermarginfee()<<"</computedelivermarginfee>"<<endl;
          xmlss<<"<computetrademarginfee>"<<runpairs[i]->computetrademarginfee()<<"</computetrademarginfee>"<<endl;
          xmlss<<"<computevatfee>"<<runpairs[i]->computevatfee()<<"</computevatfee>"<<endl;
          xmlss<<"<computearbfee>"<<runpairs[i]->arbfee<<"</computearbfee>"<<endl;
          //runpairs[i]->computerateofreturn();
          xmlss<<"<D2>"<<runpairs[i]->secondcontract->daystolastdeliverdate<<"</D2>"<<endl;
          xmlss<<"<D1>"<<runpairs[i]->firstcontract->daystolastdeliverdate<<"</D1>"<<endl;
          xmlss<<"<computetotalincome>"<<runpairs[i]->computetotalincome()<<"</computetotalincome>"<<endl;
          //xmlss<<"<computerateofreturn>"<<runpairs[i]->computerateofreturn()<<"</computerateofreturn>"<<endl;
          xmlss<<"<computerateofreturn>"<<runtradeparameters[i]->startprice<<"</computerateofreturn>"<<endl;
          xmlss<<"<lendrate>"<<runpairs[i]->commodity->lendrate<<"</lendrate>"<<endl;
          xmlss<<"<computeinvestmoneyamount>"<<runpairs[i]->computeinvestmoneyamount()<<"</computeinvestmoneyamount>"<<endl;
          xmlss<<"</pair>"<<endl;
          if(runpairs[i]->maxreturnrate<runtradeparameters[i]->startprice)
          {
              runpairs[i]->setmaxreturnrate(runtradeparameters[i]->startprice,timestr);
              runpairs[i]->maxreturnrate=runtradeparameters[i]->startprice;
          }
      }
  }
  xmlss<<"</pairstrade>"<< endl;
  //char xmlfilename[128] = "CSpreadCostStrRunOnTime.xml";//".//SpreadCost//CSpreadCostStrRunOnTime.xml";
  if (ishavedata)
  {
      char xmlfilename[128] =  "../tongtianshun/app/assets/images/CSpreadCostStrRunOnTime.xml";
      xmlfs.open(xmlfilename, ios::trunc);
      xmlfs<<xmlss.str();
      xmlfs.close();
  }
}

void CSpreadCostStrRun::Start()
{
	//g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    //hThread =(HANDLE)_beginthread(Agent, 0, (void *)this);
	//CloseHandle(hThread);
	CLightThread::CreateThread(Agent, (void *)this);

}

void* CSpreadCostStrRun::Agent(void *p)
{
    CSpreadCostStrRun *agt = (CSpreadCostStrRun *)p;
	// 每秒钟执行一次
	//while(WaitForSingleObject(g_hEvent,1000)!=WAIT_OBJECT_0)
	while(true)
	{
        agt->Run();
        sleep(1);
    }
}

HANDLE CSpreadCostStrRun::GetThread()
{
    return hThread;
}

void CSpreadCostStrRun::AddPair(CSpreadCostPair * pair)
{
	runpairs.push_back(pair);
	CTradeStru * ptrade = new CTradeStru();
	runtradeparameters.push_back(ptrade);
}

void CSpreadCostStrRun::SavePairsXML( char *)
{
  ofstream xmlfs;
  ostringstream xmlss;
  xmlss<<"<pairsattr>"<< endl;
  for(unsigned i=0; i<runtradeparameters.size();i++)
  {
	  xmlss<<"<pair>"<< endl;
	  xmlss<<"<name>"<<runpairs[i]->firstcontract->contractname <<"-"
		  <<runpairs[i]->secondcontract->contractname<<"</name>" <<endl;
	  //xmlss<<"<firstprice>"<<runpairs[i]->firstcontract->marketdata->rtprice<<"</firstprice>"<<endl;
	  xmlss<<"<firstmarginrate>"<<runpairs[i]->firstcontractmargin-runpairs[i]->commodity->trademargingap<<"</firstmarginrate>"<<endl;
	  xmlss<<"<secondmarginrate>"<<runpairs[i]->secondcontractmargin-runpairs[i]->commodity->trademargingap<<"</secondmarginrate>"<<endl;
	  xmlss<<"<storagedailyfee>"<<runpairs[i]->commodity->storagedailyfee<<"</storagedailyfee>"<<endl;
	  xmlss<<"<issinglemargin>"<<runpairs[i]->commodity->issinglemargin<<"</issinglemargin>"<<endl;
	  xmlss<<"<deliverchargebyhand>"<<runpairs[i]->commodity->deliverchargebyhand<<"</deliverchargebyhand>"<<endl;
	  xmlss<<"<tradeunit>"<<runpairs[i]->commodity->tradeunit<<"</tradeunit>"<<endl;
	  xmlss<<"<exchtrademargin>"<<runpairs[i]->commodity->exchtrademargin<<"</exchtrademargin>"<<endl;
	  xmlss<<"<vatrate>"<<runpairs[i]->vatrate<<"</vatrate>"<<endl;
	  xmlss<<"<D1>"<<runpairs[i]->firstcontract->daystolastdeliverdate<<"</D1>"<<endl;
	  xmlss<<"<D2>"<<runpairs[i]->secondcontract->daystolastdeliverdate<<"</D2>"<<endl;

	  xmlss<<"</pair>"<<endl;
  }
  xmlss<<"</pairsattr>"<< endl;
  //char xmlfilename[128] = "CSpreadCostStrRunOnTime.xml";//".//SpreadCost//CSpreadCostStrRunOnTime.xml";
  char xmlfilename[128] =  "../tongtianshun/app/assets/images/CSpreadCostPairs.xml";
  xmlfs.open(xmlfilename, ios::trunc);
  xmlfs<<xmlss.str();
  xmlfs.close();
}

