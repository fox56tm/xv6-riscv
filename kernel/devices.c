#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

static struct spinlock statlock, rtclock;
static uint64 nullstat_total;

static int
pseudoread(int user_dst, uint64 dst, int n, int minor)
{
  static char zeros[128];
  uint64 v;

  switch(minor){
  case MIN_NULL:
    return 0;
  case MIN_ZERO:
    for(int done = 0; done < n; ){
      int c = n - done;
      if(c > (int)sizeof(zeros))
        c = sizeof(zeros);
      if(either_copyout(user_dst, dst + done, zeros, c) < 0)
        return -1;
      done += c;
    }
    return n;
  case MIN_NULLSTAT:
    if(n != sizeof(uint64))
      return -1;
    acquire(&statlock);
    v = nullstat_total;
    release(&statlock);
    if(either_copyout(user_dst, dst, &v, sizeof(v)) < 0)
      return -1;
    return sizeof(v);
  }
  return -1;
}

static int
pseudowrite(int user_src, uint64 src, int n, int minor)
{
  switch(minor){
  case MIN_NULL:
    return n;
  case MIN_NULLSTAT:
    if(n < 0) return -1; 
    acquire(&statlock);
    nullstat_total += n;
    release(&statlock);
    return n;
  }
  return -1;   // zero or undefined minor
}

static int
rtcread(int user_dst, uint64 dst, int n, int minor)
{
  uint32 lo, hi;
  uint64 t;

  if(n != sizeof(uint64))
    return -1;
  acquire(&rtclock);
  lo = *(volatile uint32 *)RTC_LOW;
  hi = *(volatile uint32 *)RTC_HIGH;
  release(&rtclock);
  t = ((uint64)hi << 32) | lo;
  if(either_copyout(user_dst, dst, &t, sizeof(t)) < 0)
    return -1;
  return sizeof(t);
}

static int
rtcwrite(int user_src, uint64 src, int n, int minor)
{
  return -1;
}


void
devinit(void)
{
  initlock(&statlock, "nullstat");
  initlock(&rtclock, "rtc");
  devsw[PSEUDO].read = pseudoread;
  devsw[PSEUDO].write = pseudowrite;
  devsw[RTCDEV].read = rtcread;
  devsw[RTCDEV].write = rtcwrite;
}
