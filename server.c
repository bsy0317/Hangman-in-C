#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

void error_handling(char * message);
int compare(const void *a, const void *b);
void signalHandler(int signo);
int serv_sock, clnt_sock;
pid_t pid;

int main(int argc, char ** argv){
	struct sockaddr_in serv_addr, clnt_addr;
	int clnt_addr_size;
	char buf[5000],tmp[32];
	char word[360][128];
	char word_select[128];
	int answer[128];
	int i=0,word_count=0,word_len=0,loop=1;
	int fds1[2], fds2[2];
	FILE* fp;
	sigset_t set;
	sigfillset(&set);
	signal(SIGINT,signalHandler); //Server Shutdown Signal
	signal(SIGTERM,signalHandler); //Server Shutdown Signal
	
	pipe(fds1);
	pipe(fds2);
start:    //The point of return after IPC Communication is terminated.
	word_count=word_len=i=0;
	loop=1; //Initialize loop statement
	system("clear");
	printf("Press Ctrl+C to shutdown the server.\n");
	
	srand((unsigned int)time(NULL));
	memset(answer,-1,sizeof(answer)); //Initialize unused arrays with -1
	
	fp = fopen("word.txt","r");
	fgets(buf, sizeof(buf), fp);
	fclose(fp);
	
	char *ptr = strtok(buf,"|"); //Separating characters from buffers based on '|'
	while(ptr != NULL){
		strcpy(word[i++],ptr);
		ptr = strtok(NULL, "|");
	}
	strcpy(word[i],"EOF"); //Insert a 'EOF' at the end of the word
	word_count=i;
	
	strcpy(word_select, word[rand()%word_count]); //Choose a word randomly.
	word_len = strlen(word_select); //length of word
	
	//For debugging purposes, print the answer.
	printf("==The answer to this question is \"%s\"==\n",word_select);
	
	pid = fork();
	if(pid == 0){
		write(fds1[1], &word_len, sizeof(word_len)); //Sending length over IPC.
		while(loop){
			int i=0,j=0;
			read(fds2[0], tmp, sizeof(tmp)); //Received answers
			for(i=0; i<word_len; i++){
				for(j=0; j<strlen(tmp); j++){
					if(word_select[i] == tmp[j]){
					//Marks '1' at the location of the matching character
						answer[i] = 1; 
					}					
				}
			}
			//If the length of the word is correct,
			//  it will be out of the repeating phrase.
			for(i=0;i<word_len; i++){
				if(answer[i] == 1){
					loop=0;
				}else{
					loop=1;
					break;
				}
			}
			
			write(fds1[1], answer, sizeof(answer));	
		}
	}else{
		read(fds1[0], &word_len, sizeof(word_len));
		//printf("Anwser Length = %d\n",word_len);
		int option = 1;
		serv_sock = socket(PF_INET, SOCK_STREAM, 0);
		setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); //Prevent TCP TIME_WAIT state.
		if(serv_sock == -1) error_handling("socket() error");
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = PF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(atoi(argv[1]));
		if(bind(serv_sock, (struct sockaddr * ) &serv_addr, sizeof(serv_addr)) == -1)
			error_handling("bind() error");
  	  	if(listen(serv_sock,5) == -1) error_handling("listen() error");
  	  	clnt_addr_size = sizeof(clnt_addr);
  	  	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
  	  	if (clnt_sock == -1) error_handling("accept() error");
  	  	
  	  	write(clnt_sock, &word_len, sizeof(word_len));
  	  	while(loop){
			read(clnt_sock, tmp, sizeof(tmp));
			write(fds2[1], tmp, sizeof(tmp));
			read(fds1[0], answer, sizeof(answer));
			
			loop = 1;
			for(i=0; i<word_len; i++){
				if(answer[i] != 1){
					loop = 1;
					break;
				}else{
					loop = 0;
				}
			}
			
			write(clnt_sock, &answer, sizeof(answer));
			
		}
		//kill(pid,SIGKILL);
		exit(0);
		printf("If you see this message, you're not dead(%d).\n",pid);
	}
	shutdown(serv_sock, SHUT_RDWR);
	shutdown(clnt_sock, SHUT_RDWR);
	printf("The server is being restart, please wait a moment.\n");
	sleep(3);
	goto start;
	return 0;
}
void error_handling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	kill(pid,SIGKILL);
	exit(1);
}
void signalHandler(int signo){
	printf("Shutdown the Server.\n");
	shutdown(serv_sock, SHUT_RDWR);
	shutdown(clnt_sock, SHUT_RDWR);
	kill(pid,SIGKILL);
	exit(0);
}
int compare(const void *a, const void *b){
	if(*(int*)a > *(int*)b) return 1;
	else if(*(int*)a < *(int*)b) return -1;
	else return 0;
}
