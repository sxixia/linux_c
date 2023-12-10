#include "_mysql.h"

int main(int argc,char *argv[])
{
connection conn;

if(conn.connecttodb("127.0.0.1,root,root,ltbo,3306","utf8")!=0)
{
printf("failed.\n%s\n",conn.m_cda.message);
return -1;
}

sqlstatement stmt(&conn);

int iminid,imaxid;  // 查询条件最小和最大的id。

stmt.prepare("delete from girls where id>=:1 and id<=:2");

stmt.bindin(1,&iminid);
stmt.bindin(2,&imaxid);

iminid=1;    // 指定待查询记录的最小id的值。
imaxid=3;    // 指定待查询记录的最大id的值。


if(stmt.execute()!=0)
{
printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
return -1;
}

printf("本次删除了girls表%ld条记录。\n",stmt.m_cda.rpc);

conn.commit();

return 0;
}
