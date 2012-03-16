/***********************************************************************
 * Module:  CUser.h
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:21:38
 * Purpose: Declaration of the class CUser
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CUser_h)
#define __SSMClassDiagram_CUser_h

#ifdef SSM_EXPORTS
#define SSM_API __declspec(dllexport)
#else
#define SSM_API __declspec(dllimport)
#endif

#include "CPair.h"

class SSM_API CUser
{
public:
   /* 该构造函数对username进行赋值。 */
   CUser(std::string username);
   /* Initialization对usertype 和 allowpairs进行计算赋值。 */
   virtual void initialization(void);
   /* 通过username查询该用户运行访问的套利对和套利类型集合，并逐一按此创建classes
    * 并将这些classes指针存放在allowpairs数组中 */
   void computeallowpairs(void);
   std::string username;


   /* 0自然人1法人 */
   int usertype;
   /* 保存该用户可以显示的套利合约对class */
   vector<CPair*>* allowpairs;
   /* 营业厅id */
   std::string ibbranchid;
   vector<std::string> allowproductids;

};

#endif