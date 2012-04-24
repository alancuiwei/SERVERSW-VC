/***********************************************************************
 * Module:  CContract.h
 * Author:  CUIWei
 * Modified: 2011��10��24������һ 22:21:23
 * Purpose: Declaration of the class CContract
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CConstract_h)
#define __SSMClassDiagram_CConstract_h

#include "CCommodity.h"

class CCommodity;
class CPair;
//class CRTMarketData;

class  CContract
{
public:
   CContract(std::string contractname);
   ~CContract();
   void initialization(void);
   /* ��Լ�� */
   std::string contractname;

   vector<CPair*> pairvector;


   /* ������Ʒ�� */
   CCommodity* commodity;
   /* ��Ʒ�� */
   std::string commodityname;
   /* �г����� */
   CRTMarketData* marketdata;
   /* ������ͣ�� */
   double uplimitprice;
   /* ���յ�ͣ�� */
   double downlimitprice;
   /* ����������ջ��ж����� */
   double daystolasttradedate;
   /* �ֲ��޶� */
   double oilimit;
   /* ���뽻��ж����� */
   double daystolastdeliverdate;
   /* �������� */
   double lendrate;
   /* ʵ�ʱ�֤���� */
   double actualmarginrate;


};

#endif
