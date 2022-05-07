#ifndef CREATURE_H_
#define CREATURE_H_

#include <string>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <vector>

namespace creature {

enum move_t { 
    M_NONE, 
    PHYSICAL, 
    STATUS 
};

std::string getMoveT(const move_t &mt);

enum elemental_t {
    E_NONE,
    NORMAL,
    GRASS,
    FIRE,
    WATER
};

std::string getElementalT(const elemental_t &et);

enum target_t {
    T_NONE,
    SELF,
    OPPONENT
};

std::string getTargetT(const target_t &tt);

enum stage_t {
    SS_NONE,
    ATTACK,
    DEFENSE,
    SPEED
};

std::string getStageT(const stage_t &st);

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
    target_t target;
    // Stat affected by status moves.
    // Set to SS_NONE if category is not STATUS. 
    stage_t statType;
    // The power of a move, set to 0 if category is STATUS.
    int16_t power;
    // Amount of stages the status move changes.
    // Set to 0 if category is not STATUS.
    int16_t stages;
    int16_t uses, accuracy;
public:
    CreatureMove();
    CreatureMove(
        std::string _name, 
        move_t _category, 
        elemental_t _type,
        target_t target,
        int16_t _power, 
        int16_t _uses, 
        int16_t _accuracy
    );
    CreatureMove(
        std::string _name, 
        move_t _category, 
        elemental_t _type,
        target_t target,
        stage_t _statType,
        int16_t _stages,
        int16_t _uses, 
        int16_t _accuracy
    );

    int getUID() const { return uid; }
    std::string getName() const { return std::string(name); }
    move_t getCategory() const { return category; }
    elemental_t getType() const { return type; }
    target_t getTarget() const { return target; }
    stage_t getStatType() const { return statType; }
    int16_t getPower() const { return power; }
    int16_t getUses() const { return uses; }
    void useMove() { if (uses > 0) uses--; }
    int16_t getAccuracy() const { return accuracy; }
    int16_t getStages() const { return stages; }

    void printInfo();
};

float getStageMultiplier(int16_t stage); 

class Creature {
private:
    int uid;
    static int nextUID;
    char name[MAXNAMESIZE], nickName[MAXNAMESIZE];
    elemental_t type;
    std::unordered_map<stage_t, int16_t> statStages;
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
    void decreaseHealth(int16_t damage) { health -= damage; }
    
    void modifyStage(stage_t stat, int16_t stage) { statStages.at(stat) += stage; }
    int16_t getStage(stage_t stat) const { return statStages.at(stat); }

    int16_t getBaseAttack() const { return attack; }
    float getAttack() const { return static_cast<float>(attack) * getStageMultiplier(getStage(ATTACK)); }
    void setAttack(int16_t newAttack) { attack = newAttack; }
    int16_t getBaseDefense() const { return defense; }
    float getDefense() const { return static_cast<float>(defense) * getStageMultiplier(getStage(DEFENSE)); }
    void setDefense(int16_t newDefense) { defense = newDefense; }
    int16_t getBaseSpeed() const { return speed; }
    float getSpeed() const { return static_cast<float>(speed) * getStageMultiplier(getStage(SPEED)); }
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

void performMove(Creature &c1, Creature &c2, CreatureMove *move);
void performTurn(Creature &c1, Creature &c2, CreatureMove *c1Move, CreatureMove *c2Move);

}

#endif
