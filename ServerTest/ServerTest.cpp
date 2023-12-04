#include <string>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <cstdint>
#include <pthread.h>
#include <stdio.h>

#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_PLATFORM_C)
#include "platform.h"
#else
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST */
extern "C" {
int mbedtls_md5(const unsigned char *input, size_t ilen, unsigned char output[16]);
}
unsigned char hash[16];
unsigned char hash2[16];
char hash_out[33];
char hash_out2[33];
std::string result, result2;

int sock, listener, port, bytes_read, start;
struct sockaddr_in addr;

int tick_microsec;
std::string hour, hour2;
std::string min, min2;
std::string sec, sec2;
std::string microsec, microsec2;

#define m_size 1000 //размер массива данных 1000 
const int size_packed = 32000; //максимальный размер пакета
char bufer[16][size_packed]; //кольцевой буфер
bool flag_bufer[16]; 
char buf[size_packed];
int flag;
char Np[16];
char Data_it[m_size*6];
char Data_it2[m_size*6];
char hash_it[33];
char hash_it2[33];

int np = 0;
int f=0;
int i=0;
int s_time, e_time, stop;
int nD, nH, g, nD2, nH2, g2, clr;
bool result_h, result2_h;

void *function1(void *par1){
f=0;
s_time = time(NULL);
while(s_time == time(NULL)){};
e_time = time(NULL);
start = clock();
while(e_time == time(NULL)){};
stop = clock();
tick_microsec = (stop - start) / 1000;
listener = socket(AF_INET, SOCK_STREAM, 0);
        if(listener < 0){
                perror("socket");
                exit(1);
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
                perror("bind");
                exit(2);
        }
        if(listen(listener, 1) < 0){
		perror("listen");
		exit(3);
	}
        while(1){
                sock = accept(listener, (struct sockaddr *)NULL, NULL);
                if(sock < 0){
                        perror("accept");
                        exit(4);
                }
                while(1){
			bytes_read = recv(sock, buf, size_packed, 0);
			if(bytes_read == 0){
				int close(sock);
				break;
			}
                        buf[bytes_read] = '\0';
			np = 0;
			while(buf[np] != ' '){
				Np[np] = buf[np];
				np++;
			}
			np++;
			Np[np] = '\0';
			while(buf[np] != ' '){
				np++;
			}
			np++;
			nD = 0;
			while(buf[np] != ' '){
				Data_it[nD] = buf[np];
				np++;
				nD++;
			}
			Data_it[nD] = '\0';
			nH = 0;
			np++;
			while(nH < 33){
				hash_it[nH] = buf[np];
				np++;
				nH++;
			}
			hash_it[nH] = '\0';
			mbedtls_md5((const unsigned char*)Data_it, (size_t)sizeof(Data_it), hash);
			for(g=0;g<16;g++){
				sprintf(hash_out+2*g, "%.2x", hash[g]);
			}
			result_h = 1;
			for(g=0;g<33;g++){
				if(hash_it[g] != hash_out[g]){
					 result_h = 0;
				}
			}
			if(result_h == 1){
				result = "PASS";
			}else{
				result = "FALL";
			}
			if((f < 16)&&(flag_bufer[f] == 0)){
				flag = f;
				f++;
			}else if((f < 16)&&(flag_bufer[f] == 1)){
				f++;
			}else{
				f = 0;
			}
			
			for(i=0;i<bytes_read;i++){
				bufer[flag][i] = buf[i];
			}
			bufer[flag][i] = '\0';
			flag_bufer[flag] = 1;
                        hour = std::to_string(((time(NULL) / 60) / 60) % 24);
                        min = std::to_string((time(NULL) / 60) % 60);
                        sec = std::to_string(time(NULL) % 60);
                        microsec = std::to_string(((clock() - start) / tick_microsec) % 1000);
			std::cout << "Received: " << Np << ' ' << hour << ":" << min << ":" << sec << ":" << microsec << ' ' << result << '\n';
			int close(sock);
		}
		int close(sock);
	}
pthread_exit(0);
}

int qw;
int qnp, start_mil;
char qN1[16];

void *function2(void *par2){
qw = 0;
while(1){
	if((qw < 16)&&(flag_bufer[qw] == 1)){
		qnp = 0;
		while(bufer[qw][qnp] != ' '){
			qN1[qnp] = bufer[qw][qnp];
			qnp++;
		}
		qN1[qnp] = '\0';
		qnp++;
		while(bufer[qw][qnp] != ' '){
			qnp++;
		}
		qnp++;
		nD2 = 0;
		while(bufer[qw][qnp] != ' '){
			Data_it2[nD2] = bufer[qw][qnp];
			qnp++;
			nD2++;
		}
		Data_it2[nD2] = '\0';
		qnp++;
		nH2 = 0;
		while(nH2 < 33){
			hash_it2[nH2] = bufer[qw][qnp];
			qnp++;
			nH2++;
		}
		hash_it2[nH2] = '\0';
		mbedtls_md5((const unsigned char*)Data_it2, (size_t)sizeof(Data_it2), hash2);
		for(g2=0;g2<16;g2++){
			sprintf(hash_out2+2*g2, "%.2x", hash2[g2]);
		}
		result2_h = 1;
		for(g=0;g<33;g++){
			if(hash_it[g] != hash_out[g]){
				 result2_h = 0;
			}
		}
		if(result2_h == 1){
			result2 = "PASS";
		}else{
			result2 = "FALL";
		}
		start_mil = clock();
		while(clock() < (start_mil + tick_microsec * 15)){};
		hour2 = std::to_string(((time(NULL) / 60) / 60) % 24);
		min2 = std::to_string((time(NULL) / 60) % 60);
		sec2 = std::to_string(time(NULL) % 60);
		microsec2 = std::to_string(((clock() - start) / tick_microsec) % 1000);
		std::cout << "Processed: "  << qN1 << ' ' << hour2 << ":" << min2 << ":" << sec2 << ":" << microsec2 << ' ' << result2 << '\n';
		flag_bufer[qw] = 0;
		qw++;
	}else if((qw < 16)&&(flag_bufer[qw] == 0)){
		qw++;
	}else if(qw == 16){
		qw = 0;
	}
}
pthread_exit(0);
}

int main(int argc, char *argv[]){
if(argc > 1){
	if(!(port = int (atoi(argv[1])))){
		std::cout << "./ServerTest <PORT>" << '\n';
	}
}else{
	std::cout << "./ServerTest <PORT>" << '\n';
}

pthread_t tid1;
pthread_create(&tid1, NULL, function1, NULL);
pthread_t tid2;
pthread_create(&tid2, NULL, function2, NULL);
pthread_join(tid1, NULL);
pthread_join(tid2, NULL);

return 0;
}
