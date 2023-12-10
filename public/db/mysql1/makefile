#mysql头文件存放的目录
MYSQLINCL = -I/usr/local/mysql/include

#mysql库文件存放目录
MYSQLLIB = -L/usr/local/mysql/lib

#需要链接的mysql库
MYSQLLIBS = -lmysqlclient

PUBINCLDB = -I/project/public/db/_mysql.h

PUBCPPDB = /project/public/db/mysql/_mysql.cpp

CFLAGS=-g -Wno-write-strings

all:createtable inserttable updatetable selecttable deletetable filetoblob blobtofile

createtable:createtable.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o createtable createtable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

inserttable:inserttable.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o inserttable inserttable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

updatetable:updatetable.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o updatetable updatetable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

selecttable:selecttable.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o selecttable selecttable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

deletetable:deletetable.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o deletetable deletetable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

filetoblob:filetoblob.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o filetoblob filetoblob.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

blobtofile:blobtofile.cpp _mysql.cpp _mysql.h
	g++ $(CFLAGS) -o blobtofile blobtofile.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(PUBINCLDB) $(PUBCPPDB) -lm -lc

clean:
	rm -rf createtable inserttable updatetable selecttable deletetable filetoblob blobtofile
