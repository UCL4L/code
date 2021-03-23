#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	char *demo = "DATA:name=zhangsheng,age=24,score=90";
	struct student{
		char name[14];
		short age;
		int score;
	} man, dman;
	char *p,*tmp;

	p=strstr(demo,"name=");
	tmp=strchr(demo,',');
	strncpy(man.name,p+5,tmp-p-5);

	printf("%s\n",man.name);

	


	

	return 0;
}
