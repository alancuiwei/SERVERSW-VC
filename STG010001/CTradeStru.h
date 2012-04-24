#pragma once


class  CTradeStru
{
public:
   /* 准备买入点 */
   double startprice;
   /* 准备止损点 */
   double stopprice;
   /* 0:buy 1:sell */
   int direction;
   /* 下单数量 */
   int tradesize;
   /* 个人持仓量 */
   int position;
	CTradeStru(void);
	~CTradeStru(void);
};

