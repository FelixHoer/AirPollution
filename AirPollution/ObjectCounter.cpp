#include <stdio.h>
#include <stdlib.h>

#include "ObjectCounter.h"

int ObjectCounter::number = 0;
int ObjectCounter::max_number = 0;

void ObjectCounter::registerObject()
{
  max_number++;
}

void ObjectCounter::count()
{
  number++;
}

void ObjectCounter::endFrame()
{
  number = 0;
}

int ObjectCounter::getCount()
{
  return number;
}