/***********************************************************************
 * Module:  CRTMarketData.h
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:14:04
 * Purpose: Declaration of the class CRTMarketData
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CRTMarketData_h)
#define __SSMClassDiagram_CRTMarketData_h
//#include "stdafx.h"
#include <string>


class  CRTMarketData
{
public:
   CRTMarketData(std::string name);

   /* 合约名 */
   //std::string constractname;
   char constractname[31];
   /* 实时价格 */
   double rtprice;
   /* 申卖价一 */
   double askprice1;
   /* 申卖价二 */
   double askprice2;
   /* 申卖价三 */
   double askprice3;
   /* 申卖价四 */
   double askprice4;
   /* 申卖价五 */
   double askprice5;
   /* 申买价一 */
   double bidprice1;
   /* 申买价二 */
   double bidprice2;
   /* 申买价三 */
   double bidprice3;
   /* 申买价四 */
   double bidprice4;
   /* 申买价五 */
   double bidprice5;
   /* 今日开盘价 */
   double openprice;
   /* 最高价 */
   double highestprice;
   /* 最低价 */
   double lowestprice;
   /* 成交量 */
   long int volume;
   //std::string time;
   char time[9];


protected:
private:

};

#endif
