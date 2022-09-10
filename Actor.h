#ifndef ACTOR_H_
#define ACTOR_H_
#include <iostream>
#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

// base class for all actors
class baseActor: public GraphObject
{
public:
    baseActor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething() = 0;
    virtual bool damageable() = 0;
    virtual void bonk() = 0;
    virtual bool overlappable() = 0;
    virtual void takeDamage() = 0;
    void die();
    bool isAlive();
    StudentWorld* getWorld() const;
private:
    StudentWorld* m_world;
    bool m_alive;
};

class Peach: public baseActor
{
public:
    Peach(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
    void gainInvicibility(int ticks);
    void gainTempInvicibility(int ticks);
    void gainJumpPower();
    void gainShootPower();
    bool isInvincible();
    bool hasStarPower();
    bool hasJumpPower();
    bool hasShootPower();
private:
    int m_hitpoint;
    bool m_jumpPower;
    bool m_starPower;
    bool m_shootPower;
    int m_remaining_jump_distance;
    bool m_invincibility;
    bool m_tempInvincibility;
    int m_invincibilityTime;
    int m_tempInvincibilityTime;
    int m_time_to_recharge_before_next_fire;
};

// block class
class Block: public baseActor
{
public:
    Block(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
};

class powerBlock: public Block
{
public:
    powerBlock(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world, int typeGoodie);
    virtual void bonk();
private:
    int m_typeGoodie;
    bool m_releasedGoodie;
};

class movingEnemies: public baseActor
{
public:
    movingEnemies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    void doSomething();
    virtual void bonk();
    virtual bool damageable();
    virtual bool overlappable();
    virtual void takeDamage();
    virtual void doActivity() = 0;
};

class Goomba: public movingEnemies
{
public:
    Goomba(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doActivity();
};

class Koopa: public movingEnemies
{
public:
    Koopa(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doActivity();
};

class Shell: public baseActor
{
public:
    Shell(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual void bonk();
    virtual bool damageable();
    virtual bool overlappable();
    virtual void takeDamage();
};

class Piranha: public baseActor
{
public:
    Piranha(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
private:
    int m_fire_delay;
};

class Pipe: public baseActor
{
public:
    Pipe(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
};

class Flag: public baseActor
{
public:
    Flag(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
};

class Mario: public baseActor
{
public:
    Mario(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
};

class Goodies: public baseActor
{
public:
    Goodies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk() = 0;
    virtual bool overlappable();
    virtual void takeDamage();
};

class Flower: public Goodies
{
public:
    Flower(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void bonk();
};

class Mushroom: public Goodies
{
public:
    Mushroom(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void bonk();
};

class Star: public Goodies
{
public:
    Star(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void bonk();
};

class Projectiles: public baseActor
{
public:
    Projectiles(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doSomething();
    virtual bool damageable();
    virtual void bonk();
    virtual bool overlappable();
    virtual void takeDamage();
    virtual void doActivity() = 0;
};

class PiranhaFireball: public Projectiles
{
public:
    PiranhaFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doActivity();
};

class PeachFireball: public Projectiles
{
public:
    PeachFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
    virtual void doActivity();
};

#endif // ACTOR_H_
