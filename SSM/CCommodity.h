/***********************************************************************
 * Module:  CCommodity.h
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:20:55
 * Purpose: Declaration of the class CCommodity
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CCommodity_h)
#define __SSMClassDiagram_CCommodity_h


#include "stdafx.h"

class CContract;

class  CCommodity
{
public:
   CCommodity(std::string commodityname);
   void initialization(void);

   vector<CContract*> contractvector;
   /* 商品名 */
   std::string commodityname;
   double Getlendrate(){return lendrate;};

   /* 交易所名 */
   std::string exchangeid;
   /* 交易单位 */
   int tradeunit;
   /* 平今优惠 */
   double todayexitdiscount;
   /* 注销月份 */
   std::string cancelmonth;
   /* 交割月份 */
   std::string delivermonth;
   /* 是否只收取单向保证金
    * 0：双向收取保证金
    * 1：单向收取保证金 */
   bool issinglemargin;
   /* 最小价格变动单位 */
   int tick;
   /* 手续费类型
    * 0：按手
    * 1：按成交金额 */
   int transtype;
   /* 手续费 */
   double transfee;
   /* 增值税率 */
   double vatrate;
   /* 仓储天单价 */
   double storagedailyfee;
   double lendrate;
   double tradechargetype;
   double tradecharge;
   double deliverchargebyhand;



};

#endif
