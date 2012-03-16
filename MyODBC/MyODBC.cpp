// MyODBC.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MyODBC.h"


/*CMyODBC* CMyODBC::pinstance=NULL;

// This is the constructor of a class that has been exported.
// see MyODBC.h for the class definition
CMyODBC* CMyODBC::GetMyODBCInstance(void)
{
	if(pinstance == NULL)
	{
		pinstance = new CMyODBC();
	}
	return pinstance;
}; */

CMyODBC::CMyODBC()  
{  
    bState=false;  
    pszUName="root";       //用户名   
    pszUPassword="123456";  //密码   
    pszDSN="futuretest";         //数据源名   
    Open(); 
   
}  
CMyODBC::~CMyODBC()  
{  
    Close();  
}  
bool CMyODBC::Connect()  
{  
    if(bState==false)  
    {
	  Open();
      retCode=SQLConnect(hDbc,(SQLCHAR*) pszDSN,SQL_NTS,(SQLCHAR*) pszUName,SQL_NTS,(SQLCHAR*) pszUPassword,SQL_NTS);  
       if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
       {  
           cout<<"Erro Connect "<<retCode<<endl;  
           SQLFreeHandle( SQL_HANDLE_DBC, hDbc );  
           return false;  
       }  
       retCode=SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt);  
       if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
       {  
           cout<<"Erro Connect "<<retCode<<endl;  
           SQLDisconnect( hDbc );  
           SQLFreeHandle( SQL_HANDLE_DBC, hDbc);  
           return false;  
       }  
    }       
    bState=true;  
    cout<<"success!"<<endl;  
    return true;  
     
}  
int CMyODBC::ExecuteQuery(const char* pszSql)  
{  
     
    if(pszSql==NULL)  
       return 0;  
    cout<<"hStmt="<<hStmt<<endl;  
    retCode=SQLExecDirect(hStmt,(SQLCHAR*)pszSql,SQL_NTS);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ExecDirect "<<retCode<<endl;  
       return -1;  
    }  
/*  retCode=SQLRowCount(hStmt,&rowCount);  //不受select 影响。。 
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO)) 
    { 
       cout<<"Erro RowCount "<<retCode<<endl; 
       return -1; 
    }*/  
    retCode=SQLNumResultCols(hStmt,&colCount);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ColCount "<<retCode<<endl;  
       return -1;  
    }  
    rowCount=0;  
    while(SQL_NO_DATA!=SQLFetch(hStmt))  
    {  
       //cout<<pszBuf<<endl;   
       rowCount++;  
    }  
    SQLCancel(hStmt);  
    return rowCount;  
}  
int CMyODBC::ExecuteNonQuery(const char* pszSql)  
{  
    rowCount=0;  
    if(pszSql==NULL)  
       return 0;  
    //cout<<"hStmt="<<hStmt<<endl;   
    retCode=SQLExecDirect(hStmt,(SQLCHAR*)pszSql,SQL_NTS);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ExecDirect "<<retCode<<endl;  
       return -1;  
    }  
    retCode=SQLRowCount(hStmt,&rowCount);   
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro RowCount "<<retCode<<endl;  
       return -1;  
    }  
    retCode=SQLNumResultCols(hStmt,&colCount);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ColCount "<<retCode<<endl;  
       return -1;  
    }  
     
    SQLCancel(hStmt);  
    return rowCount;  
}  
vector<string* >  CMyODBC::ExecuteQueryVector(const char* pszSql)  
{  
    vector<string* > v;  
    if(pszSql==NULL)  
       return v;  
    retCode=SQLExecDirect(hStmt,(SQLCHAR*)pszSql,SQL_NTS);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ExecDirect "<<retCode<<endl;  
       return v;  
    }  
