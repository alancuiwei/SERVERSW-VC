// STG010001test.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../COMM/COMM.h"
#include "../SSM/SSM.h"
#include "../STG010001/STG010001.h"

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

	// 只有在7点到17间才可以正常运行
	if((exittm.tm_hour>=8)&&(exittm.tm_hour<=15))
	{
		if((exittm.tm_hour<15)||((exittm.tm_hour==15)&&(exittm.tm_min<17)))
		{
			exittm.tm_hour = 15;
			exittm.tm_min = 17;
			exittm.tm_sec = 0;
			// 得到结束时间
			exittime = mktime(&exittm);
			secnum = difftime( exittime, nowtime );
			// 接受行情
			CCOMMClient *pclientobj = new CCOMMClient("127.0.0.1",6000);
			pclientobj->Start();
			sleep(secnum);
		}
		else
		{
			// 接受行情
			CCOMMClient *pclientobj = new CCOMMClient("127.0.0.1",6000);
			pclientobj->Start();
			// 延迟五分钟
			sleep(300);
		}

	}
}

int main( )
{
	/*create user*/
	StartSSM("tester1");

	CSpreadCostStrRun* pstrategy = new CSpreadCostStrRun("01");

	for(std::vector<CPair*>::size_type i=0; i<ssm_ppairvector.size();i++)
	{
		pstrategy->AddPair((CSpreadCostPair*)ssm_ppairvector[i]);
	}

	comm_prtmarketdatamap = ssm_prtmarketdatamap;
	pstrategy->SavePairsXML(NULL);
	pstrategy->Start();
	StartReceiveMD();

	return 0;
}


