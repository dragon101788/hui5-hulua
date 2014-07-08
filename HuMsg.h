#ifndef __MSG_H__
#define __MSG_H__

#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <linux/limits.h>
#include "hulib.h"
#define MAX_SEND_SIZE 4096

struct hu_msgbuf
{
	long mtype;
	char mtext[MAX_SEND_SIZE];
};


class Msg
{
public:
	key_t key;
	int msgqueue_id;
	//hu_msgbuf qbuf;
	Msg()
	{
		key = 0;
		msgqueue_id = 0;

	}
	~Msg()
	{
		key = 0;
		if(msgqueue_id)
		{
			close(msgqueue_id);
			msgqueue_id=0;
		}

	}
	void init_message()
	{
		/* Create unique key via call to ftok() */
		char link[PATH_MAX];
		char path[PATH_MAX];
		sprintf(link, "/proc/%d/exe", getpid());
		readlink(link, path, sizeof(path));

		key = ftok(path, 1);

		debug("\r\n init_message path[%s] key[%#x]\r\n", path, key);

		/* Open the queue - create if necessary */
		if ((msgqueue_id = msgget(key, IPC_CREAT | 0660)) == -1)

		{
			errexitf("msgget:%s:%s",path,strerror(errno));
			//exit(1);
		}
	}
	void open_message(const char * path)
	{
		/* Create unique key via call to ftok() */
		key = ftok(path, 1);
		open_message(key);
//		printf("open_message key = %#x\r\n", key);
//
//		/* Open the queue - create if necessary */
//		if ((msgqueue_id = msgget(key, 0660)) == -1)
//
//		{
//			perror("msgget");
//			exit(1);
//		}
	}
	void open_message(int k)
	{
		key = k;
		debug("open_message key = %#x\r\n", key);

		/* Open the queue - create if necessary */
		if ((msgqueue_id = msgget(key, 0660)) == -1)

		{
			errexitf("msgget:%d:%s",k,strerror(errno));
			//exit(1);
		}

	}
	void read_message(long type, struct hu_msgbuf *qbuf)
	{
		/* Read a message from the queue */
		qbuf->mtype = type;
		msgrcv(msgqueue_id, (struct msgbuf *) qbuf, MAX_SEND_SIZE, type, 0);

		//printf("Type: %ld Text: %s \r\n", qbuf->mtype, qbuf->mtext);
	}

	void send_message(long type, const char *text)
	{
		/* Send a message to the queue */
		struct hu_msgbuf qbuf;
		qbuf.mtype = type;
		strcpy(qbuf.mtext, text);

		if ((msgsnd(msgqueue_id, (struct msgbuf *) &qbuf,
				strlen(qbuf.mtext) + 1, 0)) == -1)
		{
			perror("msgsnd");
			exit(1);
		}
	}
	void remove_queue()
	{
		/* Remove the queue */
		msgctl(msgqueue_id, IPC_RMID, 0);
	}

	int count()
	{
		struct msqid_ds myqueue_ds;
		/* Get current info */
		msgctl(msgqueue_id, IPC_STAT, &myqueue_ds);
		return myqueue_ds.msg_qnum;
	}

	void change_queue_mode(char *mode)
	{
		struct msqid_ds myqueue_ds;

		/* Get current info */
		msgctl(msgqueue_id, IPC_STAT, &myqueue_ds);

		/* Convert and load the mode */
		sscanf(mode, "%ho", &myqueue_ds.msg_perm.mode);

		/* Update the mode */
		msgctl(msgqueue_id, IPC_SET, &myqueue_ds);
	}

};

#endif //__MSG_H__
