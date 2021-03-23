#include <sys/types.h>
#include <sys/wait.h> //这个头文件不能少
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
		exit_code = 37; //子进程退出码
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
	// 下面阴影代码为父进程等待子进程完成
	if (pid != 0) { //父进程在此执行
		int stat_val;
		pid_t child_pid;
		child_pid = wait(&stat_val); 
			printf("Child has finished: PID=%d\n", child_pid);
		if (WIFEXITED(stat_val)) // 如果子进程正常结束，它就取一个非零值
			printf("child exited with code %d\n", WEXITSTATUS(stat_val)); // 如 果
		WIFEXITED 非零，它返回子进程的退出码
		else
			printf("child terminated abnormally\n");
	}
	exit(exit_code);
}