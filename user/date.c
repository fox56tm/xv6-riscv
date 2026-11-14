#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

static int
leap(int y)
{
  return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

static void
pn(int v, int w)
{
  int p = 1;
  for(int i = 1; i < w; i++)
    p *= 10;
  for(; p > 1 && v < p; p /= 10)
    printf("0");
  printf("%d", v);
}

int
main(void)
{
  uint64 t;
  int fd = open("/rtc", O_RDONLY);
  if(fd < 0 || read(fd, &t, sizeof(t)) != sizeof(t)){
    fprintf(2, "date: cannot read rtc\n");
    exit(1);
  }
  close(fd);

  uint64 secs = t / 1000000000ULL;
  int ns = t % 1000000000ULL;
  int days = secs / 86400;
  int rem = secs % 86400;

  int y = 1970;
  while(days >= (leap(y) ? 366 : 365)){
    days -= leap(y) ? 366 : 365;
    y++;
  }

  int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(leap(y))
    mdays[1] = 29;
  int m = 0;
  while(days >= mdays[m]){
    days -= mdays[m];
    m++;
  }
  int d = days + 1;
  m++;

  pn(d, 2); printf(".");
  pn(m, 2); printf(".");
  pn(y, 4); printf(" ");
  pn(rem / 3600, 2); printf(":");
  pn(rem % 3600 / 60, 2); printf(":");
  pn(rem % 60, 2); printf(".");
  pn(ns, 9); printf("\n");
  exit(0);
}
