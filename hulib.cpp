#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ucontext.h>
#include <signal.h>
#include <dlfcn.h>
#include "hulib.h"

huvar g_var;
//HuDebug debug;

//int system_noblk(const char *cmdstring) /* with appropriate signal handling */
//{
//	pid_t pid;
//	int status;
//	struct sigaction ignore, saveintr, savequit;
//	sigset_t chldmask, savemask;
//	if (cmdstring == NULL)
//		return (1); /* always a command processor with UNIX */
//	ignore.sa_handler = SIG_IGN; /* ignore SIGINT and SIGQUIT */
//	sigemptyset(&ignore.sa_mask);
//	ignore.sa_flags = 0;
//	if (sigaction(SIGINT, &ignore, &saveintr) < 0)
//		return (-1);
//	if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
//		return (-1);
//	sigemptyset(&chldmask); /* now block SIGCHLD */
//	sigaddset(&chldmask, SIGCHLD);
//	if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
//		return (-1);
//	if ((pid = fork()) < 0)
//	{
//		status = -1; /* probably out of processes */
//	}
//	else if (pid == 0)
//	{ /* child *//* restore previous signal actions & reset signal mask */
//		sigaction(SIGINT, &saveintr, NULL);
//		sigaction(SIGQUIT, &savequit, NULL);
//		sigprocmask(SIG_SETMASK, &savemask, NULL);
//		execl("/bin/sh", "sh", "-c", cmdstring, (char *) 0);
//		_exit(127); /* exec error */
//	}
//	else
//	{ /* parent */
//
//	} /* restore previous signal actions & reset signal mask */
//	if (sigaction(SIGINT, &saveintr, NULL) < 0)
//		return (-1);
//	if (sigaction(SIGQUIT, &savequit, NULL) < 0)
//		return (-1);
//	if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
//		return (-1);
//	return (status);
//}

unsigned long GetTickCount()
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void debug_timer(const char * str)
{
	static int oldtime = GetTickCount();
	int usetime = GetTickCount() - oldtime;
	if (str != NULL)
		printf("debug_timer [%s] %dfps/S fps/%dms\r\n", str, usetime ? 1000 / usetime : 1000, usetime);
	oldtime = GetTickCount();
}

void FPSWaitTimer(int ms)
{
	static int intvl = GetTickCount();
	int wait = ms + intvl;
	while (GetTickCount() < wait)
	{
		usleep(1000);
	}
	intvl = wait;
}

static int backtrace_xy(void **BUFFER, int SIZE)
{
	volatile int n = 0;
	volatile int *p;
	volatile int *q;
	volatile int ebp1;
	volatile int eip1;
	volatile int i = 0;
	p = &n;
	ebp1 = p[4];
	eip1 = p[6];
	fprintf(stderr, "======================= backtrace_xy addr: 0x%0x, param1: 0x%0x, param2: 0x%0x\n", backtrace_xy,
			&BUFFER, &SIZE);
	fprintf(stderr, "n addr is 0x%0x\n", &n);
	fprintf(stderr, "p addr is 0x%0x\n", &p);
	for (i = 0; i < SIZE; i++)
	{
		fprintf(stderr, "ebp1 is 0x%0x, eip1 is 0x%0x\n", ebp1, eip1);
		BUFFER[i] = (void *) eip1;
		p = (int*) ebp1;
		q = p - 5;
		eip1 = q[5];
		ebp1 = q[2];
		if (ebp1 == 0 || eip1 == 0)
			break;
	}
	fprintf(stderr, "total level: %d\n", i);
	return i;
}

void errexit(const char * str)
{
	printf("**************error exit*****************\r\n");
	printf(str);
	printf("\r\n");
	printf("*****************************************\r\n");
	dumpstack();
	exit(-1);
}

void debug_usage()
{
	struct rusage buf;

	//int err = getrusage(RUSAGE_CHILDREN, &buf);
	int err = getrusage(RUSAGE_SELF, &buf);
	//printf("ERR=%d\r\n", err);
	//printf("userCPU:%ld.%ld\t", buf.ru_utime.tv_sec, buf.ru_utime.tv_usec);
	//printf("kernelCPU:%ld.%ld\t", buf.ru_stime.tv_sec, buf.ru_stime.tv_usec);
	printf("RUSAGE_SELF MEM:%ld\r\n", buf.ru_maxrss);

}

//typedef struct
//{
//    const char *dli_fname;  /* File name of defining object.  */
//    void *dli_fbase;        /* Load address of that object.  */
//    const char *dli_sname;  /* Name of nearest symbol.���纯����*/
//    void *dli_saddr;        /* Exact value of nearest symbol.���纯�����ʼ��ַ*/
//} Dl_info;

struct ucontext_ce123
{
	unsigned long uc_flags;
	struct ucontext *uc_link;
	stack_t uc_stack;
	struct sigcontext uc_mcontext;
	sigset_t uc_sigmask; /* mask last for extensibility */
} ucontext_ce123_;

struct sigframe_ce123
{
	struct sigcontext sc; //����һ��Ĵ���������
	unsigned long extramask[1];
	unsigned long retcode; //���淵�ص�ַ
//struct aux_sigframe aux __attribute__((aligned(8)));
} sigframe_ce123;

void dumpstack()
{

	Dl_info info;
	int status;

	int *fp = 0, *next_fp = 0;
	int cnt = 0;
	int ret = 0;

	__asm__(
			"mov %0, fp\n"
			: "=r"(fp)
	);

	next_fp = (int *) (*(fp - 3));
	while (next_fp != 0)
	{
		void * pcur = (void *) *(next_fp - 1);

		status = dladdr(pcur, &info);
		if (status && info.dli_fname && info.dli_fname[0] != '\0')
		{
			printf("0x%08x 0x%08x %-20s <  %s+%p  >\r\n", pcur, info.dli_saddr, info.dli_fname, info.dli_sname,
					(unsigned long) ((unsigned int) pcur - (unsigned int) info.dli_saddr));
		}
		else
		{
			printf("[%p]", (void *) *(next_fp - 1));
		}

		//array[cnt++] = (void *)*(next_fp - 1);
		next_fp = (int *) (*(next_fp - 3));
		cnt++;
	}

	printf("Backstrace (%d deep)\n", cnt);

}

