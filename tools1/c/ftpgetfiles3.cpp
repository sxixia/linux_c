#include "_ftp.h"

//程序运行参数的结构体
struct st_arg
{
  char host[31];           // 远程服务器的IP和端口。
  int  mode;               // 传输模式，1-被动模式，2-主动模式，缺省采用被动模式。
  char username[31];       // 远程服务器ftp的用户名。
  char password[31];       // 远程服务器ftp的密码。
  char remotepath[301];    // 远程服务器存放文件的目录。
  char localpath[301];     // 本地文件存放的目录。
  char matchname[101];     // 待下载文件匹配的规则。
  char listfilename[301];  // 下载前列出服务器文件名的文件。
  int  ptype;              // 下载后服务器文件的处理方式：1-什么也不做；2-删除；3-备份。
  char remotepathbak[301]; // 下载后服务器文件的备份目录。
} starg;

//文件信息的结构体
struct st_fileinfo
{
  char filename[301];	//文件名
  char mtine[21];		//文件时间
};

vector<struct st_fileinfo> vlistfile;    // 存放下载前列出服务器文件名的容器。
CLogFile logfile;
Cftp ftp;

void EXIT(int sig);				//退出处理
void _help();					//帮助文档
bool _xmltoarg(char *strxmlbuffer);	//解析参数
bool LoadListFile();				// 把ftp.nlist()方法获取到的list文件加载到容器vlistfile中。
bool _ftpgetfiles();				//ftp下载文件主要函数

int main(int argc,char *argv[])
{
 if(argc!=3)
 {
  _help();
 }

 //处理程序退出的信号
 //关闭全部的信号和输入输出。
 // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程。
 // 但请不要用 "kill -9 +进程号" 强行终止。
 // CloseIOAndSignal(); 
 signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

 //打开日志文件
 if (logfile.Open(argv[1],"a+")==false)
 {
  printf("打开日志文件失败（%s）。\n",argv[1]); return -1;
 }

 //解析xml文件,得到文件运行的参数
 if (_xmltoarg(argv[2])==false) return -1;

 //登陆ftp服务器
 if(ftp.login(starg.host,starg.username,starg.password,starg.mode)==false)
 { logfile.Write("ftp.login(%s,%s,%s) failed\n",starg.host,starg.username,starg.password); return -1;}
 logfile.Write("ftp.login ok\n");

 _ftpgetfiles();

 ftp.logout();
 return 0;
}

void EXIT(int sig)
{
  printf("程序退出，sig=%d\n\n",sig);

  exit(0);
}

void _help()
{
  printf("\n");
  printf("Using:/project/tools1/bin/ftpgetfiles logfilename xmlbuffer\n\n");

  printf("Sample:/project/tools1/bin/procctl 30 /project/tools1/bin/ftpgetfiles /log/idc/ftpgetfiles_surfdata.log \"<host>192.168.211.131:21</host><mode>1</mode><username>diana</username><password>xixisbx</password><localpath>/idcdata/surfdata</localpath><remotepath>/tmp/idc/surfdata</remotepath><matchname>SURF_ZH*.XML,SURF_ZH*.CSV</matchname><listfilename>/idcdata/ftplist/ftpgetfiles_surfdata.list</listfilename><ptype>3</ptype><remotepathbak>/tmp/idc/surfdatabak</remotepathbak>\"\n\n\n");

  printf("本程序是通用的功能模块，用于把远程ftp服务器的文件下载到本地目录。\n");
  printf("logfilename是本程序运行的日志文件。\n");
  printf("xmlbuffer为文件下载的参数，如下：\n");
  printf("<host>192.168.211.131:21</host> 远程服务器的IP和端口。\n");
  printf("<mode>1</mode> 传输模式，1-被动模式，2-主动模式，缺省采用被动模式。\n");
  printf("<username>diana</username> 远程服务器ftp的用户名。\n");
  printf("<password>xixisbx</password> 远程服务器ftp的密码。\n");
  printf("<remotepath>/tmp/idc/surfdata</remotepath> 远程服务器存放文件的目录。\n");
  printf("<localpath>/idcdata/surfdata</localpath> 本地文件存放的目录。\n");
  printf("<matchname>SURF_ZH*.XML,SURF_ZH*.CSV</matchname> 待下载文件匹配的规则。"\
         "不匹配的文件不会被下载，本字段尽可能设置精确，不建议用*匹配全部的文件。\n");
  printf("<listfilename>/idcdata/ftplist/ftpgetfiles_surfdata.list</listfilename> 下载前列出服务器文件名的文件。\n");
  printf("<ptype>1</ptype> 文件下载成功后，远程服务器文件的处理方式：1-什么也不做；2-删除；3-备份，如果为3，还要指定备份的目录。\n");
  printf("<remotepathbak>/tmp/idc/surfdatabak</remotepathbak> 文件下载成功后，服务器文件的备份目录，此参数只有当ptype=3时才有效。\n\n\n");
}

