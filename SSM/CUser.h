/***********************************************************************
 * Module:  CUser.h
 * Author:  CUIWei
 * Modified: 2011��10��24������һ 22:21:38
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
   /* �ù��캯����username���и�ֵ�� */
   CUser(std::string username);
   /* Initialization��usertype �� allowpairs���м��㸳ֵ�� */
   virtual void initialization(void);
   /* ͨ��username��ѯ���û����з��ʵ������Ժ��������ͼ��ϣ�����һ���˴���classes
    * ������Щclassesָ������allowpairs������ */
   void computeallowpairs(void);
   std::string username;


   /* 0��Ȼ��1���� */
   int usertype;
   /* ������û�������ʾ��������Լ��class */
   vector<CPair*>* allowpairs;
   /* Ӫҵ��id */
   std::string ibbranchid;
   vector<std::string> allowproductids;

};

#endif