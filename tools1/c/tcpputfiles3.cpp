#include "_public.h"

//程序运行的参数结构体
struct st_arg
{
 int clienttype;			// 客户端类型，1-上传文件；2-下载文件。
 char ip[31];			// 服务端的IP地址。
 int port;				// 服务端的端口。
 int ptype;				// 文件上传成功后文件的处理方式：1-删除文件；2-移动到备份目录。
 char clientpath[301];		// 本地文件存放的根目录。
 char clientpathbak[301];	// 文件成功上传后，本地文件备份的根目录，当ptype==2时有效。
 bool andchild;			// 是否上传clientpath目录下各级子目录的文件，true-是；false-否。
 char matchname[301];		// 待上传文件名的匹配规则，如"*.TXT,*.XML"。
 char srvpath[301];		// 服务端文件存放的根目录。
 int timetvl;			// 扫描本地目录文件的时间间隔，单位：秒。注意：不能超过timeout参数
 int timeout;			// 进程心跳的超时时间。
 char pname[51];			// 进程名，建议用"tcpputfiles_后缀"的方式。
}starg;

CLogFile logfile;		//日志类
char strrecvbuffer[1024];//接收缓冲区
char strsendbuffer[1024];//发送缓冲区
CPActive PActive;		//进程心跳类
CTcpClient TcpClient;	//通信类

void EXIT(int sig);				//退出函数
void _help();					//打印帮助信息
bool _xmltoarg(char *strxmlbuffer);	// 把xml解析到参数starg结构中。
bool Login(const char *argv);		//登录业务。
bool ActiveTest();				//心跳函数
bool _tcpputfiles();				//文件上传的主函数
bool SendFile(const int sockfd,const char* filename,const int filesize);


int main(int argc,char *argv[])
{
 if (argc!=3) { _help(); return -1; }
 
 // 关闭全部的信号和输入输出。
 // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程。
 // 但请不要用 "kill -9 +进程号" 强行终止。
 CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);
 
 // 打开日志文件。
 if (logfile.Open(argv[1],"a+")==false)
 {
  printf("打开日志文件失败（%s）。\n",argv[1]); return -1;
 }

 // 解析xml，得到程序运行的参数。
 if(_xmltoarg(argv[2])==false) return -1;

 //把进程的心跳信息写入共享内存
 //PActive.AddPInfo(starg.timeout,starg.pname);

 // 向服务端发起连接请求。
 if (TcpClient.ConnectToServer(starg.ip,starg.port)==false)
 {
  logfile.Write("TcpClient.ConnectToServer(%s,%d) failed.\n",starg.ip,starg.port);
  EXIT(-1);
 }

  // 登录业务。
 if (Login(argv[2])==false)
 {
  logfile.Write("Login() failed.\n");
  EXIT(-1);
 }
 
 while(true)
 {
  //调用文件上传的主函数，执行文件上传的任务
  if(_tcpputfiles()==false)
  { logfile.Write("_tcpputfiles() failed.\n"); EXIT(-1);}

  sleep(starg.timetvl);
  if( ActiveTest()==false) break;
 }

 EXIT(0);
}


// 心跳。 
bool ActiveTest()    
{
 memset(strsendbuffer,0,sizeof(strsendbuffer)); 
 memset(strrecvbuffer,0,sizeof(strrecvbuffer)); 
 
 //向服务端发送请求报文
 SPRINTF(strsendbuffer,sizeof(strsendbuffer),"<activetest>ok</activetest>");
 logfile.Write("客户端发送：%s\n",strsendbuffer);
 if (TcpClient.Write(strsendbuffer)==false) return false;

 // 接收服务端的回应报文。
 if (TcpClient.Read(strrecvbuffer,20)==false) return false;
 logfile.Write("客户端接收：%s\n",strrecvbuffer);

 return true;
}

// 登录业务。 
bool Login(const char *argv)    
{
 memset(strsendbuffer,0,sizeof(strsendbuffer));
 memset(strrecvbuffer,0,sizeof(strrecvbuffer));
 
 //将程序运行的xml参数拼接<clienttype>1</clienttype>以后发送出去
 SPRINTF(strsendbuffer,sizeof(strsendbuffer),"%s<clienttype>1</clienttype>",argv);
 logfile.Write("客户端发送：%s\n",strsendbuffer);
 if (TcpClient.Write(strsendbuffer)==false) return false;
 
  // 接收服务端的回应报文并设置超时时间为20秒。
 if (TcpClient.Read(strrecvbuffer,20)==false) return false;
 logfile.Write("客户端接收：%s\n",strrecvbuffer);

 logfile.Write("登录(%s:%d)成功。\n",starg.ip,starg.port); 

 return true;
}

