#include "idcapp.h"

CLogFile logfile;
connection conn;
CPActive PActive;

// 把站点参数文件中加载到vstcode容器中。
void EXIT(int sig);

void _help();

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

// 业务处理主函数。
bool _obtmindtodb(char *pathname,char *connstr,char *charset)
{
 //如果需要增加对json和csv格式的数据文件的支持怎么办
 //如果表的字段有很多，例如50个，怎么使代码简介

 CDir Dir;

 // 打开目录。
 if (Dir.OpenDir(pathname,"*.xml,*.csv")==false)
 {
  logfile.Write("Dir.OpenDir(%s) failed.\n",pathname); return false;
 }

 CFile File;

 CZHOBTMIND ZHOBTMIND(&conn,&logfile);

 int  totalcount=0;  // 文件的总记录数。
 int  insertcount=0; // 成功插入记录数。
 CTimer Timer;       // 计时器，记录每个数据文件的处理耗时。
 bool bisxml=false;   // 处理的文件格式，true--xml,false--csv。

 while (true)
 {
  // 读取目录，得到一个数据文件名。
  if (Dir.ReadDir()==false) break;

  if (MatchStr(Dir.m_FullFileName,"*.xml")==true) bisxml=true;
  else bisxml=false;

  // 连接数据库。
  if (conn.m_state==0)
  {
   if (conn.connecttodb(connstr,charset)!=0)
   {
    logfile.Write("connect database(%s) failed.\n%s\n",connstr,conn.m_cda.message);
    return -1;
   }
    
   logfile.Write("connect database(%s) ok.\n",connstr);
  }

  totalcount=insertcount=0;

  // 打开文件。
  if (File.Open(Dir.m_FullFileName,"r")==false)
  {
   logfile.Write("File.Open(%s) failed.\n",Dir.m_FullFileName); return false;
  }

  char strBuffer[1001];   // 存放从文件中读取的一行。

  while (true)
  {
   if (bisxml==true)
    {
    if (File.FFGETS(strBuffer,1000,"<endl/>")==false) break;
    }
   else
   {
    if (File.Fgets(strBuffer,1000,true)==false) break;
    if (strstr(strBuffer,"站点")!=0) continue;
   }
   // 处理文件中的每一行。
   totalcount++;
   
   ZHOBTMIND.SplitBuffer(strBuffer,bisxml);
   
   if(ZHOBTMIND.InsertTable()==true) insertcount++;
  }

  //File.CloseAndRemove();
  conn.commit();
  logfile.Write("已处理文件%s (totalcount=%d,insertcount=%d),耗时%.2f秒。\n",Dir.m_FullFileName,totalcount,insertcount,Timer.Elapsed());
 }

  return true;
}

