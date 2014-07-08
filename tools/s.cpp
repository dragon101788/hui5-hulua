#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include "HuMsg.h"

int main(int argc, char** argv)                               //argc:参数个数？    argv：参数
{
	Msg msg;
	msg.init_message();

	int count = msg.count();
	printf("msg count = %d\r\n", count);
	if (count == 0)
	{
		printf("msg is null\r\n");
	}
	else
	{
		hu_msgbuf qbuf;
		msg.read_message(100, &qbuf);
		printf("recv[%s]\r\n", qbuf.mtext);
	}



}
