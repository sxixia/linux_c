#include "_public.h"
/*此程序为进程的心跳系统
 *服务程序创建时向一块共享内存写入自己的心跳信息
 *每隔多少秒更新一次自己的心跳信息
 *当超时未更新时，由心跳程序负责杀死服务程序，并由调度程序负责重启
 */
void EXIT(int sig)
{
printf("sig=%d\n",sig);
exit(0);
}

CPActive PActive;

int main(int argc,char *argv[])
{
if (argc!=3) { printf("using: procname updatetime\n");printf("example: ./book aaa 20"); return 0; }

signal(2,EXIT);	signal(15,EXIT);	//设置信号2和15的处理函数

PActive.AddPInfo(atoi(argv[2]),argv[1]);
while(1)
{
PActive.UptATime();
sleep(atoi(argv[2]));
}
return 0;
}

