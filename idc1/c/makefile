#开发框架头文件路径
PUBINCL = -I/project/public

#mysql头文件存放的目录
MYSQLINCL = -I/usr/local/mysql/include -I/project/public/db/mysql

#mysql库文件存放目录
MYSQLLIB = -L/usr/local/mysql/lib

#开发框架cpp文件，直接包含进来，没有采用链接库，是为了方便调试
PUBCPP = /project/public/_public.cpp

#开发框架mysql的cpp文件，直接包含进来，没有采用链接库，是为了方便调试
MYSQLCPP = /project/public/db/mysql/_mysql.cpp

#需要链接的mysql库
MYSQLLIBS = -lmysqlclient

CFLAGS=-g -Wno-write-strings

all:crtsurfdata1 crtsurfdata2 crtsurfdata3 crtsurfdata4 crtsurfdata5 crtsurfdata ftpclient obtcodetodb obtmindtodb

crtsurfdata1:crtsurfdata1.cpp
	g++ $(CFLAGS) -o crtsurfdata1 crtsurfdata1.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata1 ../bin/.

crtsurfdata2:crtsurfdata2.cpp
	g++ $(CFLAGS) -o crtsurfdata2 crtsurfdata2.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata2 ../bin/.

crtsurfdata3:crtsurfdata3.cpp
	g++ $(CFLAGS) -o crtsurfdata3 crtsurfdata3.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata3 ../bin/.

crtsurfdata4:crtsurfdata4.cpp
	g++ $(CFLAGS) -o crtsurfdata4 crtsurfdata4.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata4 ../bin/.

crtsurfdata5:crtsurfdata5.cpp
	g++ $(CFLAGS) -o crtsurfdata5 crtsurfdata5.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata5 ../bin/.

crtsurfdata:crtsurfdata.cpp
	g++ $(CFLAGS) -o crtsurfdata crtsurfdata.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata ../bin/.

ftpclient:ftpclient.cpp
	g++ $(CFLAGS) -o ftpclient ftpclient.cpp /project/public/_ftp.cpp $(PUBCPP) $(PUBINCL) -L/project/public -lftp -lm -lc
	cp ftpclient ../bin/.

obtcodetodb:obtcodetodb.cpp
	g++ $(CFLAGS) -o obtcodetodb obtcodetodb.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) -lm -lc
	cp obtcodetodb ../bin/.

obtmindtodb:obtmindtodb.cpp idcapp.h idcapp.cpp
	g++ $(CFLAGS) -o obtmindtodb obtmindtodb.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) idcapp.cpp -lm -lc
	cp obtmindtodb ../bin/.

clean:
	rm crtsurfdata1 crtsurfdata2 crtsurfdata3 crtsurfdata4 crtsurfdata5 crtsurfdata ftpclient
	rm obtcodetodb obtmindtodb
	
	
			
clean:
	rm crtsurfdata1 crtsurfdata2 crtsurfdata3 crtsurfdata4 crtsurfdata5 crtsurfdata ftpclient 
	rm obtcodetodb obtmindtodb 
