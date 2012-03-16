#pragma once
#ifdef STG010001_EXPORTS
#define STG010001_API __declspec(dllexport)
#else
#define STG010001_API __declspec(dllimport)
#endif

class STG010001_API CTradeStru
{
public:
   /* ׼������� */
   double startprice;
   /* ׼��ֹ��� */
   double stopprice;
   /* 0:buy 1:sell */
   int direction;
   /* �µ����� */
   int tradesize;
   /* ���˳ֲ��� */
   int position;
	CTradeStru(void);
	~CTradeStru(void);
};

