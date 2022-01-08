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
#include <sys/param.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <iostream>
#include<fstream>
 
using namespace std;

struct Param{          //线程传入多个参数
    int sockfd;
    char m_buf[1024];
};

string slid_window_buffer1="40323031450001002727101d04010c005CC13F00C0BF3F00C8BE3F00DBBF3F00E0BF3F0072C13F0032C13F0048BF3F000080BD3246464646464646460D0A";
string slid_window_buffer2="";
string slid_window_buffer3="";

int serial_num=1;

//十六进制数组转二进制字节流
int Change16tobit(char *bit,char *s)
{
    int n=0;
    for(int i=0;s[i];i+=2)
    {
        if(s[i]>='A'&&s[i]<='F')
            bit[n]=s[i]-'A'+10;
        else
            bit[n]=s[i]-'0';
        if(s[i+1]>='A'&&s[i+1]<='F')
            bit[n]=(bit[n]<<4)|(s[i+1]-'A'+10);
        else
            bit[n]=(bit[n]<<4)|(s[i+1]-'0');
        ++n;
    }
    return n;
}

/*将recv函数接收的数组 自动转换的 中文字符数组 转换成 16进制字符串格式的字符数据*/
void Changestr_16_2(char *t,char *s,int len) {
    char i,chs;
    for(i = 0; i<len; ++i) {
        chs = s[i] & 0X0F;
        if(chs > 9) t[2 * i + 1] = chs - 10 + 'A';
        else t[2 * i + 1] = chs + '0';
        chs = (s[i] & 0XF0) >> 4;
        if(chs > 9) t[2 * i] = chs - 10 + 'A';
        else t[2 * i] = chs + '0';
    }
    t[2 * i] = '\0';
}

// void *pthread_main(void *arg){
//     pthread_detach(pthread_self());
//     struct Param *param;
//     param = (struct Param *)arg;
//     int sockfd=(*param).sockfd;

//     struct sockaddr_in cliaddr;
//     socklen_t len=sizeof(cliaddr);
//     char mesg[5];
//     int n=recvfrom(sockfd,mesg,1024,0,(struct sockaddr *)&cliaddr,&len);
//     char sendbuf[1024];
//     int len=0;
//     if(mesg[0]==slid_window_buffer1[slid_window_buffer1.length()-1]){
//         len=Change16tobit(sendbuf,slid_window_buffer1.c_str());
//     }

//     else if(mesg[0]==slid_window_buffer2[slid_window_buffer2.length()-1]){
//         len=Change16tobit(sendbuf,slid_window_buffer2.c_str());
//         serial_num=(serial+10-1)%10;    
//     }
//     else {
//         len=Change16tobit(sendbuf,slid_window_buffer3.c_str());
//         serial_num=(serial+10-2)%10;
//     }
//     sendto(sockfd,sendbuf,len,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
//     pthread_exit(0);
// }


int main(){

    int sockfd;
    pthread_t t_id;  
    struct sockaddr_in servaddr,clientaddr;
    sockfd= socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(8275);

    // servaddr.sin_family=AF_INET;
    // servaddr.sin_addr.s_addr=inet_addr("127.0.0.2");
    // servaddr.sin_port=htons(8276);
    // bind(sockfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr));

    string Hexcode="0123456789ABCDEF";
    char sendbuf_str[1024]="40323031450001002727101d04010c005CC13F00C0BF3F00C8BE3F00DBBF3F00E0BF3F0072C13F0032C13F0048BF3F000080BD3246464646464646460D00";
    char sendbuf[1024];
    int len=Change16tobit(sendbuf,sendbuf_str);
    printf("sendbuf:%s\nlen:%d\n",sendbuf,(int)strlen(sendbuf));
    sendto(sockfd,sendbuf,len,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
    while(true){
        //areaID
        sendbuf_str[2]=Hexcode[rand()%10];
        sendbuf_str[3]=Hexcode[rand()%10];
        //记录号
        sendbuf_str[11]=Hexcode[rand()%10];
        sendbuf_str[12]=Hexcode[rand()%10];
        //数据
        for(int i=29;i<102;i++)
            sendbuf_str[i]=Hexcode[rand()%10];
        //末尾序列号
        sendbuf_str[strlen(sendbuf_str)-1]=serial_num-'0';

        memset(sendbuf, 0 ,sizeof(sendbuf));
        len=Change16tobit(sendbuf,sendbuf_str);
        sendto(sockfd,sendbuf,len,0,(struct sockaddr *)&servaddr,sizeof(servaddr));

        // char m_buf[1024];
        // struct Param param;
        // param.sockfd=sockfd;
        //pthread_create(&t_id,NULL,pthread_main,(void *)&param);  //创建新的线程处理

        serial_num=(serial_num++)%10;

        // slid_window_buffer3=slid_window_buffer2;
        // slid_window_buffer2=slid_window_buffer1;
        // slid_window_buffer1="";
        // for(int i=0;i<strlen(sendbuf_str);i++)
        //     slid_window_buffer1[i]=sendbuf_str[i];
        sleep(5);
    }
    
    close(sockfd);
    return 0;
}