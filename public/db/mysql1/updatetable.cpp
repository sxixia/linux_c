#include "_mysql.h"

int main(int argc,char *argv[])
{
connection conn;

if(conn.connecttodb("127.0.0.1,root,root,ltbo,3306","utf8")!=0)
{
printf("failed.\n%s\n",conn.m_cda.message);
return -1;
}

struct st_girls
{
long id;
char name[31];
double weight;
char btime[20];
}stgirls;

sqlstatement stmt(&conn);

//绑定数据库连接无参构造，上一句为调用有参构造。
//sqlstatement stmt;
//stmt.connect(&conn);


stmt.prepare("\
	update girls set name=:1,weight=:2,btime=str_to_date(:3,'%%Y-%%m-%%d %%H:%%i:%%s') where id=:4");

stmt.bindin(1, stgirls.name,30);
stmt.bindin(2,&stgirls.weight);
stmt.bindin(3, stgirls.btime,19);
stmt.bindin(4,&stgirls.id);


for(int ii=0;ii<5;ii++)
{
memset(&stgirls,0,sizeof(struct st_girls));
stgirls.id = ii+1;
sprintf(stgirls.name,"皮卡丘%05dgril",ii+1);
stgirls.weight = 30.25+ii;
sprintf(stgirls.btime,"2023-05-22 09:10:%02d",ii);

if(stmt.execute()!=0)
{
printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
return -1;
}

printf("成功修改了%ld条记录。\n",stmt.m_cda.rpc);
}

printf("update table grils ok.\n");

conn.commit();//提交

return 0;
}
