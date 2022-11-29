#define _GNU_SOURCE // for execvpe()
#if __has_include(<features.h>)
#  include <features.h> // __GNU_LIBRARY__
#endif
#include <fcntl.h>  // open
#include <stdio.h>  // perror
#include <stdlib.h> // exit
#include <sys/wait.h>
#include <unistd.h> // fork, exec*, fexecve

#define errExit(msg)                                                           \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
/**
 * exec() 函数原型
 * execl()
 * execle()
 * execv()
 * execlp()
 * execvp()
 * execve()
 * l 表明 list，说明执行程序的命令行参数以列表的方式提供，并且以 NULL 结尾，但是没有参数个数限制
 * v 表示 vector，说明命令行要以二维数组的形式提供给新程序，这个数组的每一行是一个命令参数
 * e 表示传递给新程序的环境变量，这个列表是个二维数组，每一行是一个环境变量。如果没有显式地传递一个环境变量表，那么新程序将复制父进程的环境表
 * 以 p 结尾的 exec() 函数表示第一个参数不是完整的路径名，而是一个程序名
 * 返回值：成功不返回值，失败返回 -1
 */

int main() { // APUE 8.10
  char *argv[] = {"ls", "-l", "-a", "-h", (char *)NULL};
  char *envp[] = {"PATH=/bin:/usr/bin", NULL};

  pid_t cpid = fork();
  if (cpid == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("execl:\n");
    if (execl("/bin/ls", "ls", "-l", "-a", "-h", (char *)NULL) == -1)
      errExit("execl");
  }

  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nexeclp:\n");
    if (execlp("ls", "ls", "-l", "-a", "-h", (char *)NULL) == -1)
      errExit("execlp");
  }

  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nexecle:\n");
    if (execle("/bin/ls", "ls", "-l", "-a", "-h", (char *)NULL, envp) == -1)
      errExit("execle");
  }

  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nexecv:\n");
    if (execv("/bin/ls", argv) == -1)
      errExit("execv");
  }

  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nexecvp:\n");
    if (execvp("ls", argv) == -1)
      errExit("execvp");
  }

  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nexecve:\n");
    if (execve("/bin/ls", argv, envp) == -1) // system call
      errExit("execve");
  }

#ifdef __GNU_LIBRARY__
  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nexecvpe:\n");
    if (execvpe("ls", argv, envp) == -1)
      errExit("execvpe");
  }

  if (wait(NULL) == -1)
    errExit("wait");
  if ((cpid = fork()) == -1)
    errExit("fork");
  else if (cpid == 0) {
    printf("\nfexecve:\n");
    int fd = 0;
    if ((fd = open("/bin/ls", O_PATH)) == -1)
      errExit("open");
    if (fexecve(fd, argv, envp) == -1)
      errExit("fexecve");
  }
#endif

  return 0;
}
