#include "head.h"

//十六进制转二进制
char *htob(string s)
{
    char result[100];
    strcpy(result,"");
    char b[16][5]={"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    for(int i=0;i<s.length();i++)
    {
        switch (s[i])
        {
        case 'A':
            strcat(result,b[10]);
            break;
        case 'B':
            strcat(result,b[11]);
            break;
        case 'C':
            strcat(result,b[12]);
            break;
        case 'D':
            strcat(result,b[13]);
            break;
        case 'E':
            strcat(result,b[14]);
            break;
        case 'F':
            strcat(result,b[15]);
            break;
        default:
            strcat(result,b[s[i]-'0']);
            break;
        }
    }
    char *re=result;
    return re;
}

//十六进制转十进制   f为标志位，0：无符号十六进制转换  1：有符号十六进制转换
char *htod(string s,int f)
{
    int result=0;
    char b[100];
    strcpy(b,htob(s));
    //无符号十六进制
    if(f==0) 
    {
        for(int i=s.length()-1;i>=0;i--)
        {
            int l=0;
            switch (s[i])
            {
            case 'A':
                l=10;
                break;
            case 'B':
                l=11;
                break;
            case 'C':
                l=12;
                break;
            case 'D':
                l=13;
                break;
            case 'E':
                l=14;
                break;
            case 'F':
                l=15;
                break;
            default:
                l=s[i]-'0';
                break;
            }
        result += l*pow(16,s.length()-1-i);
        }
    }
    //有符号十六进制
    else{
        if(b[0]=='0')     //正数
        {
            for(int i=s.length()-1;i>=0;i--)
            {
                int l=0;
                switch (s[i])
                {
                case 'A':
                    l=10;
                    break;
                case 'B':
                    l=11;
                    break;
                case 'C':
                    l=12;
                    break;
                case 'D':
                    l=13;
                    break;
                case 'E':
                    l=14;
                    break;
                case 'F':
                    l=15;
                    break;
                default:
                    l=s[i]-'0';
                    break;
                }
                result += l*pow(16,s.length()-1-i);
            }
        }
        else{               //负数
            for(int i=1;i<strlen(b);i++)  //取反
            {
                if(b[i]=='1')
                    b[i]='0';
                    else
                    b[i]='1';
            }
            for(int i=strlen(b)-1;i>=0;i--)  //加一
            {
                if(i==0&&b[i]==1)
                {
                    for(int i=strlen(b);i>0;i--)
                        b[i]=b[i-1];
                    b[0]='1';
                }

                if(b[i]=='0')
                {
                    b[i]='1';
                    break;
                }
                else
                    b[i]='0';
            }
            for(int i=strlen(b)-1;i>0;i--)        //带符号的二进制转化为十进制
                result += (b[i]-'0')*pow(2,strlen(b)-1-i);
            result =0-result;
        }
    }
    char num[100];
    sprintf(num,"%d",result);
    char *Num=num;
    return Num;
}

//十六进制转ip
char *htoip(string s)
{
    char ip[20];
    strcpy(ip,"");
    strcat(ip,(char *)htod(s.substr(0,2),0));
    strcat(ip,(char *)".");
    strcat(ip,(char *)htod(s.substr(2,2),0));
    strcat(ip,(char *)".");
    strcat(ip,(char *)htod(s.substr(4,2),0));
    strcat(ip,(char *)".");
    strcat(ip,(char *)htod(s.substr(6,2),0));
    char *result=ip;
    return result;
}


bool data_Query(char *time,char data[9][20],char *record_num,char *areaID,MYSQL mysql){
    char q[1024];
	strcpy(q,"insert into data(create_time,data1,data2,data3,data4,data5,data6,data7,data8,data9,device_id,area_id) values('");
    strcat(q,(char *)"2012-1-4-29-10-27-27");
    strcat(q,"',");
    strcat(q,(char *)data[0]);
    strcat(q,",");
    strcat(q,(char *)data[1]);
    strcat(q,",");
    strcat(q,(char *)data[2]);
    strcat(q,",");
    strcat(q,(char *)data[3]);
    strcat(q,",");
    strcat(q,(char *)data[4]);
    strcat(q,",");
    strcat(q,(char *)data[5]);
    strcat(q,",");
    strcat(q,(char *)data[6]);
    strcat(q,",");
    strcat(q,(char *)data[7]);
    strcat(q,",");
    strcat(q,(char *)data[8]);
    strcat(q,",");
    strcat(q,(char *)record_num);
    strcat(q,",");
    strcat(q,(char *)areaID);
    strcat(q,")");
    printf("insert:%s\n",q);
    int rc=mysql_real_query(&mysql,q,strlen(q));
    if(rc!=0)
    {
        cout <<"mysql_real_query():"<< mysql_error(&mysql)<<endl;
        return false;
    }
    return true;
}

bool Data_insert(string s,MYSQL mysql){
    char record_num_str[10];
    char time[7][10];//  0--6 :  s m h d mt w y
    char data[9][20];
    strcpy(record_num_str,htod(s.substr(12,4),0));
    printf("record_num_str:%s\n",record_num_str);
    strcpy(time[0],s.substr(16,2).c_str());  //s
    strcpy(time[1],s.substr(18,2).c_str());  //m
    strcpy(time[2],s.substr(20,2).c_str());   //h
    strcpy(time[3],htod(s.substr(22,2),0));   //d
    strcpy(time[4],htod(s.substr(24,2),0));   //mt
    strcpy(time[5],htod(s.substr(26,2),0));   //w
    strcpy(time[6],htod(s.substr(28,2),0));   //y
    string time_str="";
    for(int i=6;i>=0;i--)
    {
        for(int j=0;j<strlen(time[i]);j++)
            time_str+=time[i][j];
        time_str+="-";
    }
    char insert_time[100];
    strcpy(insert_time,time_str.c_str());
    printf("time:%s\n",insert_time);
    char areaID[10];
    strcpy(areaID,(s.substr(2,2)).c_str());

    for(int i=0;i<9;i++)
        strcpy(data[i],htod(s.substr(30+i*8,8),0));
    for(int i=0;i<9;i++)
    {
        if(data[i][0]=='-'){
            for(int j=0;j<strlen(data[i])-1;j++)
                data[i][j]=data[i][j+1];
            data[i][strlen(data[i])-1]='\0';
        }
        int data_len=strlen(data[i]);
        printf("%s    %d\n",data[i],data_len);
        if(data_len>2){
            for(int j=data_len;j>2;j--)
                data[i][j]=data[i][j-1];
            data[i][2]='.';
            data[i][data_len+1]='\0';
        }
    }
    return data_Query(insert_time,data,record_num_str,areaID,mysql);;
}


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
