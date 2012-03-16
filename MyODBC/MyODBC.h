// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYODBC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYODBC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef MYODBC_EXPORTS
#define MYODBC_API __declspec(dllexport)
#else
#define MYODBC_API __declspec(dllimport)
#endif

//#include <windows.h>   
#include "MySQLAPI.h"
#include <iostream>   
#include <sql.h>   
#include <sqlext.h>   
#include <vector>   
#include <string>   

using namespace std; 


/*CMyODBC的对象*/   
class MYODBC_API CMyODBC  
{  
private:  
    SQLHANDLE hEnv;  
    SQLHANDLE hDbc;  
    SQLHANDLE hStmt;  
    SQLRETURN retCode;  
    SQLINTEGER retErro;  
    SQLINTEGER rowCount;  
    SQLSMALLINT colCount;  
    bool bState;  
    char* pszUName;  
    char* pszUPassword;  
    char* pszDSN;
	//CMyODBC();
	//static CMyODBC* pinstance;
public:
	CMyODBC();
	~CMyODBC();  
	//static CMyODBC* GetMyODBCInstance();    
    bool Connect();  
    bool Close(); 
	bool Open();
    bool IsOpen();  
    int GetRowCount(){return rowCount;}  
    int GetColCount(){return colCount;}  
    vector<string*> ExecuteQueryVector(const char* pszSql); 
	string* ExecuteSingleQuery(const char* pszSql);  //执行一条查询
    int ExecuteQuery(const char* pszSql);  //执行查询数目
    int ExecuteNonQuery(const char* pszSql);//执行非查询(更新或删除)
	bool DisconnectDB();
};