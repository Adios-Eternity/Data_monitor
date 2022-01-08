#include "function.cpp"


void *pthread_main(void *arg)
{
    pthread_detach(pthread_self());    
    struct Param *param;
    param = (struct Param *)arg;
    int conn=(*param).conn;
    MYSQL mysql=(*param).mysql;
    string s0;
    for(int i=0;i<strlen(param->m_buf);i++)
        s0+=param->m_buf[i];
    cout <<s0<<endl;
    Data_insert(s0,mysql);
    pthread_exit(0);
}


int main(){

    //-L/usr/lib64/mysql -lmysqlclient -lpthread
    MYSQL mysql;
    if(mysql_init(&mysql)==NULL){
        cout <<"mysql_init():"<<mysql_error(&mysql);
    }
    if(mysql_real_connect(&mysql,"localhost","root","","data_monitor",0,NULL,0)==NULL)
    {
        cout <<"mysql_real_connect():"<< mysql_error(&mysql)<<endl;
    }
    else
    {
        cout <<"Connected MySQL successful!"<<endl;
        mysql_query(&mysql, "set names 'utf8'");
    }

    int sockfd;
    pthread_t t_id;  
    struct sockaddr_in servaddr,cliaddr;
    sockfd= socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(8275);

    bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    int n;
    socklen_t len=sizeof(cliaddr);
    char mesg[1024];
    while(1){
        n=recvfrom(sockfd,mesg,1024,0,(struct sockaddr *)&cliaddr,&len);
        printf("mesg:%s\nlen:%d\n",mesg,(int)strlen(mesg));
        struct Param param;
        param.conn=sockfd;
        param.mysql=mysql;
        char buf[1024];
        Changestr_16_2(buf,mesg,62);
        // if(map.find(cliaddr)==map.end()){
        //     map[cliaddr]=1;
        //     strcpy(param.m_buf,buf);
        //     pthread_create(&t_id,NULL,pthread_main,(void *)&param);  //创建新的线程处理
        // }
        // else{
        //     if(buf[strlen(buf)-1]-'0'==(map[cliaddr]+1)%10)
        //     {
        //         strcpy(param.m_buf,buf);
        //         pthread_create(&t_id,NULL,pthread_main,(void *)&param);  //创建新的线程处理
        //         map[cliaddr]=(map[cliaddr]++)%10;
        //     }
        //     else{
        //         char sendbuf[2];
        //         char sendbuf[0]=(map[cliaddr]+1)%10;
        //         sendto(sockfd,sendbuf,1,0,(struct sockaddr *)&cliaddr,sizeof(servaddr));
        //     }
        // }
        strcpy(param.m_buf,buf);
        pthread_create(&t_id,NULL,pthread_main,(void *)&param);  //创建新的线程处理
    }

    return 0;
}