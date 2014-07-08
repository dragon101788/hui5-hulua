#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include "../HuMsg.h"
#include "../hulib.h"

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

union semun
{
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT & IPC_SET */
	unsigned short *array; /* array for GETALL & SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
	void *__pad;
};

class HuSem
{
public:
	int semid;
	key_t key;

	int open_sem(const char * name)
	{
		key = ftok(name, 1);
		union semun arg;
		semid = semget(key, 1, 0660);
		if (semid < 0)
		{
			if (errno == ENOENT)
			{
				//printf("create new sem %s %#x\r\n", name, key);
				semid = semget(key, 1, 0660 | IPC_CREAT);
				if (semid < 0)
				{
					errexitf("semget error %s\n", strerror(errno));
				}
				arg.val = 1;
				if (semctl(semid, 0, SETVAL, arg) < 0)
				{
					errexitf("semctrl error\n");
				}
			}
			else
			{
				errexitf("semget error\n");
			}

		}

	}

	void P()
	{
		struct sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = -1;
		sb.sem_flg = 0;
		semop(semid, &sb, 1);
	}

	void V()
	{
		struct sembuf sb;
		sb.sem_num = 0;
		sb.sem_op = 1;
		sb.sem_flg = 0;
		semop(semid, &sb, 1);
	}
};
int main(int argc, char** argv)                     //argc:�������    argv������
{

	HuSem sem;
	sem.open_sem("g");
	sem.P();
	Msg msg;
	msg.open_message(argv[1]);

	msg.send_message(100, argv[2]);

	hu_msgbuf qbuf;
	msg.read_message(101, &qbuf);

	if(argc==3)
	{
		printf("read msg:\r\n%s\r\n", qbuf.mtext);
	}
	else if(argc == 4)
	{
		hustr f("%s=",argv[3]);
		const char * p = strstr(qbuf.mtext,f);
		if(p==NULL)
		{
			return 0;
		}
		else
		{
			const char * q1 = p+f.length();
			const char * q2	= strstr(q1,"\r\n");
			char str[q2-q1+1];
			memset(str,0,q2-q1+1);
			strncpy(str,q1,q2-q1);
			printf("%s",str);
		}
	}
	sem.V();

}
