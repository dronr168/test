#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <cstdint>
#include <stdio.h>

#include "common.h"

#include "md5.h"
#include "platform_util.h"
#include "error.h"
#include <string.h>

#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_PLATFORM_C)
#include "platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST */


int tick_microsec;
int n = 1;
char bufer[32000];
int port;
char *ip;
struct sockaddr_in addr;
int sock;
int start;
#define m_size 1000 //размер массива данных 1000
int16_t Data[m_size];
int mbedtls_md5(const unsigned char *input, size_t ilen, unsigned char output[16]);
void Packed(int t){
for(int nP=1;nP<(t+1);nP++){
	for(int i=0;i<m_size;i++){
		Data[i] = (time(NULL) + i) / clock();
		}
	char Data_h[m_size*6];
	for(int i=0;i<m_size;i++){
		sprintf(Data_h+6*i, "%06d", Data[i]);
		}
	unsigned char hash[16];
	mbedtls_md5((const unsigned char*)Data_h, (size_t)sizeof(Data_h), hash);
	char hash_std[33];
	for(int g=0;g<16;g++){
		sprintf(hash_std+2*g, "%02x", hash[g]);
	}
	int sleep = clock();
	while(clock() < (sleep + tick_microsec * 10)){};
	char Nm[] = "№";
        int f = 0;
        sprintf(bufer, "%s", Nm);
        f=f+3;
        char Nn[4+sizeof(char)];
        sprintf(Nn, "%04d", n);
        sprintf(bufer+f, "%4.4s", Nn);
        f=f+4;
        bufer[f] = ' ';
        f++;
        char hour[2+sizeof(char)];
        sprintf(hour, "%02d", ((time(NULL) / 60) / 60) % 24);
        sprintf(bufer+f, "%2.2s", hour);
	f=f+2;
        bufer[f] = ':';
        f++;
        char min[2+sizeof(char)];
        sprintf(min, "%02d", (time(NULL) / 60) % 60);
        sprintf(bufer+f, "%2.2s", min);
	f=f+2;
        bufer[f] = ':';
        f++;
        char sec[2+sizeof(char)];
        sprintf(sec, "%02d", time(NULL) % 60);
        sprintf(bufer+f, "%2.2s", sec);
        f=f+2;
        bufer[f] = '.';
        f++;
        char microsec[3+sizeof(char)];
        sprintf(microsec, "%03d", ((clock() - start) / tick_microsec)%1000);
        sprintf(bufer+f, "%3.3s", microsec);
        f=f+3;
        bufer[f] = ' ';
        f++;
        sprintf(bufer+f, "%s", Data_h);
        f=f+sizeof(Data_h);
        bufer[f] = ' ';
        f++;
        sprintf(bufer+f, "%s", hash_std);
        f=f+sizeof(hash_std);
        bufer[f] = '\n';
        f++;
        bufer[f] = '\0';
        send(sock, bufer, sizeof(bufer), 0);
        std::cout << "Send: №" << n << ' ' << hour << ':' << min << ':' << sec << '.' << microsec << '\n';
	n++;
        }
}



int main(int argc, char *argv[]){
int s_time = time(NULL);
while(s_time == time(NULL)){};
int e_time = time(NULL);
start = clock();
while(e_time == time(NULL)){};
int stop = clock();
tick_microsec = (stop - start) / 1000;

if(argc > 2){
	if(!(ip = argv[1])&!(port = int (atoi(argv[2])))){
		std::cout << "./ClientTest <IP> <PORT>\n";
		exit(1);
	}
}else{
std::cout << "./ClientTest <IP> <PORT>\n";
exit(1);
}

sock = socket(AF_INET, SOCK_STREAM, 0);
if(sock < 0){
	perror("socket");
	int close(sock);
	exit(2);
}

addr.sin_family = AF_INET;
if(!(inet_pton(AF_INET, ip, &(addr.sin_addr)))){
	perror("inet_pton");
	int close(sock);
	exit(3);
	}
addr.sin_port = htons(port);

if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
	perror("connect");
	int close(sock);
	exit(4);
}
Packed(1000);
int paus = time(NULL);
while(time(NULL) < (paus+10)){};
Packed(1000);

int close(sock);
return 0;
}
