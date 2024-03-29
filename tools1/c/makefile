# 开发框架头文件路径。
PUBINCL = -I/project/public

# 开发框架cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
PUBCPP = /project/public/_public.cpp

# mysql头文件存放路径
MYSQLINCL = -I/usr/local/mysql/include  -I/project/public/db/mysql

# mysql库文件保存路径
MYSQLLIB = -L/usr/local/mysql/lib

# mysql链接库
MYSQLLIBS = -lmysqlclient

# 开发框架mysql的cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
MYSQLCPP = /project/public/db/mysql/_mysql.cpp

# oracle头文件路径
ORAINCL = -I$(ORACLE_HOME)/rdbms/public -I/project/public/db/oracle

# oracle库文件路径
ORALIB =  -L$(ORACLE_HOME)/lib -L.

# oracle的oci库
ORALIBS = -lclntsh
# 开发框架mysql的cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
ORACPP = /project/public/db/oracle/_ooci.cpp

# 编译参数。
CFLAGS = -g

all: procctl checkproc gzipfiles deletefiles ftpgetfiles ftpputfiles tcpputfiles3 fileserver\
     tcpgetfiles execsql dminingmysql xmltodb syncupdate syncincrement syncincrementex\
     deletetable migratetable xmltodb_oracle deletetable_oracle migratetable_oracle dminingoracle\
     syncupdate_oracle syncincrement_oracle syncincrementex_oracle

procctl:procctl.cpp
	g++ -o procctl procctl.cpp
	cp procctl ../bin/.

checkproc:checkproc.cpp
	g++ $(CFLAGS) -o checkproc checkproc.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp checkproc ../bin/.

gzipfiles:gzipfiles.cpp
	g++ $(CFLAGS) -o gzipfiles gzipfiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp gzipfiles ../bin/.

deletefiles:deletefiles.cpp
	g++ $(CFLAGS) -o deletefiles deletefiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp deletefiles ../bin/.

ftpgetfiles:ftpgetfiles.cpp
	g++ $(CFLAGS) -o ftpgetfiles ftpgetfiles.cpp $(PUBINCL) $(PUBCPP) /project/public/libftp.a /project/public/_ftp.cpp -lm -lc
	cp ftpgetfiles ../bin/.

ftpputfiles:ftpputfiles.cpp
	g++ $(CFLAGS) -o ftpputfiles ftpputfiles.cpp $(PUBINCL) $(PUBCPP) /project/public/libftp.a /project/public/_ftp.cpp -lm -lc
	cp ftpputfiles ../bin/.

tcpputfiles3:tcpputfiles3.cpp
	g++ $(CFLAGS) -o tcpputfiles3 tcpputfiles3.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp tcpputfiles ../bin/.

fileserver:fileserver.cpp
	g++ $(CFLAGS) -o fileserver fileserver.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp fileserver ../bin/.

tcpgetfiles:tcpgetfiles.cpp
	g++ $(CFLAGS) -o tcpgetfiles tcpgetfiles.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp tcpgetfiles ../bin/.

execsql:execsql.cpp
	g++ $(CFLAGS) -o execsql execsql.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp execsql ../bin/.

webserver1:webserver1.cpp
	g++ $(CFLAGS) -o webserver1 webserver1.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc -lpthread
	cp webserver1 ../bin/.

dminingmysql:dminingmysql.cpp
	g++ $(CFLAGS) -o dminingmysql dminingmysql.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp dminingmysql ../bin/.

dminingoracle:dminingoracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o dminingoracle dminingoracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp dminingoracle ../bin/.

xmltodb:xmltodb.cpp _tools.h _tools.cpp
	g++ $(CFLAGS) -o xmltodb xmltodb.cpp _tools.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp xmltodb ../bin/.

xmltodb_oracle:xmltodb_oracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o xmltodb_oracle xmltodb_oracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp xmltodb_oracle ../bin/.

syncupdate:syncupdate.cpp _tools.h _tools.cpp
	g++ $(CFLAGS) -o syncupdate syncupdate.cpp _tools.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp syncupdate ../bin/.

syncupdate_oracle:syncupdate_oracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o syncupdate_oracle syncupdate_oracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp syncupdate_oracle ../bin/.

syncincrement:syncincrement.cpp _tools.h _tools.cpp
	g++ $(CFLAGS) -o syncincrement syncincrement.cpp _tools.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp syncincrement ../bin/.

syncincrement_oracle:syncincrement_oracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o syncincrement_oracle syncincrement_oracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp syncincrement_oracle ../bin/.

syncincrementex:syncincrementex.cpp _tools.h _tools.cpp
	g++ $(CFLAGS) -o syncincrementex syncincrementex.cpp _tools.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp syncincrementex ../bin/.

syncincrementex_oracle:syncincrementex_oracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o syncincrementex_oracle syncincrementex_oracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp syncincrementex_oracle ../bin/.

deletetable:deletetable.cpp
	g++ $(CFLAGS) -o deletetable deletetable.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp deletetable ../bin/.

deletetable_oracle:deletetable_oracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o deletetable_oracle deletetable_oracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp deletetable_oracle ../bin/.

migratetable:migratetable.cpp _tools.h _tools.cpp
	g++ $(CFLAGS) -o migratetable migratetable.cpp _tools.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp migratetable ../bin/.

migratetable_oracle:migratetable_oracle.cpp _tools_oracle.h _tools_oracle.cpp
	g++ $(CFLAGS) -o migratetable_oracle migratetable_oracle.cpp _tools_oracle.cpp $(PUBINCL) $(PUBCPP) $(ORAINCL) $(ORALIB) $(ORALIBS) $(ORACPP) -lm -lc
	cp migratetable_oracle ../bin/.

clean:
	rm -f procctl checkproc gzipfiles deletefiles ftpgetfiles ftpputfiles tcpputfiles3 fileserver
	rm -f tcpgetfiles execsql dminingmysql xmltodb syncupdate syncincrement syncincrementex
	rm -f deletetable migratetable xmltodb_oracle deletetable_oracle migratetable_oracle dminingoracle syncupdate_oracle syncincrement_oracle syncincrementex_oracle
