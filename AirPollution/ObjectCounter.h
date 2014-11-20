#ifndef _OBJECT_COUNTER_H_
#define _OBJECT_COUNTER_H_

class ObjectCounter
{
protected:
  static int number;
  static int max_number;

public:
  static void registerObject();
  static void count();
  static void endFrame();
  static int getCount();
};

#endif

