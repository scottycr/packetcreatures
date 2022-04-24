#ifndef CREATURE_H_
#define CREATURE_H_

#include <string>
#include <stdint.h>
#include <array>
#include <map>
#include <vector>
#include <unordered_map>

namespace creature {

enum move_t { 
    EMPTY, 
    PHYSICAL, 
    STATUS 
};

std::string getMoveT(const move_t &mt);

enum elemental_t {
    NONE,
    NORMAL,
    GRASS,
    FIRE,
    WATER
};

std::string getElementalT(const elemental_t &et);

const int MAXNAMESIZE = 13;

class MoveList;
extern MoveList masterList;

class CreatureMove {
private:
    int uid;
    static int nextUID;
    char name[MAXNAMESIZE];
    move_t category;
    elemental_t type;
    int16_t power, uses, accuracy;
public:
    CreatureMove();
    CreatureMove(
        std::string _name, 
        move_t _category, 
        elemental_t _type, 
        int16_t _power, 
        int16_t _uses, 
        int16_t _accuracy
    );

    int getUID() const { return uid; }
    std::string getName() const { return std::string(name); }
    move_t getCategory() const { return category; }
    elemental_t getType() const { return type; }
    int16_t getPower() const { return power; }
    int16_t getUses() const { return uses; }
    void useMove() { if (uses > 0) uses--; }
    int16_t getAccuracy() const { return accuracy; } 

    void printInfo();
};

class Creature {
private:
    int uid;
    static int nextUID;
    char name[MAXNAMESIZE], nickName[MAXNAMESIZE];
    elemental_t type;
    int16_t health, attack, defense, speed;
    std::array<CreatureMove, 4> moves;
    int16_t usableMoves;

public:
    Creature();
    Creature(
        std::string _name, 
        std::string _nickName, 
        elemental_t _type, 
        int16_t _health,
        int16_t _attack, 
        int16_t _defense, 
        int16_t _speed
    );
    
    int getUID() const { return uid; }
    std::string getName() const { return std::string(name); }
    std::string getNickName() const { return std::string(nickName); }
    void setNickName(std::string newNickName);
    elemental_t getType() const { return type; }
    int16_t getHealth() const { return health; }
    void setHealth(int16_t newHealth) { health = newHealth; }
    void decreaseHealth(int damage) { health -= damage; }
    int16_t getAttack() const { return attack; }
    void setAttack(int16_t newAttack) { attack = newAttack; }
    int8_t getDefense() const { return defense; }
    void setDefense(int16_t newDefense) { defense = newDefense; }
    int16_t getSpeed() const { return speed; }
    void setSpeed(int16_t newSpeed) { speed = newSpeed; }

    CreatureMove* getMove(int index) { return &moves[index]; }
    void addMove(CreatureMove newMove);
    void setMove(CreatureMove newMove, int index) { moves[index] = newMove; }
    std::string getMoveName(int index) { return moves[index].getName(); }
    int16_t getNumberOfMoves() const { return usableMoves; }

    void printMoveScreen();
    void printInfo();
};

class MoveList {
private:
    std::unordered_map<int, CreatureMove> moves;
    std::unordered_map<int, std::vector<CreatureMove> > creatureMovePools;
public:
    MoveList() { moves.insert(std::make_pair(-1, CreatureMove())); }
    void addMove(CreatureMove moves);
    CreatureMove getMove(int uid) { return moves.at(uid); }
    // Maybe don't need this?
    // std::vector<CreatureMove*> getMovePool(Creature* c);
    void initCreatureMovePool(int uid);
    void addCreatureMovePool(int uid, std::vector<CreatureMove> moves);
};

void dealDamage(Creature &c1, Creature &c2, CreatureMove *c1Move, CreatureMove *c2Move);

}

#endif
