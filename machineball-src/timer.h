// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _TIMER_H_
#define _TIMER_H_

#include <allegro.h>

inline void tickf(void);

class Timer
{
 public:
  unsigned long ticks;
  double ticks_per_sec;
  
  void install(double tps=596590.5) { install_int_ex(tickf, (long int)((double)1193181.0/tps)); ticks_per_sec=tps; ticks=0; }
  double seconds(void) { return (double)ticks/ticks_per_sec; }
  void reset(void) { ticks=0; }
};

extern Timer timer;

void tickf(void) { timer.ticks++; }

#endif