/*  retCode=SQLRowCount(hStmt,&rowCount);  //不受select 影响。。 
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO)) 
    { 
       cout<<"Erro RowCount "<<retCode<<endl; 
       return -1; 
    }*/  
    retCode=SQLNumResultCols(hStmt,&colCount);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ColCount "<<retCode<<endl;  
       return v;  
    }  
    rowCount=0;  
    SQLINTEGER colLen = 0;  
    SQLSMALLINT buf_len = 0;  
    SQLINTEGER colType = 0;  
   
    while(true)  
    {    
       char sz_buf[256];  
       char* pszBuf;  
       SQLINTEGER  buflen;  
       string* rowData=new string[colCount+1];    
       if(SQLFetch(hStmt)==SQL_NO_DATA)  
       {  
           break;  
       }  
       for(int i=1;i<=colCount;i++)  
       {  
           SQLColAttribute(hStmt, i, SQL_DESC_NAME, sz_buf, 256, &buf_len, 0);  
           SQLColAttribute(hStmt, i, SQL_DESC_TYPE, 0, 0, 0, &colType);  
           SQLColAttribute(hStmt, i, SQL_DESC_LENGTH, NULL, 0, 0, &colLen);  
           pszBuf=new char[colLen+1];  
           pszBuf[0]='\0';  
           SQLGetData(hStmt,i,SQL_C_CHAR,pszBuf,50,&buflen);  
           rowData[i-1]=pszBuf;  
       }  
       v.push_back(rowData);     
       rowCount++;  
       //cout<<"rowData[0]="<< (rowData[0])<<" rowData[1]="<<rowData[1]<<" rowData[2]="<<rowData[2]<<endl;;   
    }  
    SQLCancel(hStmt);  
    return v;  
}  

string* CMyODBC::ExecuteSingleQuery(const char* pszSql)
{ 
    if(pszSql==NULL)  
       return NULL;  
    retCode=SQLExecDirect(hStmt,(SQLCHAR*)pszSql,SQL_NTS);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ExecDirect "<<retCode<<endl;  
       return NULL;  
    }  
 
    retCode=SQLNumResultCols(hStmt,&colCount);  
    if((retCode != SQL_SUCCESS) && (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro ColCount "<<retCode<<endl;  
       return NULL;  
    }  
    rowCount=0;  
    SQLINTEGER colLen = 0;  
    SQLSMALLINT buf_len = 0;  
    SQLINTEGER colType = 0;  
    
    char sz_buf[256];  
    char* pszBuf;  
    SQLINTEGER  buflen;  
    string* rowData=new string[colCount+1];    
    if(SQLFetch(hStmt)==SQL_NO_DATA)  
    {  
        return NULL;  
    }  
    for(int i=1;i<=colCount;i++)  
    {  
        SQLColAttribute(hStmt, i, SQL_DESC_NAME, sz_buf, 256, &buf_len, 0);  
        SQLColAttribute(hStmt, i, SQL_DESC_TYPE, 0, 0, 0, &colType);  
        SQLColAttribute(hStmt, i, SQL_DESC_LENGTH, NULL, 0, 0, &colLen);  
        pszBuf=new char[colLen+1];  
        pszBuf[0]='\0';  
        SQLGetData(hStmt,i,SQL_C_CHAR,pszBuf,50,&buflen);  
        rowData[i-1]=pszBuf;  
    }  
	          
    rowCount++;  

    SQLCancel(hStmt);  
    return rowData;  
}


bool CMyODBC::Close()  
{  
    if(bState)  
    {  
       SQLDisconnect(hDbc);  
       SQLFreeHandle(SQL_HANDLE_DBC,hDbc);  
       SQLFreeHandle(SQL_HANDLE_ENV,hEnv);  
       bState=false;  
    }  
   
    return true;  
}  
bool CMyODBC::IsOpen()  
{  
    return bState;  
}  

bool CMyODBC::DisconnectDB()
{
    if(bState)  
    {  
       SQLDisconnect(hDbc);   
       bState=false; 
	   return true;
    } 
	return false;
}

bool CMyODBC::Open()
{
    rowCount=colCount=0;  
    retCode=SQLAllocHandle(SQL_HANDLE_ENV,NULL,&hEnv);  
    if((retCode!=SQL_SUCCESS)&& (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro AllocHandle"<<retCode<<endl;  
       return false;  
    }  
    retCode=SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER) SQL_OV_ODBC3,SQL_IS_INTEGER);  
    if((retCode!=SQL_SUCCESS)&& (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro AllocHandle"<<retCode<<endl;  
       SQLFreeHandle( SQL_HANDLE_DBC, hEnv );  
       return false;  
    }  
    retCode=SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);  
    if((retCode!=SQL_SUCCESS)&& (retCode != SQL_SUCCESS_WITH_INFO))  
    {  
       cout<<"Erro AllocHandle"<<retCode<<endl;  
       SQLFreeHandle( SQL_HANDLE_DBC, hEnv );  
       return false;  
    }

	return true;
}