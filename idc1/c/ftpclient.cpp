#include "_ftp.h"

Cftp ftp;

int main(int argc,char *argv[])
{
if(ftp.login("192.168.211.131:21","diana","xixisbx")==false)
{ printf("ftp.login(192.168.211.131:21) failed\n"); return -1;}
printf("ftp.login(192.168.211.131:21) ok\n");

if(ftp.mtime("/project/idc1/c/makefile")==false)
{ printf("ftp.mtime(/project/idc1/c/makefile) failed\n"); return -1;}
printf("ftp.mtime(/project/idc1/c/makefile) ok time=%s\n",ftp.m_mtime);

if(ftp.size("/project/idc1/c/makefile")==false)
{ printf("ftp.size(/project/idc1/c/makefile) failed\n"); return -1;}
printf("ftp.size(/project/idc1/c/makefile) ok size=%d\n",ftp.m_size);

if(ftp.nlist("/project/idc1","/project/idc1/c/tmp.lst")==false)
{ printf("ftp.nlist(/project/idc1/c) failed\n"); return -1;}
printf("ftp.nlist(/project/idc1/c) ok\n");
system("cat /project/idc1/c/tmp.lst");

//下载
if(ftp.get("/home/diana/Documents/test.txt","/home/sxixia/Downloads/test1.txt",true)==false)
{ printf("ftp.get() failed\n"); return -1;}
printf("ftp.get() ok\n");

//上传
if(ftp.put("/home/sxixia/Downloads/test2.txt","/home/diana/Downloads/test2.txt",true)==false)
{ printf("ftp.put() failed\n"); return -1;}
printf("ftp.put() ok\n");

ftp.logout();
return 0;
}
