#ifndef _FPS_COUNTER_H_
#define _FPS_COUNTER_H_

class FPSCounter
{
protected:
  int old_count;
  int current_count;
  int current_time;

public:
  FPSCounter();

  virtual void count();
  virtual int getCount();
};

#endif

