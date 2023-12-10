/*
 * 程序名:crtsurfdata2.cpp 本程序用于生成全国气象站点观测的分钟数据
 * 作者:义哥
 */

#include "_public.h"

CLogFile logfile;

int main(int argc,char* argv[])
{
//第一个参数:全国气象站点参数文件
//第二个:生成的测试数据存放和目录
//第三个：程序运行的日志
//那么argc = 4
 if(argc != 4)
 {
  printf("Using:./crtsurfdata2 inifile outpath logfile\n");
  printf("Example:/project/idc1/bin/crtsufdata2 /project/idc1/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata2.log\n\n");

  printf("inifile：全国气象站点参数文件名。\n");
  printf("outpath：全国气象站点数据文件存放的目录\n");
  printf("logfile：本程序运行的日志文件名\n\n");

  return -1;
 }

 if(logfile.Open(argv[3])==false)
 {
  printf("logfile.Open(%s) failed.\n",argv[3]);
  return -1;
 }

 logfile.Write("crtsurfdata1开始运行。\n");
 //中间放业务代码

 logfile.Write("crtsurfdata1结束运行。\n");

return 0;
}
