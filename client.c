#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define MAX_STACK_SIZE 128
#define _CRT_SECURE_NO_WARNINGS

char stack[MAX_STACK_SIZE];
int top=-1;

void error_handling(char * message);
void push(char value);
void reverseString(char* s);
void showHangman(int choice);
int IsEmpty();
int IsFull();
char pop();

int main(int argc, char ** argv){
	int sock;
	struct sockaddr_in serv_addr;
	int word_len,i=0,j=0,loop=1, try=0;
	int answer[128];
	char word[32],answer_char[128];
	if (argc != 3) {
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&answer_char, 0, sizeof(answer_char));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) == -1)
    	error_handling("connet() error");
    
    read(sock, &word_len, sizeof(word_len));
    for(i=0; i<word_len; i++){
	    	printf("%8c",'_');
	    }
	printf("\n");
    
    while(loop){
	    printf("\n> ");
	    fgets(word,sizeof(word),stdin);
	    ++try;
	    write(sock, &word, sizeof(word));
	    read(sock, &answer, sizeof(answer));
	    reverseString(word);
	    i=0;
	    top = -1;
	    while(word[i] != '\0'){
	    	push(word[i++]);
	    }
	    
	    for(i=0; i<word_len; i++){
	    	if(answer[i] == 1 && answer_char[i] == 0){
	    		char c = pop();
	    		answer_char[i] = c=='\0'?word[0]:c;
	    		//printf("%c\n",answer_char[i]);
	    	}
	    }
	    
	    for(i=0; i<word_len; i++){
	    	printf("%8c",answer_char[i]!=0?answer_char[i]:' ');
	    }
	    printf("\n");
	    for(i=0; i<word_len; i++){
	    	printf("%8c",'_');
	    }
	    printf("\n");
	    
	    for(i=0; i<word_len; i++){
	    	//printf("%d ",answer[i]);
			if(answer[i] != 1){
				loop = 1;
				break;
			}else{
				loop = 0;
			}
		}
		if(try<10) showHangman(0);
		else showHangman(try/10>5?5:(try/10));
	}
	system("clear");
	printf("\nYou succeeded by trying %d times\n",try);
	printf("The answer to the question is \"%s\"\n",answer_char);
	printf("Please check the link below for the meaning of the word.\n");
	printf("https://dic.daum.net/search.do?q=");
	printf("%s\n",answer_char);
	close(sock);
	return 0;
}
void error_handling(char * message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int IsEmpty(){
	if(top < 0) return 1;
	else return 0;
}
int IsFull(){
	if(top >= MAX_STACK_SIZE-1) return 1;
	else return 0;
}
void push(char value){
	if(IsFull() == 1){
		return;
	}
	stack[++top] = value;
}
char pop(){
	if(IsEmpty() == 1){
		return '\0';
	}
	return stack[top--];
}
void reverseString(char* s) {
  size_t size = strlen(s);
  char temp;

  for (size_t i = 0; i < size / 2; i++) {
    temp = s[i];
    s[i] = s[(size - 1) - i];
    s[(size - 1) - i] = temp;
  }
}
void showHangman(int choice){

     switch(choice)
     {

     case 5:
	printf("\n\t||===== ");
	printf("\n\t||    | ");
	printf("\n\t||   %cO/",'\\');
	printf("\n\t||    | ");
	printf("\n\t||   / %c",'\\');
	printf("\n\t||      ");
	break;
     case 4:
	printf("\n\t||===== ");
	printf("\n\t||    | ");
	printf("\n\t||   %cO/",'\\');
	printf("\n\t||    | ");
	printf("\n\t||     %c",'\\');
	printf("\n\t||      ");
	break;
     case 3:
	printf("\n\t||===== ");
	printf("\n\t||    | ");
	printf("\n\t||   %cO/",'\\');
	printf("\n\t||    | ");
	printf("\n\t||      ");
	printf("\n\t||      ");
	break;
     case 2:
	printf("\n\t||===== ");
	printf("\n\t||    | ");
	printf("\n\t||   %cO/",'\\');
	printf("\n\t||      ");
	printf("\n\t||      ");
	printf("\n\t||      ");
	break;
     case 1:
	printf("\n\t||===== ");
	printf("\n\t||    | ");
	printf("\n\t||   %cO ",'\\');
	printf("\n\t||      ");
	printf("\n\t||      ");
	printf("\n\t||      ");
	break;
     case 0:
	printf("\n\t||===== ");
	printf("\n\t||    | ");
	printf("\n\t||    O ");
	printf("\n\t||      ");
	printf("\n\t||      ");
	printf("\n\t||      ");
	break;
      }
      return;
 }
