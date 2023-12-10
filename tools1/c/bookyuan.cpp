#include "_public.h"

/*此程序为进程的心跳系统
 *服务程序创建时向一块共享内存写入自己的心跳信息
 *每隔多少秒更新一次自己的心跳信息
 *当超时未更新时，由心跳程序负责杀死服务程序，并由调度程序负责重启
 */

#define MAXNUMP_ 1000    //最大的进程数量
#define SHMKEYP_  0x5095  //共享内存的key
#define SEMKEYP_  0x5095	 //信号量的key
//进程心跳信息的结构体
struct st_pinfo
{
 int pid;        //进程的pid
 char pname[51]; //进程的名字，可以为空，为了方便
 int timeout;    //超时时间，单位为秒
 time_t atime;   //最后一次心跳时间，整数表示
};

class PActive
{
private:
	CSEM m_sem;		//给共享内存加锁的信号量
	int m_shmid;	//共享内存的id
	int m_pos;		//当前进程在共享内存组中的位置
	struct st_pinfo *m_shm;	//指向共享内存的结构体指针
public:
	PActive();
	bool AddPInfo(const int timeout,const char *pname);
	bool UptATime();
	~PActive();
};	

int main(int argc,char *argv[])
{
if (argc!=3) { printf("using: procname updatetime\n");printf("example: ./book aaa 20"); return 0; }

PActive Active;
Active.AddPInfo(30,argv[1]);
while(1)
{
Active.UptATime();
sleep(10);
}
return 0;
}

PActive::PActive()
{
 m_shmid=-1;//共享内存的id
 m_pos=-1;	//当前进程在共享内存组中的位置
 m_shm=0;	//结构体指针
}

//添加心跳信息
bool PActive::AddPInfo(const int timeout,const char *pname)
{
//如果已经找到位置，不需要再找
if(m_pos!=-1) return true;
//创建/获取共享内存，大小为n*sizeof(struct st_pinfo) 键值为16进制
if((m_shmid=shmget(SHMKEYP_,MAXNUMP_*sizeof(struct st_pinfo),0640|IPC_CREAT))==-1)
{ printf("shmget(%x) failed\n",SHMKEYP_);  return false; }

//创建信号量,如果存在则获取信号量，如果不存在，则创建信号量并初始化为value
if(m_sem.init(SEMKEYP_)==false)
 { printf("m_sem.init(%x) failed\n",SEMKEYP_);  return false; }

//共享内存连接到当前进程的地址空间
//struct st_pinfo *m_shm;
m_shm=(struct st_pinfo *)shmat(m_shmid,0,0);

//创建当前进程心跳信息结构体变量，把本进程的信息填进去
struct st_pinfo stpinfo;
memset(&stpinfo,0,sizeof(struct st_pinfo));
stpinfo.pid=getpid();		//进程id
STRNCPY(stpinfo.pname,sizeof(stpinfo.pname),pname,50);	//进程名字
stpinfo.timeout=timeout;	//超时时间
stpinfo.atime=time(0);	//最后一次心跳时间，起始为当前时间

//在共享内存中查找一个空位置，把当前进程的心跳信息存入共享内存中
/*进程的id是循环使用的，如果曾经有一个进程异常退出，没有清理掉自己的心跳信息
*它的进程信息将残留到共享内存中，如果当前进程重用了上述进程的id
*那么共享内存中存在两个进程id相同的纪录，守护进程检查到残留进程的心跳时
*会向进程id发送杀死信号，这个信号将误杀当前进程
*先进行循环遍历，如果共享内存中存在当前进程的编号，那么就是残留
*/
for(int ii=0;ii<MAXNUMP_;ii++)
 if(m_shm[ii].pid==stpinfo.pid) { m_pos=ii; break; }

m_sem.P();	//加锁

if(m_pos=-1)
{
 for(int ii=0;ii<MAXNUMP_;ii++)
 {
  //if((m_shm+ii)->pid==0)
  if(m_shm[ii].pid==0)
  {
   //找到了一个空位置
   m_pos=ii; break;
  }
 }
}

if(m_pos==-1)
{
 m_sem.V();	//解锁，异常退出之前需要进行解锁
 printf("共享内存已用完\n"); return false;
}

//把当前进程的心跳信息存入共享内存的进程组中
memcpy(m_shm+m_pos,&stpinfo,sizeof(struct st_pinfo));

m_sem.V();	//解锁

return true;
}

//更新共享内存中本进程的心跳时间
bool PActive::UptATime()
{
if(m_pos==-1) return false;

m_shm[m_pos].atime=time(0);
return true;
}

PActive::~PActive()
{
//把当前进程从共享内存中删除
if(m_pos!=-1) memset(m_shm+m_pos,0,sizeof(struct st_pinfo));

//把共享内存从当前内存中分离
if(m_shm!=0) shmdt(m_shm);
}





