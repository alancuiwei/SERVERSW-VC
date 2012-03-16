#include "StdAfx.h"
#include "MySQLAPI.h"



CMySQLAPI::CMySQLAPI(void)
{
	pServer="localhost";  //服务器
    pUName="root";       //用户名   
    pUPassword="123456";  //密码   
    pDSN="futuretest";         //数据源名 
	pPort=3306;
    Init(); 
}


CMySQLAPI::~CMySQLAPI(void)
{
	Close();
}

//编译时显示测试信息，正式使用时把此函数主题注释掉即可
void CMySQLAPI::ReportResult(ENUM_SQLREPORTTYPE flag)
{
	char* message;
	switch (flag)
	{
		case SQLCONNECT_SUCCESS:
			message="Connect successful!";
			break;
		case SQLCONNECT_FAILURE:
			message="Connect error!";
			break;
		case SQLQUERY_SUCCESS:
			message="SQL Query OK!";
			break;
		case SQLQUERY_FAILURE:
			message="SQL Query error!";
			break;
		case SQLEXPECTSINGLEROW_FAILURE:
			message="SQL Query expect single row error!";
			break;
		default:
			message=NULL;
			break;
	}
	if ( message!=NULL )
	{
		cout<<message<<endl;
		CheckError();
	}
}

//收集错误信息
void CMySQLAPI::CheckError()
{
	errorNum = mysql_errno(&mydata);
	errorInfo = mysql_error(&mydata);
	cout<<"errorNum:"<<errorNum<<endl;
	cout<<"errorInfo:"<<errorInfo<<endl;
}

//执行查询语句
void CMySQLAPI::ExecuteNonQuery(const char* pSql)
{
	if( NULL != mysql_query(&mydata,pSql) )
	//	ReportResult(SQLQUERY_SUCCESS);
	//else
		ReportResult(SQLQUERY_FAILURE);
}


//连接数据库
bool CMySQLAPI::Open()
{
	if( mysql_real_connect(&mydata,pServer,pUName,pUPassword,pDSN,pPort,0,0) != NULL)
		return true;
	else
		return false;
}

void CMySQLAPI::Init()
{
	mysql_library_init(0,NULL,NULL);
	mysql_init(&mydata);
	mysql_options(&mydata,MYSQL_SET_CHARSET_NAME,"gbk");

	rowCount=0;
	colCount=0;
	autoNum=0;
	errorNum=0;
	errorInfo="ok";
	affectedRow=0;
}


vector<string*> CMySQLAPI::ExecuteQueryVector(const char* pSql)
{
	vector<string* > v;
	if( 0==mysql_query(&mydata,pSql) )
		ReportResult(SQLQUERY_SUCCESS);
	else
		ReportResult(SQLQUERY_FAILURE);

	affectedRow = (int)mysql_affected_rows(&mydata);
	autoNum = (int)mysql_insert_id(&mydata);
	result = mysql_store_result(&mydata);

	rowCount = (int)mysql_num_rows(result);//计算行数
	colCount = (int)mysql_num_fields(result);//计算列数


	MYSQL_ROW line=NULL;
	line=mysql_fetch_row(result);//取第一行结果

	int j=0;
	while( NULL!=line )//查看是否为空行，是则结束，否则将这一行的数据转存到data的一行中并取下一行
	{	 
		string* rowData=new string[colCount]; 
		for(int i=0; i<colCount;i++)
		{
			rowData[i]=line[i];
		}
		j++;
		v.push_back(rowData); 
		line=mysql_fetch_row(result);
	}

	CheckError();
	return v;
}


string* CMySQLAPI::ExecuteSingleQuery(const char* pSql)
{
	if( 0==mysql_query(&mydata,pSql) )
		ReportResult(SQLQUERY_SUCCESS);
	else
		ReportResult(SQLQUERY_FAILURE);

	affectedRow = (int)mysql_affected_rows(&mydata);
	autoNum = (int)mysql_insert_id(&mydata);
	result = mysql_store_result(&mydata);

	rowCount = (int)mysql_num_rows(result);//计算行数
	colCount = (int)mysql_num_fields(result);//计算列数

	if(rowCount != 1 )
	{
	    ReportResult(SQLEXPECTSINGLEROW_FAILURE);
		return NULL;
	}

	string* rowData=new string[colCount];
	MYSQL_ROW line=NULL;
	line=mysql_fetch_row(result);//取第一行结果

	for(int i=0; i<colCount;i++)
	{
		rowData[i]=line[i];
	}

	return rowData;  
}

int CMySQLAPI::ExecuteQueryNum(const char* pSql)  //执行查询数目
{
	if( 0==mysql_query(&mydata,pSql) )
		ReportResult(SQLQUERY_SUCCESS);
	else
		ReportResult(SQLQUERY_FAILURE);

	affectedRow = (int)mysql_affected_rows(&mydata);
	autoNum = (int)mysql_insert_id(&mydata);
	result = mysql_store_result(&mydata);

	rowCount = (int)mysql_num_rows(result);//计算行数
	colCount = (int)mysql_num_fields(result);//计算列数

	return rowCount;
}


void CMySQLAPI::Close()
{
	mysql_close(&mydata);
}