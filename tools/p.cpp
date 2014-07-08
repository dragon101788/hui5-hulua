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

int main(int argc, char** argv)                               //argc:�������    argv������
{
	Msg msg;
	msg.open_message(argv[1]);
//	char w_buf[4096]={0};
//	sprintf(w_buf,"<scfg name=p id=%d />",strtol(argv[2],NULL,10));

	msg.send_message(100,argv[2]);


	printf("msg count = %d\r\n",msg.count());

}
