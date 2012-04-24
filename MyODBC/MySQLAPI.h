#pragma once
#include <sys/socket.h>
//#include "iostream.h"
#include <string>
#include "mysql/mysql.h"
#include <string>
#include <iostream>
#include <vector>
//#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "../Debug/libmysql.lib")
using namespace std;

typedef enum
{
	SQLCONNECT_SUCCESS = 0,
	SQLCONNECT_FAILURE,
	SQLQUERY_SUCCESS,
	SQLQUERY_FAILURE,
	SQLEXPECTSINGLEROW_FAILURE,
} ENUM_SQLREPORTTYPE;

class   CMySQLAPI
{
private:
	MYSQL mydata;                      //MySQL对象，必备的一个数据结构
	MYSQL_RES *result;                 //用于存放结果 建议用char* 数组将此结果转存
public:
	char* pServer;                     //服务器名
	char* pUName;                      //数据库账号
	char* pUPassword;                  //密码
	char* pDSN;                        //数据库名&数据源名
	unsigned int pPort;                //端口号
	int   rowCount;                    //查询结果的行数
	int   colCount;                    //查询结果的列数
	int   autoNum;                     //新增的值
	int   errorNum;                    //错误代号
	const char* errorInfo;             //错误提示
	int   affectedRow;                 //执行影响的行数

public:
	CMySQLAPI(void);
	~CMySQLAPI(void);
	//初始化
	void Init();
	//开启连接数据库
	bool Open();
	//关闭数据库
	void Close();
	//查询多条结果
	vector<string*> ExecuteQueryVector(const char* pSql);
	//查询一条结果
	string* ExecuteSingleQuery(const char* pSql);
	//执行查询数目
	int ExecuteQueryNum(const char* pSql);
	//执行非查询(更新或删除)
	void ExecuteNonQuery(const char* pSql);
	//报告执行结果
	void ReportResult(ENUM_SQLREPORTTYPE flag);
	//报告错误类型
    void CheckError();




};

