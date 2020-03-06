#include "BaseMysql.h"


BaseMysql::BaseMysql()
{
	m_con = nullptr;
	m_res = nullptr;
	m_time = 0;
}

BaseMysql::~BaseMysql()
{

}

int BaseMysql::connect(const char *ip, const char *user, const char *pswd, const char *dbname, unsigned short port)
{
	MYSQL *con, *t;
	char *query = NULL;
	con = mysql_init((MYSQL*)0);
	int ret;

	if (con != NULL && (t = mysql_real_connect(con, ip, user, pswd, dbname, port, NULL, 0))) {
		if (!mysql_select_db(con, dbname)) {
			con->reconnect = 1;
			
			query = "set names \'UTF8\'";
			ret = mysql_real_query(con, query, strlen(query));
			if (ret) {
				cout << "Error making query: " << mysql_error(con) << endl;
				return false;
			}
			else {
				cout << "query " << query << "succeed!" << endl;
				cout << "connect succeed mysql ==" << ip << ":" << port << endl;
			}
			int reconnct = 1;
			mysql_options(con, MYSQL_OPT_RECONNECT, &reconnct);
		}
		m_con = con;
	}
	else{
		cout << "Error making query: " << mysql_error(con) << endl;
		return false;
	}
	return true;
}

void BaseMysql::Close()
{
	if (m_con){
		mysql_close(m_con);
		m_con = nullptr;
	}
}

void BaseMysql::Query(const char *sql)
{
	mysql_query(m_con, sql);
	m_res = mysql_store_result(m_con);

	/*if (m_res == NULL)
	{

	}
	string str("");
	while (m_row = mysql_fetch_row(m_res))
	{
	for (int i = 0; i < Cnum; i++)
	{
	str += m_row[i];
	str += rg;
	}
	str += rg;
	rnum++;
	}

	mysql_free_result(m_res);*/
}

void BaseMysql::QueryOver()
{
	if (m_res){
		mysql_free_result(m_res);
		m_res = nullptr;
	}
}


BaseMysqlManager *m_baseMysqlManaget = nullptr;

BaseMysqlManager::BaseMysqlManager(string _ip, string _user, string _pswd, string _dbname, unsigned short _port, int len)
	:input({ _ip, _user, _pswd, _dbname }), port(_port)
{
	try{
		for (int i = 0; i < len; ++i){
			mysqlcon[new BaseMysql] = MYSQLIDLE;
		}
	}
	catch(...){

	}
	
}

BaseMysqlManager::~BaseMysqlManager()
{
	for (auto it = mysqlcon.begin(); it != mysqlcon.end();){
		BaseMysql *t = it->first;
		t->Close();
		it = mysqlcon.erase(it);
		if (t){
			delete t;
			t = nullptr;
		}
	}
}

BaseMysqlManager *BaseMysqlManager::GetSingle(){
	if (m_baseMysqlManaget == nullptr){
		m_baseMysqlManaget = new BaseMysqlManager("192.168.0.112","root","LY2328266.+","ccgame",3306,10);
	}
	return m_baseMysqlManaget;
}

BaseMysql * BaseMysqlManager::GetOne()
{
	if (false == canrun) return nullptr;
	lock_guard<mutex> t(conmutex);
	for (auto it = mysqlcon.begin(); it != mysqlcon.end(); ++it)
	{
		if (it->second == MYSQLIDLE){
			return it->first;
		}
	}
	return nullptr;
}

void BaseMysqlManager::ReturnOne(BaseMysql *con)
{
	if (con == nullptr || mysqlcon.find(con) == mysqlcon.end()) return;
	con->QueryOver();
	lock_guard<mutex> t(conmutex);
	mysqlcon[con] = MYSQLIDLE;
}

void BaseMysqlManager::Run()
{
	if (canrun){
		//MYSQL_OPT_RECONNECT
	}
}

void BaseMysqlManager::Init()
{
	int ret = 0;
	for (auto it = mysqlcon.begin(); it != mysqlcon.end();++it)
	{
		(it->first)->connect(input[0].c_str(), input[1].c_str(), input[2].c_str(), input[3].c_str(), port);
	}
	canrun = true;
}

void BaseMysqlManager::Stop()
{
	lock_guard<mutex> t(conmutex);
	canrun = true;
	for (auto it = mysqlcon.begin(); it != mysqlcon.end(); )
	{
		if (it->second == MYSQLIDLE){
			it->second = MYSQLDELETE;
			it->first->Close();
			it = mysqlcon.erase(it);
		}
		else
			++it;
	}
}
