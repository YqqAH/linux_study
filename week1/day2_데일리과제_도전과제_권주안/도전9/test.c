#include <stdio.h>
#include <ctype.h>
#include <string.h>

char str[10000];
char temp[10000];
char bye[3]={'b','y','e'};
char word[4]={'e','x','i','t'};

int checkBye() {
	int len=0;
	while(1) {
		if(str[len]!='\0'){
			str[len]=tolower(str[len]);
			len++;
		}
		else {
			break;
		}
	}
	for(int i=0;i<len;i++){
		if(str[i]!=bye[i]) return 0;		
	}
	return 1;
}

int checkExit() {
	int len=0;
	while(1) {
		if(str[len]!='\0'){
			str[len]=tolower(str[len]);
			len++;
		}
		else {
			break;
		}
	}
	for(int i=0;i<len;i++){
		if(str[i]!=word[i]) return 0;
	}
	return 1;
}

int main() {
	int run = 1;
	while (run) {
		printf("SSAFY_ECHOBOT >> ");
		scanf("%s", str);
		strcpy(temp, str);
		if(checkBye() || checkExit()) {
			run = 0;
		}
		else {
			printf("%s\n", temp);
		}
	}
	return 0;
}

