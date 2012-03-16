#pragma once
#ifdef MYODBC_EXPORTS
#define MYODBC_API __declspec(dllexport)
#else
#define MYODBC_API __declspec(dllimport)
#endif
#include "winsock.h"
//#include "iostream.h"
#include <string>
#include "include/mysql.h"
#include <string>
#include <iostream>
#include <vector>  
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "../Debug/libmysql.lib")
using namespace std;

MYODBC_API typedef enum
{
	SQLCONNECT_SUCCESS = 0,
	SQLCONNECT_FAILURE,
	SQLQUERY_SUCCESS,
	SQLQUERY_FAILURE,
	SQLEXPECTSINGLEROW_FAILURE,
} ENUM_SQLREPORTTYPE;

class MYODBC_API  CMySQLAPI
{
private:
	MYSQL mydata;                      //MySQL���󣬱ر���һ�����ݽṹ
	MYSQL_RES *result;                 //���ڴ�Ž�� ������char* ���齫�˽��ת��
public:
	char* pServer;                     //��������
	char* pUName;                      //���ݿ��˺�
	char* pUPassword;                  //����
	char* pDSN;                        //���ݿ���&����Դ��
	unsigned int pPort;                //�˿ں�
	int   rowCount;                    //��ѯ���������
	int   colCount;                    //��ѯ���������
	int   autoNum;                     //������ֵ
	int   errorNum;                    //�������
	const char* errorInfo;             //������ʾ
	int   affectedRow;                 //ִ��Ӱ�������

public:
	CMySQLAPI(void);
	~CMySQLAPI(void);
	//��ʼ��
	void Init();
	//�����������ݿ�
	bool Open();
	//�ر����ݿ�
	void Close();
	//��ѯ�������
	vector<string*> ExecuteQueryVector(const char* pSql); 
	//��ѯһ�����
	string* ExecuteSingleQuery(const char* pSql);
	//ִ�в�ѯ��Ŀ
	int ExecuteQueryNum(const char* pSql);
	//ִ�зǲ�ѯ(���»�ɾ��)
	void ExecuteNonQuery(const char* pSql);
	//����ִ�н��
	void ReportResult(ENUM_SQLREPORTTYPE flag);
	//�����������
    void CheckError(); 
	

	            
	
};

