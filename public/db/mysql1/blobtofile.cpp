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
long id;			//编号
char pic[2000000];		//图片内容
unsigned long picsize;	//图片内容占用字节数
}stgirls;

sqlstatement stmt(&conn);

stmt.prepare("select id,pic from girls where id in(1,2)");

stmt.bindout(1,&stgirls.id);
stmt.bindoutlob(2,stgirls.pic,2000000,&stgirls.picsize);

if(stmt.execute()!=0)
{
printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
return -1;
}

while(true)
{
memset(&stgirls,0,sizeof(struct st_girls));
//从结果集中获取记录
if(stmt.next()!=0) break;

//生成文件名
char filename[101];	memset(filename,0,sizeof(filename));
sprintf(filename,"%d_out.jpg",stgirls.id);

//把内容写入文件
buftofile(filename,stgirls.pic,stgirls.picsize);

}

printf("成功查询了%ld条记录。\n",stmt.m_cda.rpc);

printf("select table grils ok.\n");

return 0;
}
