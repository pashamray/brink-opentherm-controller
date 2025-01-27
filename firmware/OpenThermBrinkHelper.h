/**
  OpenThermBrinkHelper.h
  https://github.com/pashamray/brink-opentherm-controller

  Licensed under MIT license
  Copyright 2025, Pavlo Shamrai
**/

#ifndef OpenThermBrinkHelper_h
#define OpenThermBrinkHelper_h

#include <OpenTherm.h>

class OpenThermBrinkHelper
{
public:
  OpenThermBrinkHelper(OpenTherm *ot);
  ~OpenThermBrinkHelper();
  void loop();
  unsigned int getSpeed();
  void setSpeed(unsigned int speed);
private:
  OpenTherm *ot;
  struct {
    bool needPush = false;
    unsigned int setSpeed = 0;
    unsigned int getSpeed = 0;
  } params;

  void pushParameters();
  void pullParameters();
  unsigned int getRelativeVentilationLevel();
  unsigned int setRelativeVentilationLevel(unsigned int level);
};

#endif // OpenThermBrinkHelper_h