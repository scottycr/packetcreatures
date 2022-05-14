#include <string>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "creature.h"

using namespace std;

using namespace creature;

int CreatureMove::nextUID = 0;

string creature::getMoveT(const move_t &mt) {
    switch (mt) {
        case PHYSICAL:
            return "PHYSICAL";
        case STATUS:
            return "STATUS";
        default:
            return "M_NONE";
    }
}

string creature::getElementalT(const elemental_t &et) {
    switch (et) {
        case NORMAL:
            return "NORMAL";
        case GRASS:
            return "GRASS";
        case FIRE:
            return "FIRE";
        case WATER:
            return "WATER";
        default:
            return "E_NONE";
    }
}

string creature::getTargetT(const target_t &tt) {
    switch (tt) {
        case SELF:
            return "SELF";
        case OPPONENT:
            return "OPPONENT";
        default:
            return "T_NONE";
    }
}

string creature::getStageT(const stage_t &st) {
    switch (st) {
        case ATTACK:
            return "ATTACK";
        case DEFENSE:
            return "DEFENSE";
        case SPEED:
            return "SPEED";
        default:
            return "SS_NONE";
    }
}

CreatureMove::CreatureMove() :
    uid(-1),
    name("empty"), 
    category(M_NONE), 
    type(E_NONE), 
    target(T_NONE),
    statType(SS_NONE),
    power(0), 
    stages(0),
    uses(0), 
    accuracy(0)
{
    name[MAXNAMESIZE-1] = '\0';
}

CreatureMove::CreatureMove(
    string _name, 
    move_t _category, 
    elemental_t _type, 
    target_t _target,
    int16_t _power, 
    int16_t _uses, 
    int16_t _accuracy
) : uid(nextUID),
    category(_category), 
    type(_type),
    target(_target),
    statType(SS_NONE),
    power(_power),
    stages(0),
    uses(_uses),
    accuracy(_accuracy)
{ 
    nextUID++;
    strncpy(name, _name.c_str(), MAXNAMESIZE);
    name[MAXNAMESIZE-1] = '\0';
}

float creature::getStageMultiplier(int16_t stage) {
    switch (stage) {
        case -6:
            return 2.0/8.0;
        case -5:
            return 2.0/7.0;
        case -4:
            return 2.0/6.0;
        case -3:
            return 2.0/5.0;
        case -2:
            return 2.0/4.0;
        case -1:
            return 2.0/3.0;
        case 1:
            return 3.0/2.0;
        case 2:
            return 4.0/2.0;
        case 3:
            return 5.0/2.0;
        case 4:
            return 6.0/2.0;
        case 5:
            return 7.0/2.0;
        case 6:
            return 8.0/2.0;
        default:
            return 1.0;
    }
}

CreatureMove::CreatureMove(
    string _name, 
    move_t _category, 
    elemental_t _type,
    target_t _target, 
    stage_t _statType,
    int16_t _stages,
    int16_t _uses, 
    int16_t _accuracy
) : uid(nextUID),
    category(_category), 
    type(_type),
    target(_target),
    statType(_statType),
    power(0),
    stages(_stages),
    uses(_uses),
    accuracy(_accuracy)
{ 
    nextUID++;
    strncpy(name, _name.c_str(), MAXNAMESIZE);
    name[MAXNAMESIZE-1] = '\0';
}

void CreatureMove::printInfo() {
    cout << uid << ' ' << name << endl;
    cout << getMoveT(category) << ' ' << getElementalT(type) << endl;
    cout << power << ' '<< uses << ' ' << accuracy << endl;
}

int Creature::nextUID = 0;
MoveList creature::masterList = MoveList();

Creature::Creature() : 
    uid(-1),
    name("empty"), 
    nickName("empty"),
    type(E_NONE),
    health(0), 
    attack(0), 
    defense(0), 
    speed(0) 
{
    name[MAXNAMESIZE-1] = '\0';
    nickName[MAXNAMESIZE-1] = '\0';

    statStages.insert(make_pair(ATTACK, 0));
    statStages.insert(make_pair(DEFENSE, 0));
    statStages.insert(make_pair(SPEED, 0));
    
    for (unsigned i=0; i<4; i++) moves[i] = masterList.getMove(-1);
    usableMoves = 0;
}

