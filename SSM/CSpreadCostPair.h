/***********************************************************************
 * Module:  CSpreadCostPair.h
 * Author:  Administrator
 * Modified: 2011��10��26�������� 23:04:22
 * Purpose: Declaration of the class CSpreadCostPair
 * Comment: �޷��տ�������
 ***********************************************************************/

#if !defined(__Arbitrage_Client_CSpreadCostPair_h)
#define __Arbitrage_Client_CSpreadCostPair_h


#include "CPair.h"

class   CSpreadCostPair : public CPair
{
public:
	CSpreadCostPair();
	~CSpreadCostPair();
   CSpreadCostPair(std::string firstconstractname, std::string secondconstractname, std::string productid);
   virtual void initialization(void);
   /* ����洢���� */
   double computestoragefee(void);
   /* ����洢���� */
   double computestoragedays(void);
   /* ������ֵ˰�� */
   double computevatrate(void);
   /* ����ǰ�ں�Լ��֤����� */
   double computefirstconstractmargin(void);
   /* ������ں�Լ��֤����� */
   double computesecondconstractmargin(void);
   /* ���㽻���������� */
   double computetransfee(void);
   /* ���㽻���������� */
   double computedeliverfee(void);
   /* ���㽻�֤��ռ�óɱ� */
   double computedelivermarginfee(void);
   /* ���㽻�ױ�֤��ռ�óɱ� */
   double computetrademarginfee(void);
   /* ������ֵ˰�ʷ��� */
   double computevatfee(void);
   /* ������������ */
   double computearbfee(void);
   /* ���������� */
   double computetotalincome(void);
   /* ����������
    * ��֤���������ʣ�
    * float return_ratio = profit/((r_price+marginamount_forward)*primary_number);
    * ������������ʣ�
    * float return_ratio = profit/((marginamount_recent+marginamount_forward)*primary_number) */
   double computerateofreturn(void);
   /* �����껯������ */
   double computeannualrateofreturn(void);
   /* ���㵱ǰƽ�������� */
   double computecurrentrateofreturn(void);
   double computeinvestmoneyamount(void);


   /* �洢���� */
   double storagedays;
   /* ���ں�Լ��֤����� */
   double firstcontractmargin;
   /* Զ�ں�Լ��֤����� */
   double secondcontractmargin;
   /* ��Ʒ��ֵ˰�� */
   double vatrate;
   /* ������������ */
   double transfee;
   /* ������������ */
   double deliverfee;

   CCommodity* commodity;


};

#endif
