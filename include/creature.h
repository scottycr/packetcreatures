#ifndef CREATURE_H_
#define CREATURE_H_

#include <string>
#include <stdint.h>
#include <array>
#include <map>
#include <vector>
#include <unordered_map>

namespace creature {

enum movecategory { nocategory, physical, status };
enum elemental {
    noelemental,
    normal,
    grass,
    fire,
    water
};

class CreatureMove;

class Creature {
private:
    int uid;
    static int nextUID;
    std::string name, nickName;
    elemental type;
    uint8_t attack, defense, speed;
    std::array<CreatureMove*, 4> moves;

public:
    Creature();
    Creature(
        std::string _name, 
        std::string _nickName, 
        elemental _type, 
        uint8_t _attack, 
        uint8_t _defense, 
        uint8_t _speed
    );
    
    int getUID() const { return uid; }
    std::string getName() const { return name; }
    std::string getNickName() const { return nickName; }
    void setNickName(std::string newNickName) { nickName = newNickName; }
    elemental getType() const { return type; }
    uint8_t getAttack() const { return attack; }
    void setAttack(uint8_t newAttack) { attack = newAttack; }
    uint8_t getDefense() const { return defense; }
    void setDefense(uint8_t newDefense) { defense = newDefense; }
    uint8_t getSpeed() const { return speed; }
    void setSpeed(uint8_t newSpeed) { speed = newSpeed; }
};

class CreatureMove {
private:
    int uid;
    static int nextUID;
    std::string name;
    movecategory category;
    elemental type;
    uint8_t power, uses, accuracy;
public:
    CreatureMove();
    CreatureMove(
        std::string _name, 
        movecategory _category, 
        elemental _type, 
        uint8_t _power, 
        uint8_t _uses, 
        uint8_t _accuracy
    );

    int getUID() const { return uid; }
    std::string getName() const { return name; }
    movecategory getCategory() const { return category; }
    elemental getType() const { return type; }
    uint8_t getPower() const { return power; }
    uint8_t getUses() const { return uses; }
    void useMove() { if (uses > 0) uses--; }
    uint8_t getAccuracy() const { return accuracy; } 
};

class MoveList {
private:
    std::unordered_map<int, CreatureMove*> moves;
    std::unordered_map<Creature*, std::vector<CreatureMove *> > creatureMovePools;
public:
    MoveList() {};
    void addMove(CreatureMove* m);
    // Maybe don't need this?
    // std::vector<CreatureMove*> getMovePool(Creature* c);
    void addCreatureMovePool(Creature* c);
    void addCreatureMovePool(Creature* c, std::vector<CreatureMove *> &m);
};

}

#endif
