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

	// 只有在7点到17间才可以正常运行
	if((exittm.tm_hour>=8)&&(exittm.tm_hour<=25))
	{
		if(((exittm.tm_hour>8)&&(exittm.tm_hour<25))
			||((exittm.tm_hour==15)&&(exittm.tm_min<1))
			||((exittm.tm_hour==8)&&(exittm.tm_min>40)))
		{
			tqm_issavetodb=false;
			exittm.tm_hour = 15;
			exittm.tm_min = 16;
			exittm.tm_sec = 0;
			// 得到结束时间
			exittime = mktime(&exittm);
			secnum = difftime( exittime, nowtime );
			// 接受行情
			StartTraderThreadProcess();
			StartMdThreadProcess( );

			usleep(1000*secnum*1000);
		}
		else
		{
			// 延迟10分钟
			tqm_issavetodb=true;
			StartTraderThreadProcess();
			StartMdThreadProcess( );
			usleep(1000*900*1000);
		}
	}

    return 0;
}

