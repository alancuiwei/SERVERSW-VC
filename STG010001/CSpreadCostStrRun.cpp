#include "stdafx.h"
#include "CSpreadCostStrRun.h"
#include <fstream>
#include <sstream>
#include <string>
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
  ofstream objectdata;
  ofstream xmlfs;
  ostringstream xmlss;
  xmlss<<"<pairstrade>"<< endl;
  //xmldata<<"<pairstrade>"<< endl;
  //objectdata.open("..//Debug//SpreadCost//CSpreadCostStrRun.txt", ios::trunc);
  for(unsigned i=0; i<runtradeparameters.size();i++)
  {
      runtradeparameters[i]->startprice = runpairs[i]->computeannualrateofreturn( );
	  cout<<runpairs[i]->firstcontract->contractname<<":"<<runtradeparameters[i]->startprice<<endl;
	  // ���浽xml�ļ���
	  xmlss<<"<pair>"<< endl;
	  xmlss<<"<name>"<<runpairs[i]->firstcontract->contractname <<"-"
		  <<runpairs[i]->secondcontract->contractname<<"</name>" <<endl;
      std::string time1(runpairs[i]->firstcontract->marketdata->time);
      std::string time2(runpairs[i]->secondcontract->marketdata->time);
	  xmlss<<"<time>"<<((time1>time2)?time1:time2)<<" "<<"</time>"<<endl;
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
	  xmlss<<"<computearbfee>"<<runpairs[i]->computearbfee()<<"</computearbfee>"<<endl;
	  xmlss<<"<D2>"<<runpairs[i]->secondcontract->daystolastdeliverdate<<"</D2>"<<endl;
	  xmlss<<"<D1>"<<runpairs[i]->firstcontract->daystolastdeliverdate<<"</D1>"<<endl;
	  xmlss<<"<computetotalincome>"<<runpairs[i]->computetotalincome()<<"</computetotalincome>"<<endl;
	  xmlss<<"<computerateofreturn>"<<runpairs[i]->computerateofreturn()<<"</computerateofreturn>"<<endl;
	  xmlss<<"<lendrate>"<<runpairs[i]->commodity->lendrate<<"</lendrate>"<<endl;
	  xmlss<<"<computeinvestmoneyamount>"<<runpairs[i]->computeinvestmoneyamount()<<"</computeinvestmoneyamount>"<<endl;
	  xmlss<<"</pair>"<<endl;
	  /*xmldata<<"<pair>"<< endl;
	  xmldata<<"<name>"<<runpairs[i]->firstcontract->contractname <<"-"
		  <<runpairs[i]->secondcontract->contractname<<"</name>" <<endl;
	  xmldata<<"<time>"<<runpairs[i]->firstcontract->marketdata->time<<"</time>"<<endl;
	  xmldata<<"<firstprice>"<<runpairs[i]->firstcontract->marketdata->rtprice<<"</firstprice>"<<endl;
	  xmldata<<"<secondprice>"<<runpairs[i]->secondcontract->marketdata->rtprice<<"</secondprice>"<<endl;
	  xmldata<<"<diffprice>"<<runpairs[i]->secondcontract->marketdata->rtprice-runpairs[i]->firstcontract->marketdata->rtprice<<"</diffprice>"<<endl;
	  xmldata<<"<startprice>"<<runtradeparameters[i]->startprice<<"</startprice>"<<endl;
	  xmldata<<"<computestoragefee>"<<runpairs[i]->computestoragefee()<<"</computestoragefee>"<<endl;
	  xmldata<<"<computestoragedays>"<<runpairs[i]->computestoragedays()<<"</computestoragedays>"<<endl;
	  xmldata<<"<computevatrate>"<<runpairs[i]->computevatrate()<<"</computevatrate>"<<endl;
	  xmldata<<"<computefirstconstractmargin>"<<runpairs[i]->computefirstconstractmargin()<<"</computefirstconstractmargin>"<<endl;
	  xmldata<<"<computesecondconstractmargin>"<<runpairs[i]->computesecondconstractmargin()<<"</computesecondconstractmargin>"<<endl;
	  xmldata<<"<computetransfee>"<<runpairs[i]->computetransfee()<<"</computetransfee>"<<endl;
	  xmldata<<"<computedeliverfee>"<<runpairs[i]->computedeliverfee()<<"</computedeliverfee>"<<endl;
	  xmldata<<"<computedelivermarginfee>"<<runpairs[i]->computedelivermarginfee()<<"</computedelivermarginfee>"<<endl;
	  xmldata<<"<computetrademarginfee>"<<runpairs[i]->computetrademarginfee()<<"</computetrademarginfee>"<<endl;
	  xmldata<<"<computevatfee>"<<runpairs[i]->computevatfee()<<"</computevatfee>"<<endl;
	  xmldata<<"<computearbfee>"<<runpairs[i]->computearbfee()<<"</computearbfee>"<<endl;
	  xmldata<<"<D2>"<<runpairs[i]->secondcontract->daystolastdeliverdate<<"</D2>"<<endl;
	  xmldata<<"<D1>"<<runpairs[i]->firstcontract->daystolastdeliverdate<<"</D1>"<<endl;
	  xmldata<<"<computetotalincome>"<<runpairs[i]->computetotalincome()<<"</computetotalincome>"<<endl;
	  xmldata<<"<computerateofreturn>"<<runpairs[i]->computerateofreturn()<<"</computerateofreturn>"<<endl;
	  xmldata<<"<lendrate>"<<runpairs[i]->commodity->lendrate<<"</lendrate>"<<endl;
	  xmldata<<"<computeinvestmoneyamount>"<<runpairs[i]->computeinvestmoneyamount()<<"</computeinvestmoneyamount>"<<endl;
	  xmldata<<"</pair>"<<endl;*/

	  // ���浽xml�ļ���
	  runtradeparameters[i]->direction = 0;
	  runtradeparameters[i]->tradesize = 0;
	  if((/**/(runtradeparameters[i]->startprice>0)
		  &&runpairs[i]->firstcontract->marketdata->volume>50)
		  &&(runpairs[i]->secondcontract->marketdata->volume>50))
	  {
		  if(  ((runtradeparameters[i]->position==1)
			    &&(runtradeparameters[i]->startprice<0.01))
			 ||((runtradeparameters[i]->position>1)
			    &&(runtradeparameters[i]->startprice>=0.0)) )
		  {
		       continue;
		  }

		if(runtradeparameters[i]->startprice>=0.01)
		{
			runtradeparameters[i]->tradesize = 2;
			runtradeparameters[i]->position += 2;
		}
		else if(runtradeparameters[i]->startprice>0.0)
		{
			runtradeparameters[i]->tradesize = 1;
			runtradeparameters[i]->position =1;
		}

		char filename[128] = "..//Debug//SpreadCost//CSpreadCostStrRun.txt";
		/*sprintf(filename, "..//Debug//%s-%s_%s.txt",
				runpairs[i]->firstcontractname.c_str(),
				runpairs[i]->secondcontractname.c_str(),
				runpairs[i]->firstcontract->marketdata->time.c_str());
		std::string filedir="..//Debug//SpreadCost//" +  runpairs[i]->firstcontractname
							+ runpairs[i]->secondcontractname
							+ runpairs[i]->firstcontract->marketdata->time
							+ ".txt";
		sprintf(filename, filedir.c_str() );*/
		//objectdata.open(filename, ios::trunc);
		objectdata.open(filename, ios::app);
		objectdata<<"������:"<< runpairs[i]->symbol <<endl;
		objectdata<<"       ʱ�䣺    "<<runpairs[i]->firstcontract->marketdata->time<<endl;
		objectdata<<"       ǰ���Լ��"<<runpairs[i]->firstcontractname<<":"<<
			runpairs[i]->firstcontract->marketdata->rtprice<<endl;
		objectdata<<"       �����Լ��"<<runpairs[i]->secondcontractname<<":"<<
			runpairs[i]->secondcontract->marketdata->rtprice<<endl;
		objectdata<<"       �껯�����ʣ�"<<runtradeparameters[i]->startprice*100<<"%"<<endl;

		objectdata<<"       ����������"<<runtradeparameters[i]->tradesize<<endl;
		objectdata<<"       �洢���ã�"<<runpairs[i]->computestoragefee()<<endl;
		objectdata<<"       �洢������"<<runpairs[i]->computestoragedays()<<endl;
		objectdata<<"       ��ֵ˰�ʣ�"<<runpairs[i]->computevatrate()<<endl;
		objectdata<<"       ǰ�ں�Լ��֤�������"<<runpairs[i]->computefirstconstractmargin()<<endl;
		objectdata<<"       ���ں�Լ��֤�������"<<runpairs[i]->computesecondconstractmargin()<<endl;
		objectdata<<"       �����������ã�"<<runpairs[i]->computetransfee()<<endl;
		objectdata<<"       �����������ã�"<<runpairs[i]->computedeliverfee()<<endl;
		objectdata<<"       ���֤��ռ�óɱ���"<<runpairs[i]->computedelivermarginfee()<<endl;
		objectdata<<"       ���ױ�֤��ռ�óɱ���"<<runpairs[i]->computetrademarginfee()<<endl;
		objectdata<<"       ��ֵ˰�ʷ��ã�"<<runpairs[i]->computevatfee()<<endl;
		objectdata<<"       �������ã�"<<runpairs[i]->computearbfee()<<endl;
		objectdata<<"       D2��"<<runpairs[i]->secondcontract->daystolastdeliverdate<<endl;
		objectdata<<"       D1��"<<runpairs[i]->firstcontract->daystolastdeliverdate<<endl;
		objectdata<<"       �����룺"<<runpairs[i]->computetotalincome()<<endl;
		objectdata<<"       �����ʣ�"<<runpairs[i]->computerateofreturn()*100<<"%"<<endl;
		objectdata<<"       �����ʣ�"<<runpairs[i]->commodity->lendrate*100<<"%"<<endl;
		objectdata<<"       ���ʽ�Ͷ�룺"<<runpairs[i]->computeinvestmoneyamount()<<endl;
		objectdata<<endl;
		objectdata.close();
      }
  }
  xmlss<<"</pairstrade>"<< endl;
  //char xmlfilename[128] = "CSpreadCostStrRunOnTime.xml";//".//SpreadCost//CSpreadCostStrRunOnTime.xml";
  char xmlfilename[128] =  "../tongtianshun/app/assets/images/CSpreadCostStrRunOnTime.xml";
  xmlfs.open(xmlfilename, ios::trunc);
  xmlfs<<xmlss.str();
  xmlfs.close();
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
	// ÿ����ִ��һ��
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
