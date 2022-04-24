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

enum elemental_t {
    NONE,
    NORMAL,
    GRASS,
    FIRE,
    WATER
};

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
    uint8_t power, uses, accuracy;
public:
    CreatureMove();
    CreatureMove(
        std::string _name, 
        move_t _category, 
        elemental_t _type, 
        uint8_t _power, 
        uint8_t _uses, 
        uint8_t _accuracy
    );

    int getUID() const { return uid; }
    std::string getName() const { return std::string(name); }
    move_t getCategory() const { return category; }
    elemental_t getType() const { return type; }
    uint8_t getPower() const { return power; }
    uint8_t getUses() const { return uses; }
    void useMove() { if (uses > 0) uses--; }
    uint8_t getAccuracy() const { return accuracy; } 
};

class Creature {
private:
    int uid;
    static int nextUID;
    char name[MAXNAMESIZE], nickName[MAXNAMESIZE];
    elemental_t type;
    int16_t health;
    uint8_t attack, defense, speed;
    std::array<CreatureMove*, 4> moves;
    uint8_t usableMoves;

public:
    Creature();
    Creature(
        std::string _name, 
        std::string _nickName, 
        elemental_t _type, 
        int16_t _health,
        uint8_t _attack, 
        uint8_t _defense, 
        uint8_t _speed
    );
    
    int getUID() const { return uid; }
    std::string getName() const { return std::string(name); }
    std::string getNickName() const { return std::string(nickName); }
    void setNickName(std::string newNickName);
    elemental_t getType() const { return type; }
    int16_t getHealth() const { return health; }
    void setHealth(int16_t newHealth) { health = newHealth; }
    void decreaseHealth(int damage) { health -= damage; }
    uint8_t getAttack() const { return attack; }
    void setAttack(uint8_t newAttack) { attack = newAttack; }
    int8_t getDefense() const { return defense; }
    void setDefense(uint8_t newDefense) { defense = newDefense; }
    uint8_t getSpeed() const { return speed; }
    void setSpeed(uint8_t newSpeed) { speed = newSpeed; }

    std::array<CreatureMove*, 4>* getMoves() { return &moves; }
    CreatureMove* getMove(int index) { return moves[index]; }
    void setMoves(std::array<CreatureMove*, 4> &newMoves);
    std::string getMoveName(int index) { return moves[index]->getName(); }
    uint8_t getNumberOfMoves() const { return usableMoves; }
};

class MoveList {
private:
    std::unordered_map<int, CreatureMove*> moves;
    std::unordered_map<int, std::vector<CreatureMove *> > creatureMovePools;
public:
    MoveList() { moves.insert(std::make_pair(-1, new CreatureMove())); }
    void addMove(CreatureMove* moves);
    CreatureMove* getMove(int uid) { return moves.at(uid); }
    // Maybe don't need this?
    // std::vector<CreatureMove*> getMovePool(Creature* c);
    void initCreatureMovePool(int uid);
    void addCreatureMovePool(int uid, std::vector<CreatureMove *> &moves);
};

void dealDamage(Creature *c1, Creature *c2, CreatureMove *c1Move, CreatureMove *c2Move);

}

#endif
