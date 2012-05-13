// CollectMarketData.cpp : Defines the entry point for the console application.
#include "../TQM/TQM.h"
//#include "../COMM/lightthread.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main( )
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
	if((exittm.tm_hour>=8)&&(exittm.tm_hour<=15))
	{
		if(((exittm.tm_hour>8)&&(exittm.tm_hour<15))
			||((exittm.tm_hour==15)&&(exittm.tm_min<17))
			||((exittm.tm_hour==8)&&(exittm.tm_min>40)))
		{
			tqm_issavetodb=false;
			exittm.tm_hour = 15;
			exittm.tm_min = 17;
			exittm.tm_sec = 10;
			// �õ�����ʱ��
			exittime = mktime(&exittm);
			secnum = difftime( exittime, nowtime );
			// ��������
			StartTraderThreadProcess();
			StartMdThreadProcess( );
			std::cout << secnum << std::endl;

			sleep(secnum);
		}
		else
		{
			// �ӳ�10����
			tqm_issavetodb=true;
			StartTraderThreadProcess();
			StartMdThreadProcess( );
			sleep(600);
		}
	}

    return 0;
}