void EXIT(int sig)
{
 logfile.Write("客户端程序退出，sig= %d\n",sig);
 exit(0);
}

void _help()
{
 printf("\n");
 printf("Using:/project/tools1/bin/tcpputfiles logfilename xmlbuffer\n\n");

 printf("Sample:/project/tools1/bin/procctl 20 /project/tools1/bin/tcpputfiles /log/idc/tcpputfiles_surfdata.log \"<ip>192.168.211.129</ip><port>5005</port><ptype>1</ptype><clientpath>/tmp/tcp/surfdata1</clientpath><clientpathbak>/tmp/tcp/surfdata1bak</clientpathbak><andchild>true</andchild><matchname>*.XML,*.CSV</matchname><srvpath>/tmp/tcp/surfdata2</srvpath><timetvl>10</timetvl><timeout>50</timeout><pname>tcpputfiles_surfdata</pname>\"\n");
 printf("       /project/tools1/bin/procctl 20 /project/tools1/bin/tcpputfiles /log/idc/tcpputfiles_surfdata.log \"<ip>192.168.211.129</ip><port>5005</port><ptype>2</ptype><clientpath>/tmp/tcp/surfdata1</clientpath><clientpathbak>/tmp/tcp/surfdata1bak</clientpathbak><andchild>true</andchild><matchname>*.XML,*.CSV</matchname><srvpath>/tmp/tcp/surfdata2</srvpath><timetvl>10</timetvl><timeout>50</timeout><pname>tcpputfiles_surfdata</pname>\"\n\n\n");

 printf("本程序是数据中心的公共功能模块，采用tcp协议把文件发送给服务端。\n");
 printf("logfilename   本程序运行的日志文件。\n");
 printf("xmlbuffer     本程序运行的参数，如下：\n");
 printf("ip            服务端的IP地址。\n");
 printf("port          服务端的端口。\n");
 printf("ptype         文件上传成功后的处理方式：1-删除文件；2-移动到备份目录。\n");
 printf("clientpath    本地文件存放的根目录。\n");
 printf("clientpathbak 文件成功上传后，本地文件备份的根目录，当ptype==2时有效。\n");
 printf("andchild      是否上传clientpath目录下各级子目录的文件，true-是；false-否，缺省为false。\n");
 printf("matchname     待上传文件名的匹配规则，如\"*.TXT,*.XML\"\n");
 printf("srvpath       服务端文件存放的根目录。\n");
 printf("timetvl       扫描本地目录文件的时间间隔，单位：秒，取值在1-30之间。\n");
 printf("timeout       本程序的超时时间，单位：秒，视文件大小和网络带宽而定，建议设置50以上。\n");
 printf("pname         进程名，尽可能采用易懂的、与其它进程不同的名称，方便故障排查。\n\n");
}

bool _xmltoarg(char *strxmlbuffer)
{
 memset(&starg,0,sizeof(struct st_arg));
 
 GetXMLBuffer(strxmlbuffer,"ip",starg.ip);
 if (strlen(starg.ip)==0) { logfile.Write("ip is null.\n"); return false;}
 
 GetXMLBuffer(strxmlbuffer,"port",&starg.port);
 if ( starg.port==0) { logfile.Write("port is null.\n"); return false; }

 GetXMLBuffer(strxmlbuffer,"ptype",&starg.ptype);
 if ((starg.ptype!=1)&&(starg.ptype!=2)) { logfile.Write("ptype not in (1,2).\n"); return false; }

 GetXMLBuffer(strxmlbuffer,"clientpath",starg.clientpath);
 if (strlen(starg.clientpath)==0) { logfile.Write("clientpath is null.\n"); return false; }

 GetXMLBuffer(strxmlbuffer,"clientpathbak",starg.clientpathbak);
 if ((starg.ptype==2)&&(strlen(starg.clientpathbak)==0)) { logfile.Write("clientpathbak is null.\n"); return false; }

 GetXMLBuffer(strxmlbuffer,"andchild",&starg.andchild);

 GetXMLBuffer(strxmlbuffer,"matchname",starg.matchname);
 if (strlen(starg.matchname)==0) { logfile.Write("matchname is null.\n"); return false; }

 GetXMLBuffer(strxmlbuffer,"srvpath",starg.srvpath);
 if (strlen(starg.srvpath)==0) { logfile.Write("srvpath is null.\n"); return false; }

 GetXMLBuffer(strxmlbuffer,"timetvl",&starg.timetvl);
 if (starg.timetvl==0) { logfile.Write("timetvl is null.\n"); return false; }

 //如果用户传入的扫描上传时间参数太长，则重新设定。
 if(starg.timetvl>30) starg.timetvl=30;

 // 进程心跳的超时时间，一定要大于starg.timetvl，没有必要小于50秒。
 GetXMLBuffer(strxmlbuffer,"timeout",&starg.timeout);
 if (starg.timeout==0) { logfile.Write("timeout is null.\n"); return false; }
 if (starg.timeout<50) starg.timeout=50;

 GetXMLBuffer(strxmlbuffer,"pname",starg.pname,50);
 if (strlen(starg.pname)==0) { logfile.Write("pname is null.\n"); return false; }
 
 return true;
}

