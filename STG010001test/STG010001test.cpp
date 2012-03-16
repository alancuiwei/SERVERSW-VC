// STG010001test.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <fstream>
#include <time.h>


void StartReceiveMD()
{
    struct tm *nowtm;
	time_t nowtime;
    struct tm exittm;
	time_t exittime;
	double secnum;

	nowtime=time(NULL);
	nowtm=localtime(&nowtime);
	exittm=*nowtm;

	// ֻ����7�㵽17��ſ�����������
	if((exittm.tm_hour>=8)&&(exittm.tm_hour<=18))
	{
		if((exittm.tm_hour<15)||((exittm.tm_hour==15)&&(exittm.tm_min<30)))
		{
			exittm.tm_hour = 15;
			exittm.tm_min = 30;
			exittm.tm_sec = 0;
			// �õ�����ʱ��
			exittime = mktime(&exittm);
			secnum = difftime( exittime, nowtime );
			// ��������
			CCOMMClient *pclientobj = new CCOMMClient("192.168.0.108",6000);
			pclientobj->Start();
			Sleep(1000*secnum);
		}
		else
		{
			// ��������
			CCOMMClient *pclientobj = new CCOMMClient("192.168.0.108",6000);
			pclientobj->Start();
			// �ӳ������
			Sleep(1000*300);
		}

	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//std::string test=g_ssm_puser->get_username();
	//std::cout<<test<<"OK"<<std::endl;

	/*create user*/
	StartSSM("tester1");

	CSpreadCostStrRun* pstrategy = new CSpreadCostStrRun("01");


	/*���Զ�������*/
	//char filename[64] = "";
	//sprintf(filename, "objectdata.csv" );
	ofstream objectdata;
	objectdata.open("..//Debug//objectdata.csv", ios::trunc);
    objectdata<<"�û���Ϣ:"<<endl;
	objectdata<<",username,"<<ssm_puser->username<<endl;
	objectdata<<",usertype,"<<ssm_puser->usertype<<endl;
	objectdata<<",ibbranchid,"<<ssm_puser->ibbranchid<<endl;
	objectdata<<endl<<endl;

    objectdata<<"��������Ϣ:"<<endl;
	for(std::vector<CPair*>::size_type i=0; i<ssm_ppairvector.size();i++)
	{
	    objectdata<<",pairid,"<<ssm_ppairvector[i]->pairid<<endl;
		objectdata<<",firstcontractname,"<<ssm_ppairvector[i]->firstcontractname<<endl;
		objectdata<<",secondcontractname,"<<ssm_ppairvector[i]->secondcontractname<<endl;
		objectdata<<",productid,"<<ssm_ppairvector[i]->productid<<endl;
		objectdata<<",strategyid,"<<ssm_ppairvector[i]->strategyid<<endl;
		objectdata<<",isinstrumentsupport,"<<ssm_ppairvector[i]->isinstrumentsupport<<endl;
		objectdata<<",strategytypeid,"<<ssm_ppairvector[i]->strategytypeid<<endl;
		objectdata<<",symbol,"<<ssm_ppairvector[i]->symbol<<endl;
		objectdata<<",storagedays,"<<((CSpreadCostPair*)ssm_ppairvector[i])->storagedays<<endl;
		objectdata<<",firstcontractmargin,"<<((CSpreadCostPair*)ssm_ppairvector[i])->firstcontractmargin<<endl;
		objectdata<<",secondcontractmargin,"<<((CSpreadCostPair*)ssm_ppairvector[i])->secondcontractmargin<<endl;
		objectdata<<",vatrate,"<<((CSpreadCostPair*)ssm_ppairvector[i])->vatrate<<endl;
		objectdata<<",delivercost,"<<((CSpreadCostPair*)ssm_ppairvector[i])->deliverfee<<endl;
		objectdata<<endl;
		pstrategy->AddPair((CSpreadCostPair*)ssm_ppairvector[i]);
	}
	objectdata<<endl;

    objectdata<<"��Լ��Ϣ:"<<endl;
	for(std::vector<CContract*>::size_type i=0; i<ssm_pcontractvector.size();i++)
	{
	    objectdata<<",contractname,"<<ssm_pcontractvector[i]->contractname<<endl;
        objectdata<<",commodityname,"<<ssm_pcontractvector[i]->commodityname<<endl;
		objectdata<<",uplimitprice,"<<ssm_pcontractvector[i]->uplimitprice<<endl;
		objectdata<<",downlimitprice,"<<ssm_pcontractvector[i]->downlimitprice<<endl;
		objectdata<<",daystolasttradedate,"<<ssm_pcontractvector[i]->daystolasttradedate<<endl;
		objectdata<<",oilimit,"<<ssm_pcontractvector[i]->oilimit<<endl;
		objectdata<<",daystolastdeliverdate,"<<ssm_pcontractvector[i]->daystolastdeliverdate<<endl;
		objectdata<<",lendrate,"<<ssm_pcontractvector[i]->lendrate<<endl;
		objectdata<<",actualmarginrate,"<<ssm_pcontractvector[i]->actualmarginrate<<endl;
		objectdata<<endl;
	}
	objectdata<<endl;

	objectdata<<"Ʒ����Ϣ:"<<endl;
	for(std::vector<CCommodity*>::size_type i=0; i<ssm_pcommodityvector.size();i++)
	{
	    objectdata<<",commodityname,"<<ssm_pcommodityvector[i]->commodityname<<endl;
		objectdata<<",exchangeid,"<<ssm_pcommodityvector[i]->exchangeid<<endl;
		objectdata<<",tradeunit,"<<ssm_pcommodityvector[i]->tradeunit<<endl;
		objectdata<<",todayexitdiscount,"<<ssm_pcommodityvector[i]->todayexitdiscount<<endl;
		objectdata<<",cancelmonth,"<<ssm_pcommodityvector[i]->cancelmonth<<endl;
		objectdata<<",delivermonth,"<<ssm_pcommodityvector[i]->delivermonth<<endl;
		objectdata<<",issinglemargin,"<<ssm_pcommodityvector[i]->issinglemargin<<endl;
		objectdata<<",tick,"<<ssm_pcommodityvector[i]->tick<<endl;
		objectdata<<",transtype,"<<ssm_pcommodityvector[i]->transtype<<endl;
		objectdata<<",transfee,"<<ssm_pcommodityvector[i]->transfee<<endl;
		objectdata<<",vatrate,"<<ssm_pcommodityvector[i]->vatrate<<endl;
		objectdata<<",storagedailyfee,"<<ssm_pcommodityvector[i]->storagedailyfee<<endl;
		objectdata<<",lendrate,"<<ssm_pcommodityvector[i]->lendrate<<endl;
		objectdata<<",deliverchargebyhand,"<<ssm_pcommodityvector[i]->deliverchargebyhand<<endl;
		objectdata<<endl;
	}
	objectdata<<endl;

	objectdata.close();
	comm_prtmarketdatamap = ssm_prtmarketdatamap;
	pstrategy->Start();
	StartReceiveMD();
	while(false)
	{
		for(std::vector<CContract*>::size_type i=0; i<ssm_pcontractvector.size();i++)
		{
			printf("%s:%lf\n", 
				ssm_pcontractvector[i]->contractname.c_str(),
				ssm_pcontractvector[i]->marketdata->rtprice);
		}	
	}
	//Sleep(60);
	return 0;
}


