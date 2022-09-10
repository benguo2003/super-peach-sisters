#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <iostream>

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// baseActor implementation

baseActor::baseActor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): GraphObject(imageID, startX, startY, dir, depth, size), m_world(world), m_alive(true)
{}

StudentWorld* baseActor::getWorld() const{
    return m_world;
}

bool baseActor::isAlive(){
    return m_alive;
}

void baseActor::die(){
    m_alive = false;
}

// Block implementation

Block::Block(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Block::doSomething(){}

void Block::bonk(){
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

bool Block::damageable(){
    return false;
}

bool Block::overlappable(){
    return false;
}

void Block::takeDamage(){}

// powerBlock implementation

powerBlock::powerBlock(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world, int typeGoodie): Block(imageID, startX, startY, dir, depth, size, world), m_typeGoodie(typeGoodie), m_releasedGoodie(false)
{}

void powerBlock::bonk(){
    if(!m_releasedGoodie && getWorld()->releaseGoodie(this)){
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        getWorld()->addPowerUp(m_typeGoodie, getX(), getY());
        m_releasedGoodie = true;
    }
}

// movingEnemies Implementation

movingEnemies::movingEnemies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void movingEnemies::doSomething(){
    if(!isAlive())
        return;
    if(getWorld()->overlapWithPeach(this)){
        bonk();
    }
    if(getDirection() == 0){
        if(getWorld()->moveOverlap('r', this, 1) == this || getWorld()->moveOverlap('r', this, 1)->overlappable()){
            if(getWorld()->hasSolidFooting(this, 'r')){
                moveTo(getX() + 1, getY());
            }
            else{
                setDirection(180);
                if(getWorld()->moveOverlap('l', this, 1) == this || getWorld()->moveOverlap('l', this, 1)->overlappable())
                    moveTo(getX() - 1, getY());
                else
                    return;
            }
        }
        else{
            setDirection(180);
            moveTo(getX() - 1, getY());
        }
    }
    else if(getDirection() == 180){
        if(getWorld()->moveOverlap('l', this, 1) == this || getWorld()->moveOverlap('l', this, 1)->overlappable()){
            if(getWorld()->hasSolidFooting(this, 'l'))
                moveTo(getX() - 1, getY());
            else{
                setDirection(0);
                if(getWorld()->moveOverlap('l', this, 1) == this || getWorld()->moveOverlap('l', this, 1)->overlappable())
                    moveTo(getX() + 1, getY());
                else
                    return;
            }
        }
        else{
            setDirection(0);
            moveTo(getX() + 1, getY());
        }
    }
}

void movingEnemies::bonk(){
    getWorld()->enemyPeachBonk(this);
    doActivity();
}

bool movingEnemies::damageable(){
    return true;
}

bool movingEnemies::overlappable(){
    return true;
}

void movingEnemies::takeDamage(){
    getWorld()->increaseScore(100);
    die();
    doActivity();
}

// Goomba Implementation

Goomba::Goomba(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): movingEnemies(imageID, startX, startY, dir, depth, size, world)
{}

void Goomba::doActivity(){}

// Koopa implementation

Koopa::Koopa(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): movingEnemies(imageID, startX, startY, dir, depth, size, world)
{}

void Koopa::doActivity(){
    getWorld()->generateShell(getDirection(), getX(), getY(), this);
}

// Shell implementation

Shell::Shell(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Shell::doSomething(){
    if(getWorld()->overlap(this) != this && getWorld()->overlap(this)->damageable()){
        getWorld()->overlap(this)->takeDamage();
        die();
    }
    else{
        if(getWorld()->moveOverlap('d', this, 2) == this)
            moveTo(getX(), getY() - 2);
        if(getDirection() == 0){
            if(getWorld()->moveOverlap('r', this, 2) != this && !getWorld()->moveOverlap('r', this, 2)->overlappable()){
                die();
                return;
            }
            else
                moveTo(getX() + 2, getY());
        }
        else{
            if(getWorld()->moveOverlap('l', this, 2) != this && !getWorld()->moveOverlap('l', this, 2)->overlappable()){
                die();
                return;
            }
            else
                moveTo(getX() - 2, getY());
        }
    }
}

void Shell::bonk(){
}

bool Shell::damageable(){
    return false;
}

bool Shell::overlappable(){
    return true;
}

void Shell::takeDamage(){}

// Piranha implementation

Piranha::Piranha(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world), m_fire_delay(0)
{}

void Piranha::doSomething(){
    if(!isAlive())
        return;
    
    increaseAnimationNumber();
    
    if(getWorld()->overlapWithPeach(this)){
        bonk();
        return;
    }
    
    if(getWorld()->peachInY(this)){
        setDirection(getWorld()->determineDirection(this));
        if(m_fire_delay > 0){
            m_fire_delay--;
            return;
        }
        else{
            if(getWorld()->peachInX(this)){
                getWorld()->generatePiranhaFireball(getDirection(), getX(), getY());
                getWorld()->playSound(SOUND_PIRANHA_FIRE);
                m_fire_delay = 40;
            }
        }
    }
    else
        return;
    
}

void Piranha::bonk(){
    getWorld()->enemyPeachBonk(this);
}

bool Piranha::damageable(){
    return true;
}

bool Piranha::overlappable(){
    return true;
}

void Piranha::takeDamage(){
    getWorld()->increaseScore(100);
    die();
}

// Pipe implementation

Pipe::Pipe(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Pipe::doSomething(){
}

void Pipe::bonk(){
}

bool Pipe::damageable(){
    return false;
}

bool Pipe::overlappable(){
    return false;
}

void Pipe::takeDamage(){}

// Flag implementation

Flag::Flag(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Flag::doSomething(){
    if(isAlive()){
        if(getWorld()->overlapWithPeach(this)){
            getWorld()->increaseScore(1000);
            die();
            getWorld()->completedLevel();
        }
    }
}

void Flag::bonk(){}

bool Flag::damageable(){
    return false;
}

bool Flag::overlappable(){
    return true;
}

void Flag::takeDamage(){}

// Mario implementation

Mario::Mario(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Mario::doSomething(){
    if(isAlive()){
        if(getWorld()->overlapWithPeach(this)){
            getWorld()->increaseScore(1000);
            die();
            getWorld()->wonGame();
        }
    }
}

void Mario::bonk(){
}

bool Mario::damageable(){
    return false;
}

bool Mario::overlappable(){
    return true;
}

void Mario::takeDamage(){}

// Goodies implementation

Goodies::Goodies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Goodies::doSomething(){
    if(getWorld()->overlapWithPeach(this))
        bonk();
    if(getWorld()->moveOverlap('d', this, 2) == this || getWorld()->moveOverlap('d', this, 2)->overlappable())
        moveTo(getX(), getY() - 2);
    if(getDirection() == 0){
        if(getWorld()->moveOverlap('r', this, 2) == this || getWorld()->moveOverlap('r', this, 2)->overlappable())
            moveTo(getX() + 2, getY());
        else{
            setDirection(180);
            return;
        }
    }
    else if(getDirection() == 180){
        if(getWorld()->moveOverlap('l', this, 2) == this || getWorld()->moveOverlap('l', this, 2)->overlappable())
            moveTo(getX() - 2, getY());
        else{
            setDirection(0);
            return;
        }
    }
}

bool Goodies::damageable(){
    return false;
}

bool Goodies::overlappable(){
    return true;
}

void Goodies::takeDamage(){}

// Flower implementation

Flower::Flower(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): Goodies(imageID, startX, startY, dir, depth, size, world)
{}

void Flower::bonk(){
    getWorld()->increaseScore(50);
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
    getWorld()->grantShootPower();
    die();
}

// Mushroom implementation

Mushroom::Mushroom(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): Goodies(imageID, startX, startY, dir, depth, size, world)
{}

void Mushroom::bonk(){
    getWorld()->increaseScore(75);
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
    getWorld()->grantJumpPower();
    die();
}

// Star implementation

Star::Star(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): Goodies(imageID, startX, startY, dir, depth, size, world)
{}

void Star::bonk(){
    getWorld()->increaseScore(100);
    getWorld()->playSound(SOUND_PLAYER_POWERUP);
    getWorld()->grantInvincibility(150);
    die();
}

// Projectiles implementation

Projectiles::Projectiles(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): baseActor(imageID, startX, startY, dir, depth, size, world)
{}

void Projectiles::doSomething(){
    doActivity();
    if(isAlive()){
        if(getWorld()->moveOverlap('d', this, 2) == this || getWorld()->moveOverlap('d', this, 2)->overlappable())
            moveTo(getX(), getY() - 2);
        if(getDirection() == 0){
            if(getWorld()->moveOverlap('r', this, 2) == this || getWorld()->moveOverlap('r', this, 2)->overlappable())
                moveTo(getX() + 2, getY());
            else{
                die();
                return;
            }
        }
        else if(getDirection() == 180){
            if(getWorld()->moveOverlap('l', this, 2) == this || getWorld()->moveOverlap('l', this, 2)->overlappable())
                moveTo(getX() - 2, getY());
            else{
                die();
                return;
            }
        }
    }
}

bool Projectiles::damageable(){
    return false;
}

void Projectiles::bonk(){}

bool Projectiles::overlappable(){
    return true;
}

void Projectiles::takeDamage(){}

// PiranhaFireball implementation

PiranhaFireball::PiranhaFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): Projectiles(imageID, startX, startY, dir, depth, size, world)
{}

void PiranhaFireball::doActivity(){
    if(getWorld()->overlapWithPeach(this)){;
        die();
        getWorld()->peachTakeDamage();
        return;
    }
}

// PeachFireball implementation

PeachFireball::PeachFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world): Projectiles(imageID, startX, startY, dir, depth, size, world)
{}


void PeachFireball::doActivity(){
    if(getWorld()->overlap(this) != this && !getWorld()->overlap(this)->overlappable() && !getWorld()->overlap(this)->damageable()){
        die();
        return;
    }
    if(getWorld()->overlap(this) != this && getWorld()->overlap(this)->damageable()){
        die();
        getWorld()->overlap(this)->takeDamage();
        return;
    }
}

// Peach implementation

Peach::Peach(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* p_world): baseActor(imageID, startX, startY, dir, depth, size, p_world), m_hitpoint(1), m_jumpPower(false), m_starPower(false), m_shootPower(false), m_remaining_jump_distance(0), m_invincibility(false), m_tempInvincibility(false), m_invincibilityTime(0), m_tempInvincibilityTime(0), m_time_to_recharge_before_next_fire(0)
{}

void Peach::doSomething(){
    if(!isAlive())
        return;
    
    if(m_invincibilityTime > 0)
        m_invincibilityTime--;
    else
        m_invincibility = false;
    
    if(m_tempInvincibilityTime > 0)
        m_tempInvincibilityTime--;
    else
        m_tempInvincibility = false;
    
    if(m_time_to_recharge_before_next_fire > 0)
        m_time_to_recharge_before_next_fire--;
    
    if(getWorld()->overlap(this))
        getWorld()->overlap(this)->bonk();
    
    if(m_remaining_jump_distance > 0){
        if(getWorld()->moveOverlap('u', this, 4) != this && !getWorld()->moveOverlap('u', this, 4)->overlappable()){
            getWorld()->moveOverlap('u', this, 4)->bonk();
            m_remaining_jump_distance = 0;
        }
        else{
            moveTo(getX(), getY()+4);
            m_remaining_jump_distance--;
        }
    }
    else{
        if(getWorld()->blocksPeachFalling(this))
            moveTo(getX(), getY()-4);
    }
    int ch;
    if(getWorld()->getKey(ch)){
        switch (ch)
        {
            case KEY_PRESS_LEFT:{
                setDirection(180);
                if(getWorld()->moveOverlap('l', this, 4) == this || getWorld()->moveOverlap('l', this, 4)->overlappable()){
                    moveTo(getX()-4, getY());
                }
                else{
                    getWorld()->moveOverlap('l', this, 4)->bonk();
                }
                break;
            }
            case KEY_PRESS_RIGHT:{
                setDirection(0);
                if(getWorld()->moveOverlap('r', this, 4) == this || getWorld()->moveOverlap('r', this, 4)->overlappable()){
                    moveTo(getX()+4, getY());
                }
                else{
                    getWorld()->moveOverlap('r', this, 4)->bonk();
                }
                break;

            }
            case KEY_PRESS_UP:{
                if(m_remaining_jump_distance == 0){
                    if(((getWorld()->moveOverlap('u', this, 4) == this || getWorld()->moveOverlap('u', this, 4)->overlappable()) && (getWorld()->moveOverlap('d', this, 4) != this))){
                        getWorld()->playSound(SOUND_PLAYER_JUMP);
                        if(hasJumpPower())
                            m_remaining_jump_distance = 12;
                        else
                            m_remaining_jump_distance = 8;
                    }
                    else{
                        getWorld()->moveOverlap('u', this, 4)->bonk();
                    }
                }
                break;
            }
            case KEY_PRESS_SPACE:{
                if(hasShootPower()){
                    if(m_time_to_recharge_before_next_fire == 0){
                        getWorld()->playSound(SOUND_PLAYER_FIRE);
                        m_time_to_recharge_before_next_fire = 8;
                        getWorld()->generatePeachFireball();
                    }
                }
                break;
            }
        }
    }
}

bool Peach::isInvincible(){
    if(m_invincibility)
        return true;
    return false;
}

void Peach::gainInvicibility(int ticks){
    m_invincibility = true;
    m_invincibilityTime = ticks;
}

void Peach::gainTempInvicibility(int ticks){
    m_tempInvincibility = true;
    m_tempInvincibilityTime = ticks;
}

void Peach::gainJumpPower(){
    m_hitpoint = 2;
    m_jumpPower = true;
}

void Peach::gainShootPower(){
    m_hitpoint = 2;
    m_shootPower = true;
}

void Peach::takeDamage(){
    if(m_hitpoint == 2){
        getWorld()->playSound(SOUND_PLAYER_HURT);
        m_hitpoint--;
        gainTempInvicibility(10);
        m_jumpPower = false;
        m_shootPower = false;
    }
    else{
        die();
    }
}

void Peach::bonk(){
}

bool Peach::damageable(){
    return true;
}

bool Peach::overlappable(){
    return true;
}

bool Peach::hasStarPower(){
    if(isInvincible())
        return true;
    return false;
}

bool Peach::hasJumpPower(){
    if(m_jumpPower)
        return true;
    return false;
}

bool Peach::hasShootPower(){
    if(m_shootPower)
        return true;
    return false;
}
