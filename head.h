#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <unordered_map>
#include <sys/param.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <iostream>
#include<fstream>
using namespace std;

struct Param{          //线程传入多个参数
    int conn;
    MYSQL mysql;
    char m_buf[1024];
};

//unordered_map<struct sockaddr_in,int>map;

//十六进制转2进制
char *htob(string s);

//十六进制转十进制;
char *htod(string s,int f); 

//十六进制转ip
char *htoip(string s);

bool data_Query(char *time,char data[9][20],char *record_num,char *areaID,MYSQL mysql);


bool Data_insert(string s,MYSQL mysql);

//十六进制数组转字节流
int Change16tobit(char *bit,char *s);

/*将recv函数自动转换的 中文字符数组 转换成 16进制字符串格式的字符数据*/
void Changestr_16_2(char *t,char *s,int len);


