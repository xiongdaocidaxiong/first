#ifndef __BASEMYSQL__H__
#define __BASEMYSQL__H__
#pragma  once
#include "ComDefine.h"
#include "mysql.h"


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
	BaseMysqlManager(std::string _ip, std::string _user, std::string _pswd, std::string _dbname, unsigned short port, int len);
	~BaseMysqlManager();

	static BaseMysqlManager *GetSingle();
	BaseMysql * GetOne();
	void ReturnOne(BaseMysql *);
	void Run();

	void Init();
	void Stop();
public:

	std::unordered_map<BaseMysql *, int> mysqlcon;
	std::vector<std::string>		input;
	unsigned short		port;
	std::mutex				conmutex;
	int					canrun;
};
extern BaseMysqlManager *m_baseMysqlManaget;
#endif