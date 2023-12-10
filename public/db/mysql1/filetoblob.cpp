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

stmt.prepare("\
	update girls set pic=:1 where id=:2");

stmt.bindinlob(1, stgirls.pic,&stgirls.picsize);
stmt.bindin(2,&stgirls.id);


for(int ii=1;ii<3;ii++)
{
memset(&stgirls,0,sizeof(struct st_girls));
stgirls.id = ii;
//把图片内容加载到pic中
if (ii==1) stgirls.picsize = filetobuf("1.jpg",stgirls.pic);

if (ii==2) stgirls.picsize = filetobuf("2.jpg",stgirls.pic);

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
