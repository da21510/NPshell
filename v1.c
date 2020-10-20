#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(void) {

	//	char PATH[][];


	//-----------------------------------------------------------
	while(1){
		int i=0;
		int k=0;
		int flag=0;

		char input[512];
		
		char Q[7500][512];

		printf("%% ");
		while(flag==0){
			input[i]=fgetc(stdin);


			if (input[i]==' ' && i!=0 && input[i-1]!=' '){
				i++;
			}
			else if (input[i]=='\n'){
				input[i] = '\0';
				flag=1;
				strcpy(Q[k],input);
				k++;
			}
			else if (input[i]=='|'){
				input[i] = '\0';
				strcpy(Q[k],input);
				k++;
				i=0;
			}
			else if (input[i]!=' '){
				i++;
			}

		}

		//-----------------------------------------------------------
	int is_empty=0;
	int l=0;
	int pipefd0[2];
	int pipefd1[2];

	while(is_empty==0){		
		int len;
		char buffer[1024]={0};

		if(pipe(pipefd0)<0)
			printf("pipe1 create error");
		signal(SIGCHLD,SIG_IGN);
		if(fork() == 0){ /*child process*/
			close(pipefd0[0]);
			dup2(pipefd0[1], STDOUT_FILENO); /*pipefd0 will close after STDIN receive EOF*/

			char *argv[50];
			int argvi = 0;
			int it = 0;
			_Bool f = 1;
			while(Q[l][it] != '\0') {
				if(Q[l][it] != ' ') {
					if (f){
						argv[argvi] = &Q[l][it];
						++argvi;
						++it;
						f = 0;
					} else {
						++it;
					}
				} else {
					Q[l][it] = '\0';
					++it;
					f = 1;
				}
			}
			argv[argvi] = NULL;


			execvp(argv[0],argv);
			exit(0);
		}else{ /*parent process*/
			close(pipefd0[1]);
			while((len=read(pipefd0[0],buffer,1023))>0){
				buffer[len]='\0';
				printf("%s\n",buffer);
			}
		}


		l++;
		if(l==k){
			is_empty=1;
		}


	}
	}




	return 0;
}

