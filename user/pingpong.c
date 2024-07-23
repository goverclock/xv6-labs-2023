#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p2c[2];  // parent to child
    int c2p[2];  // child to parent
    pipe(p2c);
    pipe(c2p);

    char buf;
    int pid = fork();
    switch (pid) {
        case -1:
            fprintf(2, "failed to fork()");
            exit(-1);
        case 0:  // child
            close(p2c[1]);
            close(c2p[0]);

            // read a byte from parent
            read(p2c[0], &buf, 1);
            close(p2c[0]);
            printf("%d: received ping\n", getpid());

            // send pong
            write(c2p[1], &buf, 1);
            close(c2p[1]);
            break;

        default:  // parent
            close(c2p[1]);
            close(p2c[0]);

            // send a byte to child
            write(p2c[1], "x", 1);
            close(p2c[1]);

            // read pong
            read(c2p[0], &buf, 1);
            close(c2p[0]);
            printf("%d: received pong\n", getpid());
    }

    exit(0);
}