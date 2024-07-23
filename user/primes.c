// p = get a number from left neighbor
// print p
// loop:
//     n = get a number from left neighbor
//     if (p does not divide n)
//         send n to right neighbor

#include "kernel/types.h"
#include "user/user.h"

void prog(int ifd) {
    char buf;
    read(ifd, &buf, 1);  // assume no error
    int p = buf;
    printf("prime %d\n", p);

    int ofd = -1;
    while (read(ifd, &buf, 1) != 0) {
        int n = buf;
        if (n % p == 0) {
            continue;
        }
        if (ofd == -1) {
            int pi[2];
            pipe(pi);
            switch (fork()) {
                case -1:
                    fprintf(2, "fuck it fork()");
                    exit(-1);
                case 0:  // right process
                    close(pi[1]);
                    return prog(pi[0]);
                default:  // left(this) process
                    close(pi[0]);
                    ofd = pi[1];
            }
        }
        write(ofd, &buf, 1);
    }

    close(ifd);
    close(ofd);
    wait(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, 1);
    }
    close(p[1]);

    prog(p[0]);
    close(p[0]);

    exit(0);
}