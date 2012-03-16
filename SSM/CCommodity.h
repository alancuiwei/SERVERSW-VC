/***********************************************************************
 * Module:  CCommodity.h
 * Author:  CUIWei
 * Modified: 2011��10��24������һ 22:20:55
 * Purpose: Declaration of the class CCommodity
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CCommodity_h)
#define __SSMClassDiagram_CCommodity_h

#ifdef SSM_EXPORTS
#define SSM_API __declspec(dllexport)
#else
#define SSM_API __declspec(dllimport)
#endif

#include "stdafx.h"

class CContract;

class SSM_API  CCommodity
{
public:
   CCommodity(std::string commodityname);
   void initialization(void);

   vector<CContract*> contractvector;
   /* ��Ʒ�� */
   std::string commodityname;
   double Getlendrate(){return lendrate;};

   /* �������� */
   std::string exchangeid;
   /* ���׵�λ */
   int tradeunit;
   /* ƽ���Ż� */
   double todayexitdiscount;
   /* ע���·� */
   std::string cancelmonth;
   /* �����·� */
   std::string delivermonth;
   /* �Ƿ�ֻ��ȡ����֤��
    * 0��˫����ȡ��֤��
    * 1��������ȡ��֤�� */
   bool issinglemargin;
   /* ��С�۸�䶯��λ */
   int tick;
   /* ����������
    * 0������
    * 1�����ɽ���� */
   int transtype;
   /* ������ */
   double transfee;
   /* ��ֵ˰�� */
   double vatrate;
   /* �ִ��쵥�� */
   double storagedailyfee;
   double lendrate;
   double tradechargetype;
   double tradecharge;
   double deliverchargebyhand;



};

#endif