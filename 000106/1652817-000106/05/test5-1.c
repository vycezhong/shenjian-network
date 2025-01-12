#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
void create_deamon(){
	// fork off the parent process
	pid_t pid = fork();
	if(pid < 0){
		exit(EXIT_FAILURE);
	}
	
	// exit the parent process
	if(pid > 0){
		exit(EXIT_SUCCESS); 
	}
	
	// create a new sid for the child process
	pid_t sid = setsid();
	if (sid < 0){
		exit(EXIT_FAILURE);
	}

	signal(SIGCHLD, SIG_IGN);
	
	pid = fork();
	
	if(pid < 0){
		exit(EXIT_FAILURE);
	}
	
	if(pid > 0){
		exit(EXIT_SUCCESS);
	}
	
	// set new file permissions
	umask(0);	

	// change the current working directory
	if((chdir("/")) < 0){
		exit(EXIT_FAILURE);
	}
	
	// close out the standard file descriptors
	/*	
	int x;
	for(x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
		close(x);
	
	openlog("zyc_daemon", LOG_PID, LOG_DAEMON);*/
}
int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Usage: ./test4-1 number\n");
		exit(EXIT_FAILURE);
	}
	int n = atoi(argv[1]);
	//printf("loop %d times\n", n);

	create_deamon();
	
	int i;
	for (i = 0;i < n; ++i){
		pid_t pid = fork();
		
		// fork failed 
		if(pid < 0){			
			break;
		}
		
		// child 
		if (pid == 0){
			char str[1024*10];
			str[0] = 'z';
			str[511] = 'y';
			str[1023] = 'c';
			str[10240 - 1] = 't';
			while(1){
				sleep(1); 
			}
		}
		if(i % 200 == 0){
			printf("已分裂%d个子进程\n", i);
		}
	}

	printf("成功分裂%d个子进程\n", i);
	
	while(1){
		sleep(1); 
	}
}
