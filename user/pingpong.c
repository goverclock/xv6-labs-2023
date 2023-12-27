// Add the program to UPROGS in Makefile.
// Use pipe to create a pipe.
// Use fork to create a child.
// Use read to read from a pipe, and write to write to a pipe.
// Use getpid to find the process ID of the calling process.
// User programs on xv6 have a limited set of library functions available to them.
// You can see the list in user/user.h; the source (other than for system calls) is
// in user/ulib.c, user/printf.c, and user/umalloc.c.

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int p1[2];  // parent to child
  int p2[2];  // child to parent
  pipe(p1);
  pipe(p2);

  char buf[2];
  int pid = fork();
  switch (pid) {
    case -1:
      fprintf(2, "failed to fork()");
      exit(-1);
    case 0:   // child
      close(p1[1]);
      close(p2[0]);

      // read a byte from parent
      read(p1[0], buf, 1);
      printf("%d: received ping\n", getpid());
      close(p1[0]);

      // send pong
      write(p2[1], buf, 1);
      close(p2[1]);
      break;

    default:  // parent
      close(p2[1]);
      close(p1[0]);

      // send a byte to child
      write(p1[1], "x", 1);
      close(p1[1]);

      // read pong
      read(p2[0], buf, 1);
      close(p2[0]);
      printf("%d: received pong\n", getpid());
  }


  exit(0);
}