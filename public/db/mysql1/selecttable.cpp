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

int iminid,imaxid;  // 查询条件最小和最大的id。

stmt.prepare("\
      select id,name,weight,date_format(btime,'%%Y-%%m-%%d %%H:%%i:%%s') from girls where id>=:1 and id<=:2");

stmt.bindin(1,&iminid);
stmt.bindin(2,&imaxid);
stmt.bindout(1,&stgirls.id);
stmt.bindout(2, stgirls.name,30);
stmt.bindout(3,&stgirls.weight);
stmt.bindout(4, stgirls.btime,19);

iminid=1;    // 指定待查询记录的最小id的值。
imaxid=3;    // 指定待查询记录的最大id的值。

/*
    注意事项：
    1、如果SQL语句的主体没有改变，只需要prepare()一次就可以了；
    2、结果集中的字段，调用bindout()绑定变量的地址；
    3、bindout()方法的返回值固定为0，不用判断返回值；
    4、如果SQL语句的主体已改变，prepare()后，需重新用bindout()绑定变量；
    5、调用execute()方法执行SQL语句，然后再循环调用next()方法获取结果集中的记录；
    6、每调用一次next()方法，从结果集中获取一条记录，字段内容保存在已绑定的变量中。
*/

if(stmt.execute()!=0)
{
printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
return -1;
}

while(true)
{
 memset(&stgirls,0,sizeof(struct st_girls));

 //从结果集中获取一条记录，需要判断返回值，0-成功，1403-无记录，其他-失败
 if(stmt.next()!=0) break;

 printf("id=%ld,name=%s,weight=%.02f,btime=%s\n",stgirls.id,stgirls.name,stgirls.weight,stgirls.btime);
}

// rpc保存了SQL语句执行后影响的记录数。
printf("本次查询了girls表%ld条记录。\n",stmt.m_cda.rpc);


return 0;
}
