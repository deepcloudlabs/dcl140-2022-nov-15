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
  struct msgbuf *recvmsg;
  int msgqid;
  int rval = 0;

  recvmsg = (struct msgbuf *)malloc(sizeof(long)+BUFSIZE);
if ((key = ftok("/tmp", 'B')) == -1) {
    perror("ftok");
    exit(1);
}

  msgqid = msgget( key, IPC_CREAT | 0600);

  if (msgqid == -1) {
    perror("msgget");
    exit(1);
  }
  system("ipcs");

  if (msgrcv(msgqid, recvmsg, BUFSIZE, 1L, 0) == -1) {
    perror("msgrcv");
    rval = 1;
  }
  printf("\nReceived message *%s*\n\n", recvmsg->mtext);

//  msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);
//  system("ipcs");
  exit(rval);
}
