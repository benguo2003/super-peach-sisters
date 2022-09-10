#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_peach(nullptr), m_completedLevel(false), m_wonGame(false)
{}

int StudentWorld::init()
{
     m_completedLevel = false;
     m_wonGame = false;
     Level lev(assetPath());
     Level::LoadResult result = lev.loadLevel(whatLevel());
     if (result == Level::load_fail_file_not_found){
         cerr << "Could not find level01.txt data file" << endl;
         return GWSTATUS_LEVEL_ERROR;
     }
     else if (result == Level::load_fail_bad_format){
         cerr << "level01.txt is improperly formatted" << endl;
         return GWSTATUS_LEVEL_ERROR;
     }
     else if (result == Level::load_success)
     {
         cerr << "Successfully loaded level" << endl;
         Level::GridEntry ge;
         for(int i = 0; i < GRID_WIDTH; i++){
             for(int j = 0; j < GRID_HEIGHT; j++){
                 ge = lev.getContentsOf(i, j);
                 switch(ge)
                 {
                     case Level::empty:
                         break;
                     case Level::peach:{
                         m_peach = new Peach(IID_PEACH, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 0, 1, this);
                         break;
                     }
                     case Level::koopa:{
                         Koopa* k = new Koopa(IID_KOOPA, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, randInt(0, 1) * 180, 1, 0, this);
                         objects.push_back(k);
                         break;
                     }
                     case Level::goomba:{
                         Goomba* g = new Goomba(IID_GOOMBA, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, randInt(0, 1) * 180, 1, 0, this);
                         objects.push_back(g);
                         break;
                     }
                     case Level::piranha:{
                         Piranha* p = new Piranha(IID_PIRANHA, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, randInt(0, 1) * 180, 1, 0, this);
                         objects.push_back(p);
                         break;
                     }
                     case Level::block:{
                         Block* b = new Block(IID_BLOCK, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 2, 1, this);
                         objects.push_back(b);
                         break;
                     }
                     case Level::star_goodie_block:{
                         powerBlock* p = new powerBlock(IID_BLOCK, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 2, 1, this, 3);
                         objects.push_back(p);
                         break;
                     }
                     case Level::mushroom_goodie_block:{
                         powerBlock* p = new powerBlock(IID_BLOCK, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 2, 1, this, 2);
                         objects.push_back(p);
                         break;
                     }
                     case Level::flower_goodie_block:{
                         powerBlock* p = new powerBlock(IID_BLOCK, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 2, 1, this, 1);
                         objects.push_back(p);
                         break;
                     }
                     case Level::pipe:{
                         Pipe* p = new Pipe(IID_PIPE, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 2, 1, this);
                         objects.push_back(p);
                         break;
                     }
                     case Level::flag:{
                         Flag* f = new Flag(IID_FLAG, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 1, 1, this);
                         objects.push_back(f);
                         break;
                     }
                     case Level::mario:{
                         Mario* m = new Mario(IID_MARIO, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0, 1, 1, this);
                         objects.push_back(m);
                         break;
                     }
                 }
             }
         }
     }
     return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if(m_wonGame){
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    if(m_completedLevel){
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if(m_peach->isAlive())
        m_peach->doSomething();
    unsigned long size = objects.size();
    for(int i = 0; i < size; i++){
        if(objects[i]->isAlive()){
            objects[i]->doSomething();
            if(!m_peach->isAlive()){
                decLives();
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    
    for(vector<baseActor*>::iterator it = objects.begin(); it != objects.end(); ++it){
        if(!(*it)->isAlive()){
            delete *it;
            it = objects.erase(it);
            --it;
        }
    }
    
    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << setw(6) << getScore();
    if(m_peach->hasStarPower())
        oss << " StarPower!";
    if(m_peach->hasJumpPower())
        oss << " JumpPower!";
    if(m_peach->hasShootPower())
        oss << " ShootPower!";
    string s = oss.str();
    setGameStatText(s);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for(int i = 0; i < objects.size(); i++){
        delete objects[i];
    }
    objects.clear();
    objects.shrink_to_fit();
    delete m_peach;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

void StudentWorld::completedLevel(){
    m_completedLevel = true;
}

void StudentWorld::wonGame(){
    m_wonGame = true;
}

void StudentWorld::addPowerUp(int typeGoodie, int x, int y){
    if(typeGoodie == 1){
        Flower* f = new Flower(IID_FLOWER, x, y + 8, 0, 1, 1, this);
        objects.push_back(f);
    }
    else if(typeGoodie == 2){
        Mushroom* m = new Mushroom(IID_MUSHROOM, x, y + 8, 0, 1, 1, this);
        objects.push_back(m);
    }
    else{
        Star* s = new Star(IID_STAR, x, y + 8, 0, 1, 1, this);
        objects.push_back(s);
    }
}

void StudentWorld::generateShell(int direction, int x, int y, baseActor* myself){
    if(!myself->isAlive()){
        Shell* s = new Shell(IID_SHELL, x, y, direction, 1, 1, this);
        objects.push_back(s);
    }
}

void StudentWorld::generatePiranhaFireball(int direction, int x, int y){
    PiranhaFireball* p = new PiranhaFireball(IID_PIRANHA_FIRE, x, y, direction, 1, 1, this);
    objects.push_back(p);
}

void StudentWorld::generatePeachFireball(){
    int x = m_peach->getX();
    int y = m_peach->getY();
    int direction = m_peach->getDirection();
    if(direction == 0){
        PeachFireball* p = new PeachFireball(IID_PEACH_FIRE, x + 4, y, direction, 1, 1, this);
        if(overlap(p) != p)
            delete p;
        else
            objects.push_back(p);
    }
    if(direction == 180){
        PeachFireball* p = new PeachFireball(IID_PEACH_FIRE, x - 4, y, direction, 1, 1, this);
        if(overlap(p) != p)
            delete p;
        else
            objects.push_back(p);
    }
}

void StudentWorld::enemyPeachBonk(baseActor* bonker){
    if(m_peach->isInvincible()){
        playSound(SOUND_PLAYER_KICK);
        bonker->takeDamage();
    }
    else{
        m_peach->takeDamage();
    }
}

bool StudentWorld::peachInX(baseActor* myself){
    if(m_peach->getX() <= myself->getX() + (8 * SPRITE_WIDTH) && m_peach->getX() >= myself->getX() - (8 * SPRITE_WIDTH))
        return true;
    return false;
}

bool StudentWorld::peachInY(baseActor* myself){
    if(m_peach->getY() <= myself->getY() + (1.5 * SPRITE_HEIGHT) && m_peach->getY() >= myself->getY() - (1.5 * SPRITE_HEIGHT))
        return true;
    return false;
}

int StudentWorld::determineDirection(baseActor* myself){
    if(m_peach->getX() <= myself->getX())
        return 180;
    else
        return 0;
}

string StudentWorld::whatLevel(){
    ostringstream oss;
    oss << "level";
    oss.fill('0');
    oss << setw(2) << getLevel();
    oss << ".txt";
    string s = oss.str();
    return s;
}

bool StudentWorld::releaseGoodie(baseActor* myself){
    if(myself->getY() == m_peach->getY() + SPRITE_HEIGHT){
        return true;
    }
    return false;
}

bool StudentWorld::hasSolidFooting(baseActor* myself, char direction){
    for(int i = 0; i < objects.size(); i++){
        if(direction == 'l'){
            if((myself->getX() - 1 >= objects[i]->getX() && myself->getX() - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() - 1 >= objects[i]->getY() && myself->getY() - 1 <= objects[i]->getY() + SPRITE_HEIGHT - 1) && (objects[i]->overlappable() == false))
                return true;
        }
        else{
            if((myself->getX() + SPRITE_WIDTH >= objects[i]->getX() && myself->getX() + SPRITE_WIDTH <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() - 1 >= objects[i]->getY() && myself->getY() - 1 <= objects[i]->getY() + SPRITE_HEIGHT - 1) && (objects[i]->overlappable() == false))
                return true;
        }
    }
    return false;
}

baseActor* StudentWorld::moveOverlap(char direction, baseActor* myself, int numPixels){
    if(direction == 'r'){
        // bottom right
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() + numPixels + SPRITE_WIDTH - 1 >= objects[i]->getX()) && myself->getX() + numPixels + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
        // bottom left
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() + numPixels >= objects[i]->getX()) && myself->getX() + numPixels <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
        // top left
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() + numPixels >= objects[i]->getX()) && myself->getX() + numPixels <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
        // top right
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() + numPixels + SPRITE_WIDTH - 1 >= objects[i]->getX()) && myself->getX() + numPixels + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() + SPRITE_HEIGHT - 1 <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
    }
    else if(direction == 'l'){
        // bottom left
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() - numPixels >= objects[i]->getX()) && myself->getX() - numPixels <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
        // bottom right
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() - numPixels + SPRITE_WIDTH - 1 >= objects[i]->getX()) && myself->getX() - numPixels + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
        // top left
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() - numPixels >= objects[i]->getX()) && myself->getX() - numPixels <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
        // top right
        for(int i = 0; i < objects.size(); i++){
            if(((myself->getX() - numPixels + SPRITE_WIDTH - 1 >= objects[i]->getX()) && myself->getX() - numPixels + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() + SPRITE_HEIGHT - 1 <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
    }
    else if(direction == 'u'){
        for(int i = 0; i < objects.size(); i++){
            if((myself->getY() + numPixels + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() + numPixels + SPRITE_HEIGHT - 1 <= objects[i]->getY() + SPRITE_HEIGHT - 1) && ((myself->getX() >= objects[i]->getX() && myself->getX() <= objects[i]->getX() + SPRITE_WIDTH - 1) || (myself->getX() + SPRITE_WIDTH - 1 >= objects[i]->getX() && myself->getX() + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1))){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
    }
    else if(direction == 'd'){
        for(int i = 0; i < objects.size(); i++){
            if((myself->getY() - numPixels <= objects[i]->getY() + SPRITE_HEIGHT - 1 && myself->getY() - numPixels >= objects[i]->getY()) && ((myself->getX() >= objects[i]->getX() && myself->getX() <= objects[i]->getX() + SPRITE_WIDTH - 1) || (myself->getX() + SPRITE_WIDTH - 1 >= objects[i]->getX() && myself->getX() + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1))){
                if(objects[i] != myself)
                    return objects[i];
            }
        }
    }
    return myself;
}


bool StudentWorld::overlapWithPeach(baseActor* myself){
    if(((myself->getX() + SPRITE_WIDTH - 1 >= m_peach->getX()) && myself->getX() + SPRITE_WIDTH - 1 <= m_peach->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= m_peach->getY() && myself->getY() <= m_peach->getY() + SPRITE_HEIGHT - 1))
        return true;
    if(((myself->getX() >= m_peach->getX()) && myself->getX() <= m_peach->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= m_peach->getY() && myself->getY() <= m_peach->getY() + SPRITE_HEIGHT - 1))
        return true;
    
    return false;
}

baseActor* StudentWorld::overlap(baseActor* myself){
    // bottom right
    for(int i = 0; i < objects.size(); i++){
        if(((myself->getX() + SPRITE_WIDTH - 1 >= objects[i]->getX()) && myself->getX() + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
            if(objects[i] != myself)
                return objects[i];
        }
    }
    // bottom left
    for(int i = 0; i < objects.size(); i++){
        if(((myself->getX() >= objects[i]->getX()) && myself->getX() <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
            if(objects[i] != myself)
                return objects[i];
        }
    }
    // top left
    for(int i = 0; i < objects.size(); i++){
        if(((myself->getX() >= objects[i]->getX()) && myself->getX() <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
            if(objects[i] != myself)
                return objects[i];
        }
    }
    // top right
    for(int i = 0; i < objects.size(); i++){
        if(((myself->getX() + SPRITE_WIDTH - 1 >= objects[i]->getX()) && myself->getX() + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1) && (myself->getY() + SPRITE_HEIGHT - 1 >= objects[i]->getY() && myself->getY() + SPRITE_HEIGHT - 1 <= objects[i]->getY() + SPRITE_HEIGHT - 1)){
            if(objects[i] != myself)
                return objects[i];
        }
    }
    return myself;
}

bool StudentWorld::blocksPeachFalling(baseActor* myself){
    int numPixels = 4;
    for(int i = 0; i < objects.size(); i++){
        if((myself->getY() - numPixels <= objects[i]->getY() + SPRITE_HEIGHT - 1 && myself->getY() - numPixels >= objects[i]->getY()) && ((myself->getX() >= objects[i]->getX() && myself->getX() <= objects[i]->getX() + SPRITE_WIDTH - 1) || (myself->getX() + SPRITE_WIDTH - 1 >= objects[i]->getX() && myself->getX() + SPRITE_WIDTH - 1 <= objects[i]->getX() + SPRITE_WIDTH - 1))){
            if(!objects[i]->overlappable())
                return false;
        }
    }
    return true;
}

void StudentWorld::peachTakeDamage(){
    if(!m_peach->isInvincible())
        m_peach->takeDamage();
}
void StudentWorld::grantInvincibility(int ticks) const{
    m_peach->gainInvicibility(ticks);
}

void StudentWorld::grantShootPower() const{
    m_peach->gainShootPower();
}
void StudentWorld::grantJumpPower() const{
    m_peach->gainJumpPower();
}
