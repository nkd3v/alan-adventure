#pragma once

#include "Entity.h"

class Buff
{
protected:
  std::string name;

  float lifetime;
  float lifetimeMax;

  Entity& entity;

public:
  Buff(float lifetimeMax, Entity& entity);
  virtual ~Buff();

  virtual const std::string& getName() const = 0;

  virtual void saveDefault() = 0;
  virtual void activate() = 0;
  virtual void restoreDefault() = 0;

  virtual bool endOfLife();
  virtual void updateLifetime();
  virtual void update();
  virtual void reset(float newLifetimeMax = 0.f);
};