bool _tcpputfiles()
{
 CDir Dir;
 //调用OpenDir()打开starg.clientpath目录
 if(Dir.OpenDir(starg.clientpath,starg.matchname,10000,starg.andchild)==false)
 {
  logfile.Write("Dir.OpenDir(%s) failed.\n",starg.clientpath);
  return false;
 }

 int delayed=0;		//未收到对端确认报文的文件数量。
 int buflen=0;		//用于存放strrecvbuffer的长度

 while(true)
 {
  memset(strsendbuffer,0,sizeof(strsendbuffer));
  memset(strrecvbuffer,0,sizeof(strrecvbuffer));

  //遍历目录中的每一个文件，调用ReadDir()获取一个文件名
  if(Dir.ReadDir()==false) break;

  //把文件名，修改时间，文件大小组成报文，发送给服务端
  SNPRINTF(strsendbuffer,sizeof(strsendbuffer),1000,"<filename>%s</filename><mtime>%s</mtime><size>%d</size>",Dir.m_FullFileName,Dir.m_ModifyTime,Dir.m_FileSize);

  //logfile.Write("strsendbuffer=%s\n",strsendbuffer);
  if(TcpClient.Write(strsendbuffer)==false)
  {
   logfile.Write("_tcpputfiles(): TcpClient.Write() failed.\n"); 
   return false;
  }

  //把文件内容发送给服务端
  logfile.Write("send %s(%d) ...",Dir.m_FullFileName,Dir.m_FileSize);
  if(SendFile(TcpClient.m_connfd,Dir.m_FullFileName,Dir.m_FileSize)==true)
  {
   logfile.WriteEx("ok.\n"); delayed++;
  }
  else
  {
   logfile.WriteEx("failed.\n"); TcpClient.Close(); return false;
  }

  //接收服务端的确认报文
 while (delayed>0)
 {
  memset(strrecvbuffer,0,sizeof(strrecvbuffer));
  if (TcpRead(TcpClient.m_connfd,strrecvbuffer,&buflen,-1)==false) break;
  //logfile.Write("strrecvbuffer=%s\n",strrecvbuffer);

   //删除或者转存本地的文件。
  delayed--;
 }
  logfile.Write("strrecvbuffer=%s\n",strrecvbuffer);
 }
 return true;
}

bool SendFile(const int sockfd,const char* filename,const int filesize)
{
 int onread=0;		//每次调用fread时打算读取的字节数
 int bytes=0;		//调用一次fread从文件中读取的字节数
 char buffer[1000];	//存放读取数据的缓冲区
 int totalbytes=0;	//从文件中已经读取的字节总数
 FILE *fp=NULL;		//文件操作指针
 //以"rb"模式打开文件
 if((fp=fopen(filename,"rb"))==NULL) return false;

 while(true)
 {
  memset(buffer,0,sizeof(buffer));
  //首先计算本次应该读取的字节数，如果剩余的数据超过1000字节，就先读取1000字节
  onread=(filesize-totalbytes) >1000 ? 1000:(filesize-totalbytes); 

  //从文件中读取数据
  bytes=fread(buffer,1,onread,fp);

  //把读取到的数据发送给服务端
  if(bytes>0)
  {
   if(Writen(sockfd,buffer,bytes)==false) { fclose(fp); return false;}
  }

  //计算文件已读取的字节数，如果已经读完，跳出循环
  totalbytes+=bytes;
  if(totalbytes==filesize) break;
 } 
 fclose(fp);
 return true;
}
