#ifndef __BASEMYSQL__H__
#define __BASEMYSQL__H__

#include <vector>
#include <string>
#include <list>
#include <mutex>
#include <unordered_map>
#include <iostream>

#include "mysql.h"
using namespace std;




enum  MYSQLSTATE{
	MYSQLIDLE,
	MYSQLINUSR,
	MYSQLDELETE
};
class BaseMysql{
public:
	BaseMysql();
	~BaseMysql();
	int connect(const char *ip, const char *user, const char *pswd, const char *dbname, unsigned short port);
	void Close();
	void Query(const char *sql);
	void QueryOver();
public:
	MYSQL *m_con;
	MYSQL_RES *m_res;
	unsigned long m_time;
};

class BaseMysqlManager{
public:
	BaseMysqlManager(string _ip, string _user, string _pswd, string _dbname, unsigned short port,int len);
	~BaseMysqlManager();

	static BaseMysqlManager *GetSingle();
	BaseMysql * GetOne();
	void ReturnOne(BaseMysql *);
	void Run();

	void Init();
	void Stop();
public:

	unordered_map<BaseMysql *, int> mysqlcon;
	vector<string>		input;
	unsigned short		port;
	mutex				conmutex;
	int					canrun;
};
extern BaseMysqlManager *m_baseMysqlManaget;
#endif