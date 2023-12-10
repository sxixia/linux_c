#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
  if (argc<3)
  {
    printf("Using:./procctl timetvl program argv ...\n");
    printf("Example: 60 /project/idc1/bin/crtsurfdata5 /project/idc1/ini/stcode.ini /tmp/idc/surfdata /log/idc/crtsurfdata5.log xml,json,csv\n\n");

    printf("本程序是服务程序的调度程序，周期性启动服务程序或shell脚本。\n");
    printf("timetvl 运行周期，单位：秒。被调度的程序运行结束后，在timetvl秒后会被procctl重新启动。\n");
    printf("program 被调度的程序名，必须使用全路径。\n");
    printf("argvs   被调度的程序的参数。\n");
    printf("注意，本程序不会被kill杀死，但可以用kill -9强行杀死。\n\n\n");
    return -1;
  }

  // 关闭信号和IO，后台运行。本程序不希望被打扰。
  for (int ii=0;ii<64;ii++)
  {
    signal(ii,SIG_IGN); 
    close(ii);
  }

  // 生成子进程，父进程退出，让程序运行在后台，由系统1号进程托管。
  if (fork()!=0) exit(0);

  // 子进程启用SIGCHLD信号，让父进程可以wait子进程退出的状态。
  signal(SIGCHLD,SIG_DFL);

  char *pargv[argc-1];
  /*pargv[0] = "/project/idc1/bin/crtsurfdata5";
  pargv[1] = "/project/idc1/ini/stcode.ini";
  pargv[2] = "/tmp/idc/surfdata";
  pargv[3] = "/log/idc/crtsurfdata5.log";
  pargv[4] = "xml,json,csv";
  pargv[5] = NULL;*/
  for(int ii=2;ii<argc;ii++)
  {
   pargv[ii-2]=argv[ii];
  }
  pargv[argc-2]=NULL;
  
  while (true)
  {
    if (fork()==0)
    {
       // 子进程使用fork()再次生成一个子进程，第二个子进程通过调用execv()来执行被调度的程序，并传递相应的参数。
      execv(argv[2],pargv);
      exit(0);
    }
    else
    {
        //父进程使用wait()等待子进程退出状态，并获取子进程的退出状态信息。
        //父进程通过调用sleep()来等待一定的时间，以实现调度周期。
      int status;
      wait(&status);
      sleep(atoi(argv[1]));
    }
  }
}
