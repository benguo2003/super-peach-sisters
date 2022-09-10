#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class baseActor;
class Peach;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    void addPowerUp(int typeGoodie, int x, int y);
    void generateShell(int direction, int x, int y, baseActor* myself);
    void generatePiranhaFireball(int direction, int x, int y);
    void generatePeachFireball();
    bool releaseGoodie(baseActor* myself);
    bool hasSolidFooting(baseActor* myself, char direction);
    void enemyPeachBonk(baseActor* bonker);
    bool peachInX(baseActor* myself);
    bool peachInY(baseActor* myself);
    int determineDirection(baseActor* myself);
    
    string whatLevel();
    
    void peachTakeDamage();
    void grantInvincibility(int ticks) const;
    void grantShootPower() const;
    void grantJumpPower() const;
    
    void completedLevel();
    void wonGame();
    
    bool blocksPeachFalling(baseActor* myself);
    baseActor* moveOverlap(char direction, baseActor* myself, int numPixels);
    baseActor* overlap(baseActor* myself);
    bool overlapWithPeach(baseActor* myself);
    
private:
    Peach* m_peach;
    vector<baseActor*> objects;
    bool m_completedLevel;
    bool m_wonGame;
};

#endif // STUDENTWORLD_H_
