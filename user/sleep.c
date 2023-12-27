#include "kernel/types.h"
#include "user/user.h"

static const char err_msg[] = "fuck you\n";

int main(int argc, char *argv[]) {
  if (argc == 1) {
    write(2, err_msg, strlen(err_msg));
    exit(-1);
  }

  sleep(atoi(argv[1]));

  exit(0);
}