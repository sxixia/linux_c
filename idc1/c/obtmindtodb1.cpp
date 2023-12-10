#include "_public.h"
#include "_mysql.h"

CLogFile logfile;
connection conn;
CPActive PActive;

void _help();
// 把站点参数文件中加载到vstcode容器中。
void EXIT(int sig);

//业务处理主函数
bool _obtmindtodb(char *pathname,char *connstr,char *charset);

int main(int argc,char *argv[])
{
// 帮助文档
 if (argc!=5) { _help();  return -1; }

// 处理程序退出的信号
// CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

// 打开日志文件
 if(logfile.Open(argv[4],"a+")==false)
 {
  printf("打开日志文件失败（%s）。\n",argv[4]);
  return -1;
 }

 PActive.AddPInfo(5000,"obtmindtodb");   // 进程的心跳，10秒足够。
 
 _obtmindtodb(argv[1],argv[2],argv[3]);

/*
// 连接数据库
 if(conn.connecttodb(argv[2],argv[3])!=0)
 {
  logfile.Write("connect database(%s) failed.\n%s\n",argv[2],conn.m_cda.message);
  return -1;
 }

 logfile.Write("connect database(%s) ok.\n",argv[2]);

//提交事务
 conn.commit();
 */

return 0;
}

void _help()
{
    printf("\n");
    printf("Using:./obtmindtodb pathname connstr charset logfile\n");

    printf("Example:/project/tools1/bin/procctl 10 /project/idc1/bin/obtmindtodb /idcdata/surfdata \"127.0.0.1,root,root,ltbo,3306\" utf8 /log/idc/obtmindtodb.log\n\n");

    printf("本程序用于把全国站点分钟观测数据保存到数据库的T_ZHOBTMIND表中，数据只插入，不更新。\n");
    printf("pathname 全国站点分钟观测数据文件存放的目录。\n");
    printf("connstr  数据库连接参数：ip,username,password,dbname,port\n");
    printf("charset  数据库的字符集。\n");
    printf("logfile  本程序运行的日志文件名。\n");
    printf("程序每10秒运行一次，由procctl调度。\n\n\n");
}


void EXIT(int sig)
{
  logfile.Write("程序退出，sig=%d\n\n",sig);

  conn.disconnect();

  exit(0);
}

bool _obtmindtodb(char *pathname,char *connstr,char *charset)
{
 CDir Dir;
 //打开目录
 if(Dir.OpenDir(pathname,"*.xml")==false)
 {
  logfile.Write("Dir.OpenDir(%s) failed.\n",pathname);
  return false;
 } 

 //打开文件
 while(true)
 {
  //读取目录，得到一个数据文件名
  if(Dir.ReadDir()==false) break;
  logfile.Write("filename=%s\n",Dir.m_FullFileName);
  
  //处理文件中的每一行
  /*
  while(true)
  {

  }
  */
 }

 //删除文件，提交事务

 return true;
}
