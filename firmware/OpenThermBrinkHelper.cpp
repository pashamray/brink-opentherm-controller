/**
  OpenThermBrinkHelper.cpp
  https://github.com/pashamray/brink-opentherm-controller

  Licensed under MIT license
  Copyright 2025, Pavlo Shamrai
**/

#include "OpenThermBrinkHelper.h"

OpenThermBrinkHelper::OpenThermBrinkHelper(int inPin, int outPin): OpenTherm(inPin, outPin)
{
}

void OpenThermBrinkHelper::loop()
{
  pushParameters();
  pullParameters();
}

unsigned int OpenThermBrinkHelper::getSpeed()
{
  return params.getSpeed;
}

void OpenThermBrinkHelper::setSpeed(unsigned int speed)
{
  if (params.setSpeed == speed) {
    return;
  }

  params.needPush = true;
  params.setSpeed = speed;
}

void OpenThermBrinkHelper::pushParameters()
{
  if (params.needPush == false) {
    return;
  }

  setRelativeVentilationLevel(params.setSpeed);
  params.needPush = false;
}

void OpenThermBrinkHelper::pullParameters()
{
  if (params.needPush == true) {
    return;
  }

  params.getSpeed = getRelativeVentilationLevel();
}

unsigned int OpenThermBrinkHelper::getRelativeVentilationLevel()
{
  unsigned long response = sendRequest(buildRequest(OpenThermMessageType::READ_DATA, OpenThermMessageID::RelVentLevel, 0));

  OpenThermResponseStatus responseStatus = getLastResponseStatus();
  if (responseStatus != OpenThermResponseStatus::SUCCESS) {
    return 0;
  }

  return getUInt(response);
}

unsigned int OpenThermBrinkHelper::setRelativeVentilationLevel(unsigned int level)
{
  unsigned long response = sendRequest(buildRequest(OpenThermMessageType::WRITE_DATA, OpenThermMessageID::Vset, min(level, (unsigned int) 100)));

  OpenThermResponseStatus responseStatus = getLastResponseStatus();
  if (responseStatus != OpenThermResponseStatus::SUCCESS) {
    return 0;
  }

  return getUInt(response);
}