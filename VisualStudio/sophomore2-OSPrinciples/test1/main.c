#include <sys/types.h>
#include <sys/wait.h> //���ͷ�ļ�������
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main() {
	pid_t pid;
	char *message;
	int n;
	int exit_code;
	printf("fork progra starting\n");

	pid = fork();
	switch (pid) {
	case -1:
		perror("fork failed");
		exit(1);
	case 0:
		message = "child";
		n = 5;
		exit_code = 37; //�ӽ����˳���
		break;
	default:
		message = "parent";
		n = 3;
		exit_code = 0;
		break;
	}
	for (; n > 0; n--) {
		puts(message);
		sleep(1);
	}
	// ������Ӱ����Ϊ�����̵ȴ��ӽ������
	if (pid != 0) { //�������ڴ�ִ��
		int stat_val;
		pid_t child_pid;
		child_pid = wait(&stat_val); 
			printf("Child has finished: PID=%d\n", child_pid);
		if (WIFEXITED(stat_val)) // ����ӽ�����������������ȡһ������ֵ
			printf("child exited with code %d\n", WEXITSTATUS(stat_val)); // �� ��
		WIFEXITED ���㣬�������ӽ��̵��˳���
		else
			printf("child terminated abnormally\n");
	}
	exit(exit_code);
}