Creature::Creature(
    string _name,
    string _nickName,
    elemental_t _type,
    int16_t _health,
    int16_t _attack,
    int16_t _defense,
    int16_t _speed
) : uid(nextUID),
    type(_type),
    health(_health),
    attack(_attack), 
    defense(_defense), 
    speed(_speed) 
{
    nextUID++;
    strncpy(name, _name.c_str(), MAXNAMESIZE);
    name[MAXNAMESIZE-1] = '\0';
    strncpy(nickName, _nickName.c_str(), MAXNAMESIZE);
    nickName[MAXNAMESIZE-1] = '\0';

    statStages.insert(make_pair(ATTACK, 0));
    statStages.insert(make_pair(DEFENSE, 0));
    statStages.insert(make_pair(SPEED, 0));

    for (unsigned i=0; i<4; i++) moves[i] = masterList.getMove(-1);
    usableMoves = 0;
}

void Creature::addMove(CreatureMove newMove) {
    if (usableMoves < 4) {
        moves[usableMoves] = newMove;
        usableMoves++;
    } else {
        cerr << "Not enough space! Use setMoves() instead." << endl;
    } 
}

void Creature::printInfo() {
    cout << uid << ' ' << name << endl;
    cout << getElementalT(type) << endl;
    cout << health << ' ' << attack << ' ' << defense << ' ' << speed << endl;
    cout << usableMoves << endl;
    for (auto &s : statStages) cout << getStageT(s.first) << ' ' << s.second << endl;
    cout << endl;
    for (auto &m : moves) {
        m.printInfo();
        cout << endl;
    }
}

void Creature::printMoveScreen() {
    cout << name << ": " << health << endl; 
    cout << "Moves:" << endl;
    for (unsigned i=0; i<4; i++) {
        if (moves[i].getUID() != -1) {
            cout << "\t" << i+1 << ": " << moves[i].getName() << "\tUses: " << moves[i].getUses() << endl;
        }
    }
    cout << endl;
}

void MoveList::addMove(CreatureMove newMove) {
    moves.insert(make_pair(newMove.getUID(), newMove));
}

// Maybe don't need this?
/* std::vector<CreatureMove*> MoveList::getMovePool(Creature*) {

} */

void MoveList::initCreatureMovePool(int uid) {
    creatureMovePools.insert(make_pair(uid, vector<CreatureMove>()));
}

void MoveList::addCreatureMovePool(int uid, vector<CreatureMove> moves) {
    creatureMovePools.insert(make_pair(uid, moves));
}

void creature::performMove(Creature &c1, Creature &c2, CreatureMove *move) {
    float damage = 0;
    // THIS IS TEMPORARY!!!
    // PLEASE ADD LEVELS TO CREATURES FUTURE ME!!!
    float level = 5;
    // End of warning.

    float power = 0;
    float attack = 0;
    float defense = 0;
    float random = 0;
    int critical = 0;
    
    if (move->getCategory() != STATUS) {
        power = move->getPower();
        attack = c1.getAttack();
        defense = c2.getDefense();
        // Somewhere within [.85-1.00].
        srand(time(NULL));
        random = (rand() % 16 + 85.0) / 100.0;
        critical = rand() % 16;

        damage = ((2.0 * level) / 5.0) + 2.0;
        damage = (damage * power * (attack/defense)) / 50.0;
        damage = (damage + 2.0) * random;
        if (critical == 0) damage *= 2.0;
        move->useMove();
        c2.decreaseHealth(static_cast<int16_t>(damage));
    } else {
        stage_t statStage = move->getStatType(); 
        int16_t stages = move->getStages();

        // Change it to add stages, not set.
        if (move->getTarget() == SELF) {
            c1.modifyStage(statStage, stages);
        } else if (move->getTarget() == OPPONENT) {
            c2.modifyStage(statStage, stages);
        }
    }
}

// Deal with the damage being broken...
void creature::performTurn(Creature &c1, Creature &c2, CreatureMove *c1Move, CreatureMove *c2Move) {
    if (c1.getSpeed() > c2.getSpeed()) {
        performMove(c1, c2, c1Move);
        if (c2.getHealth() > 0) {
            performMove(c2, c1, c2Move);
        }
    } else {
        performMove(c2, c1, c2Move);
        if (c1.getHealth() > 0) {
            performMove(c1, c2, c1Move);
        }
    }
}