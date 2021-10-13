#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int num_sending_signal;
int num_recv_signal=0;
int num_recv_ack = 0;
pid_t pid;

void recv_ack_handler(int sig){
	
	// insert your code
	num_recv_ack++;
	num_recv_signal++;
	num_sending_signal--;
	return;
}

void terminate_handler(int sig){
	// insert your code
	printf("all signals are sended\n");
	printf("receiver: received %d signals\n", num_recv_signal);

	exit(0);
}

void sending_handler(int sig){	

	// insert your code
	printf("sender: sending %d signal\n", num_sending_signal);
//	num_sending_signal--;
	kill(pid, SIGUSR1);
	alarm(1);
	return;
}

void sending_ack(int sig){

	// insert your code 
	num_recv_signal++;
//	waitpid(0, NULL, WNOHANG);
	printf("receiver: receive %d signal and sending acks\n", num_recv_signal);
//	num_sending_signal--;
	kill(getppid(), SIGUSR1);
	return;
}

int main(int argc, char *argv[]){
	int status;
	num_sending_signal = atoi(argv[1]);
	printf("sending signal: %d\n", num_sending_signal);
	pid = fork();		
	if (pid == 0){ //child
		pid = getppid();
		// insert your code
		// signal using sending_ack
		signal(SIGUSR1, sending_ack);
		signal(SIGINT, terminate_handler);
		// signal using terminate_handler
		while(1){
		}
		

	}
	else{ //parent

		// insert your code
		// signal using recv_ack_handler
		// signal using sending_handler

		signal(SIGALRM, sending_handler);
		signal(SIGUSR1, recv_ack_handler);

		alarm(1);
		while(num_sending_signal!=0 || num_recv_ack!=num_recv_signal){
		}
		if(num_recv_signal==num_recv_ack){
			kill(pid, SIGINT);
		}
	}
	return 0;
}
