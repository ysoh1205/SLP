#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MAX_CLIENT 1028
#define BUF_SIZE 1024

typedef struct{
    int user;
    int action;
    int data;
} query;

void *t_function(void *data);

int client_index = 0;

int logUser[1024] = {0, };
int pass[1024];
int seat[256];

int main(int argc, char **argv)
{
    for(int i=0; i<1024; i++){
        pass[i]=-1;
    }
    for(int i=0; i<256; i++){
        seat[i]=-1;
    }
    if (argc != 2)
    {
            printf("Usage : %s [port]\n", argv[0]);
            return 1;
    }

    int server_sock, client_sock;
    pthread_t thread_client[MAX_CLIENT];

    if((server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
    {
            printf("socket create error\n");
            return -1;
    }

    int on = 1;
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
            printf("socket option set error\n");
            return -1;
    }

    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    {
            printf("bind error\n");
            return -1;
    }

    if(listen(server_sock, 5) < 0)
    {
            printf("listen error\n");
            return -1;
    }

    while(1)
    {

        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
        if(client_sock < 0)
        {
            printf("accept error\n");
        }

        if(client_index == MAX_CLIENT)
        {
            printf("client accept full(max client count : %d)\n", MAX_CLIENT);
            close(client_sock);
            continue;
        }

        if(pthread_create(&thread_client[client_index], NULL, t_function, (void *)&client_sock) != 0 )
        {
            printf("Thread create error\n");
            close(client_sock);
            continue;
        }

        client_index++;

    }

    return 0;

}

void *t_function(void *arg)
{
    int client_sock = *((int *)arg);
    int loggedin = 0;
    int who=-1;

    char buf[BUF_SIZE];

    while(1)
    {
        query* q;
        int num;
        memset(buf, 0, sizeof(buf));
        if (read(client_sock, buf, sizeof(buf)) <= 0)
        {
            client_index--;
            close(client_sock);
            break;
        }
        q = (query*)buf;

        //printf("%d %d %d\n", q->user, q->action, q->data);
        
        if(q->action==0 && q->data==0 && q->user==0){
            client_index--;
            if(who!=-1) logUser[who] = 0;
            who = -1;
            loggedin = 0;
            send(client_sock, seat, sizeof(seat), 0);
            close(client_sock);
            break;
        }
        if((q->action<1 || q->action> 5) || (q->user<0 || q->user > 1023)){
            num = -1;
            send(client_sock, &num, sizeof(int), 0);
            continue;
        }


        if(loggedin==0 && q->action!=1){
            num = -1;
            send(client_sock, &num, sizeof(int),0);
            continue;
        }
        if(loggedin==1 && q->action!=1 && logUser[q->user]==0){
            num = -1;
            send(client_sock, &num, sizeof(int),0);
            continue;
        }
        if(loggedin==0 && q->action==1 && logUser[q->user]==1){
            num = -1;
            send(client_sock, &num, sizeof(int),0);
            continue;
        }
        if(loggedin==0 && q->action==1 && pass[q->user]!=-1 && pass[q->user]!=q->data){
            num = -1;
            send(client_sock, &num, sizeof(int),0);
            continue;
        }

        if(loggedin==0 && q->action==1){
            num = 1;
            send(client_sock, &num, sizeof(num),0);
            loggedin=1;
            logUser[q->user]=1;
            pass[q->user]=q->data;
            who = q->user;
            continue;
        }

        if(q->action==2){
            if(q->data<0 || q->data>255){
                num = -1;
                send(client_sock, &num, sizeof(int), 0);
                continue;
            }
            if(seat[q->data]==-1){
                seat[q->data] = q->user;
                num = q->data;
            } else{
                num = -1;
            }
            send(client_sock, &num, sizeof(int),0);
            continue;
        }

        if(q->action==3){
            num = -1;
            for(int i=0; i<256; i++){
                if(seat[i]==q->user) num = i;
            }
            send(client_sock, &num, sizeof(int),0);            
            continue;
        }

        if(q->action==4){
            num = -1;
            if(seat[q->data]==q->user){
                seat[q->data] = -1;
                num = q->data;
            }
            send(client_sock, &num, sizeof(int),0);
            continue;            
        }
        if(q->action==5){
            loggedin=0;
            logUser[q->user]=0;
            num = 1;
            send(client_sock, &num, sizeof(int),0);
            continue;
        }


        if(write(client_sock, &num, sizeof(int)) <= 0)
        {
            client_index--;
            close(client_sock);
            break;
        }

    }
}