// 把xml解析到参数starg结构中。
bool _xmltoarg(char *strxmlbuffer)
{
  memset(&starg,0,sizeof(struct st_arg));

  GetXMLBuffer(strxmlbuffer,"host",starg.host,30);   // 远程服务器的IP和端口。
  if (strlen(starg.host)==0)
  { logfile.Write("host is null.\n");  return false; }

  GetXMLBuffer(strxmlbuffer,"mode",&starg.mode);   // 传输模式，1-被动模式，2-主动模式，缺省采用被动模式。
  if (starg.mode!=2)  starg.mode=1;

  GetXMLBuffer(strxmlbuffer,"username",starg.username,30);   // 远程服务器ftp的用户名。
  if (strlen(starg.username)==0)
  { logfile.Write("username is null.\n");  return false; }

  GetXMLBuffer(strxmlbuffer,"password",starg.password,30);   // 远程服务器ftp的密码。
  if (strlen(starg.password)==0)
  { logfile.Write("password is null.\n");  return false; }

  GetXMLBuffer(strxmlbuffer,"remotepath",starg.remotepath,300);   // 远程服务器存放文件的目录。
  if (strlen(starg.remotepath)==0)
  { logfile.Write("remotepath is null.\n");  return false; }

  GetXMLBuffer(strxmlbuffer,"localpath",starg.localpath,300);   // 本地文件存放的目录。
  if (strlen(starg.localpath)==0)
  { logfile.Write("localpath is null.\n");  return false; }

  GetXMLBuffer(strxmlbuffer,"matchname",starg.matchname,100);   // 待下载文件匹配的规则。
  if (strlen(starg.matchname)==0)
  { logfile.Write("matchname is null.\n");  return false; }

  GetXMLBuffer(strxmlbuffer,"listfilename",starg.listfilename,300);   // 下载前列出服务器文件名的文件。
  if (strlen(starg.listfilename)==0)
  { logfile.Write("listfilename is null.\n");  return false; }

  // 下载后服务器文件的处理方式：1-什么也不做；2-删除；3-备份。
  GetXMLBuffer(strxmlbuffer,"ptype",&starg.ptype);   
  if ( (starg.ptype!=1) && (starg.ptype!=2) && (starg.ptype!=3) )
  { logfile.Write("ptype is error.\n"); return false; }

  GetXMLBuffer(strxmlbuffer,"remotepathbak",starg.remotepathbak,300); // 下载后服务器文件的备份目录。
  if ( (starg.ptype==3) && (strlen(starg.remotepathbak)==0) )
  { logfile.Write("remotepathbak is null.\n");  return false; }

  return true;
}

bool _ftpgetfiles()
{
 //进入ftp服务器存放文件的目录
 if(ftp.chdir(starg.remotepath)==false)
 {
  logfile.Write("ftp.chdir(%s) failed\n",starg.remotepath); return false; 
 }
 //调用ftp.nlist方法列出服务器目录中的文件，结果存放到本地文件中
 if(ftp.nlist(".",starg.listfilename)==false)
 {
  logfile.Write("ftp.nlist(%S) failed\n",starg.listfilename); return false;
 }
 // 把ftp.nlist()方法获取到的list文件加载到容器vlistfile中。
 if (LoadListFile()==false)
 {
  logfile.Write("LoadListFile() failed.\n");  return false;
 }
  
 char strremotefilename[301],strlocalfilename[301];
  //遍历容器
 for(int ii=0;ii<vlistfile.size();ii++)
  {
   SNPRINTF(strremotefilename,sizeof(strremotefilename),300,"%s/%s",starg.remotepath,vlistfile[ii].filename);
   SNPRINTF(strlocalfilename,sizeof(strlocalfilename),300,"%s/%s",starg.localpath,vlistfile[ii].filename);
     // 调用ftp.get()方法从服务器下载文件。
   logfile.Write("get %s ...",strremotefilename);

   if (ftp.get(strremotefilename,strlocalfilename)==false) 
    {
    logfile.WriteEx("failed.\n"); break;
    }

   logfile.WriteEx("ok.\n"); 

   // 删除文件。
   if (starg.ptype==2) 
    {
     if (ftp.ftpdelete(strremotefilename)==false)
      {
       logfile.Write("ftp.ftpdelete(%s) failed.\n",strremotefilename); return false;
      }
    }
  // 转存到备份目录。
   if (starg.ptype==3) 
    {
     char strremotefilenamebak[301];
     SNPRINTF(strremotefilenamebak,sizeof(strremotefilenamebak),300,"%s/%s",starg.remotepathbak,vlistfile[ii].filename);
     if (ftp.ftprename(strremotefilename,strremotefilenamebak)==false)
      {
       logfile.Write("ftp.ftprename(%s,%s) failed.\n",strremotefilename,strremotefilenamebak);  return false;
      }
    }
  }
  
 return true;
}

bool LoadListFile()
{
 vlistfile.clear();
 CFile File;
 if(File.Open(starg.listfilename,"r")==false)
 {
  logfile.Write("File.Open(%s) failed",starg.listfilename); return false;
 }
 
 struct st_fileinfo stfileinfo;
 while(true)
 {
  memset(&stfileinfo,0,sizeof(struct st_fileinfo));
  if(File.Fgets(stfileinfo.filename,300,true)==false) break;

  if(MatchStr(stfileinfo.filename,starg.matchname)==false) continue;

  vlistfile.push_back(stfileinfo);
 }
 
 /*for(int ii=0;ii<vlistfile.size();ii++)
 {
  logfile.Write("filename=%s=\n",vlistfile[ii].filename);
 }*/

 return true;
}


