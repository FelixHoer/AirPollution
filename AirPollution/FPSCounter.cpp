#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "FPSCounter.h"

FPSCounter::FPSCounter() : old_count(0), current_count(0), current_time(0)
{}

void FPSCounter::count()
{
  int time_since_start = glutGet(GLUT_ELAPSED_TIME);
  if (time_since_start - current_time < 1000)
  {
    current_count++;
  }
  else
  {
    current_time = time_since_start;
    old_count = current_count;
    current_count = 1;
  }
}

int FPSCounter::getCount()
{
  return old_count;
}