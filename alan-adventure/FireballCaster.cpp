#include "stdafx.h"
#include "Bow.h"

Bow::Bow(Entity& entity) :
  Weapon(.5f, entity)
{
  defaultCooldown = .5f;
  rapidCooldown = .1f;
  rapidFireCountdown = 0.f;

  shootArrowSound.setBuffer(am.getSoundBuffer("SHOOT_ARROW"));
  shootArrowSound.setVolume(20);

  am.loadTexture("BOW", "assets/Weapon/bow.png");
  sprite.setTexture(am.getTexture("BOW"));
}

Bow::~Bow()
{
}

const std::vector<std::unique_ptr<Bullet>>& Bow::getBullets() const
{
  return bullets;
}

void Bow::updateCooldown()
{
  rapidFireCountdown -= Time.deltaTime;
  if (rapidFireCountdown < 0.f)
    rapidFireCountdown = 0.f;

  if (rapidFireCountdown > 0.f)
    cooldownMax = rapidCooldown;
  else
    cooldownMax = defaultCooldown;

  cooldown -= Time.deltaTime;
  if (cooldown <= 0.f)
    cooldown = 0.f;
}

void Bow::activateRapidFire(float time)
{
  rapidFireCountdown = time;
}

void Bow::shoot(const sf::Vector2f& pos, float angle)
{
  if (canShoot())
  {
    bullets.emplace_back(std::make_unique<Bullet>(pos, 1000.f, angle));
    shootArrowSound.play();
  }
}

void Bow::update(const float& dt)
{
  updateCooldown();
  sprite.setOrigin(getSize() / 2.f);

  if (entity.getMovementComponent()->getState(MOVING_RIGHT)
    || entity.getMovementComponent()->getState(IDLE))
  {
    sprite.setPosition(entity.getCenter() + sf::Vector2f(15.f, 0.f));
    sprite.setRotation(0.f);
  }
  else
  {
    sprite.setPosition(entity.getCenter() + sf::Vector2f(-15.f, 0.f));
    sprite.setRotation(180.f);
  }

  for (auto it = bullets.begin(); it != bullets.end();)
  {
    if ((*it)->endofLife())
      it = bullets.erase(it);
    else
      it++;
  }

  for (auto& bullet : bullets)
    bullet->update(dt);
}

void Bow::render(sf::RenderTarget& target)
{
  target.draw(sprite);

  for (auto& bullet : bullets)
    bullet->render(target);
}
