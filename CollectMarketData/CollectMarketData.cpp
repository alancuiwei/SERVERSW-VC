// CollectMarketData.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
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
	if((exittm.tm_hour>=8)&&(exittm.tm_hour<=17))
	{
		if(((exittm.tm_hour>8)&&(exittm.tm_hour<15))
			||((exittm.tm_hour==15)&&(exittm.tm_min<1))
			||((exittm.tm_hour==8)&&(exittm.tm_min>40)))
		{
			tqm_issavetodb=false;
			exittm.tm_hour = 15;
			exittm.tm_min = 16;
			exittm.tm_sec = 0;
			// �õ�����ʱ��
			exittime = mktime(&exittm);
			secnum = difftime( exittime, nowtime );
			// ��������
			StartTraderThreadProcess();
			StartMdThreadProcess( );

			Sleep(1000*secnum);
		}
		else
		{
			// �ӳ�10����
			tqm_issavetodb=true;
			StartTraderThreadProcess();
			StartMdThreadProcess( );
			Sleep(1000*900);
		}
	}

    return 0;
}

