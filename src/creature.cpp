#include <string>
#include <stdint.h>
#include <array>
#include <map>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <iostream>

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
            return "EMPTY";
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
            return "NONE";
    }
}

CreatureMove::CreatureMove() :
    uid(-1),
    name("empty"), 
    category(EMPTY), 
    type(NONE), 
    power(0), 
    uses(0), 
    accuracy(0)
{
    name[MAXNAMESIZE-1] = '\0';
}

CreatureMove::CreatureMove(
    string _name, 
    move_t _category, 
    elemental_t _type, 
    int16_t _power, 
    int16_t _uses, 
    int16_t _accuracy
) : uid(nextUID), 
    category(_category), 
    type(_type), 
    power(_power), 
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
    type(NONE),
    health(0), 
    attack(0), 
    defense(0), 
    speed(0) 
{
    name[MAXNAMESIZE-1] = '\0';
    nickName[MAXNAMESIZE-1] = '\0';
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
    for (unsigned i=0; i<4; i++) moves[i] = masterList.getMove(-1);
    usableMoves = 0;
}

void Creature::addMove(CreatureMove newMove) {
    if (usableMoves < 4) {
        moves[usableMoves] = newMove;
        usableMoves++;
    } else {
        cout << "Not enough space! Use setMoves() instead." << endl;
    } 
}

void Creature::printInfo() {
    cout << uid << ' ' << name << endl;
    cout << getElementalT(type) << endl;
    cout << health << ' ' << attack << ' ' << defense << ' ' << speed << endl;
    cout << usableMoves << endl;
    cout << endl;
    for (auto &m : moves) {
        m.printInfo();
        cout << endl;
    }
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

// Deal with the damage being broken...
void creature::dealDamage(Creature *c1, Creature *c2, CreatureMove *c1Move, CreatureMove *c2Move) {
    srand(time(NULL));
    double damage = 0;

    // THIS IS TEMPORARY!!!
    // PLEASE ADD LEVELS TO CREATURES FUTURE ME!!!
    int level = 5;
    // End of warning.

    double power = 0;
    double attack = 0;
    double defense = 0;
    double random = 0;
    
    if (c1->getSpeed() > c2->getSpeed()) {
        power = c1Move->getPower();
        attack = c1->getAttack();
        defense = c2->getDefense();
        // Somewhere within [.85-1.00].
        random = (rand() % 16 + 85.0) / 100.0;

        damage = ((2 * level) / 5) + 2;
        damage = (damage * power * (attack/defense)) / 50;
        damage = (damage + 2) * random;
        c2->decreaseHealth(static_cast<int>(damage));
        c1Move->useMove();

        if (c2->getHealth() > 0) {
            power = c2Move->getPower();
            attack = c2->getAttack();
            defense = c1->getDefense();
            // Somewhere within [.85-1.00].
            random = (rand() % 16 + 85.0) / 100.0;

            damage = ((2 * level) / 5) + 2;
            damage = (damage * power * (attack/defense)) / 50;
            damage = (damage + 2) * random;
            c1->decreaseHealth(static_cast<int>(damage));
            c2Move->useMove();
        }
    } else {
        power = c2Move->getPower();
        attack = c2->getAttack();
        defense = c1->getDefense();
        // Somewhere within [.85-1.00].
        random = (rand() % 16 + 85) / 100;

        damage = ((2 * level) / 5) + 2;
        damage = (damage * power * (attack/defense)) / 50;
        damage = (damage + 2) * random;
        c1->decreaseHealth(static_cast<int>(damage));
        c2Move->useMove();

        if (c1->getHealth() > 0) {
            power = c1Move->getPower();
            attack = c1->getAttack();
            defense = c2->getDefense();
            // Somewhere within [.85-1.00].
            random = (rand() % 16 + 85) / 100;

            damage = ((2 * level) / 5) + 2;
            damage = (damage * power * (attack/defense)) / 50;
            damage = (damage + 2) * random;
            c2->decreaseHealth(static_cast<int>(damage));
            c1Move->useMove();
        }
    }
}