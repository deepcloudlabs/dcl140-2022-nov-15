#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 80
struct msgbuf {
   long mtype;
   char mtext[1];
};

int main() {
  key_t key;
  struct msgbuf *sendmsg;
  struct msgbuf *recvmsg;
  int msgqid;
  int rval = 0;

  sendmsg = (struct msgbuf *)malloc(sizeof(long)+BUFSIZE);

  if (sendmsg == NULL) {
    fprintf(stderr, "Out of virtual memory.\n");
    exit(1);
  }
if ((key = ftok("/tmp", 'B')) == -1) {
    perror("ftok");
    exit(1);
}
  msgqid = msgget(key , IPC_CREAT | 0600);

  if (msgqid == -1) {
    perror("msgget");
    exit(1);
  }
  system("ipcs");
  sendmsg->mtype = 1L;
  (void)strcpy(sendmsg->mtext, "Test messages.");

  if(msgsnd(msgqid,sendmsg,strlen(sendmsg->mtext)+1,0) == -1){
    perror("msgsnd");
    rval = 1;
  }
//  msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);
//  system("ipcs");
  exit(rval);
}
