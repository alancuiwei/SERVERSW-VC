#pragma once


class  CTradeStru
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

