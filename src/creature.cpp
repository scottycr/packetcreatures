#include "creature.h"

using namespace std;

using namespace creature;

int Creature::nextUID = 0;

Creature::Creature() : 
    uid(-1),
    name("empty"), 
    nickName("empty"),
    type(noelemental), 
    attack(0), 
    defense(0), 
    speed(0) 
{}

Creature::Creature(
    string _name,
    string _nickName,
    elemental _type,
    uint8_t _attack,
    uint8_t _defense,
    uint8_t _speed
) : uid(nextUID),
    name(_name), 
    nickName(_nickName),
    type(_type), 
    attack(_attack), 
    defense(_defense), 
    speed(_speed) 
{ nextUID++; }

int CreatureMove::nextUID = 0;

CreatureMove::CreatureMove() :
    uid(-1),
    name("empty"), 
    category(nocategory), 
    type(noelemental), 
    power(0), 
    uses(0), 
    accuracy(0)
{}

CreatureMove::CreatureMove(
    string _name, 
    movecategory _category, 
    elemental _type, 
    uint8_t _power, 
    uint8_t _uses, 
    uint8_t _accuracy
) : uid(nextUID),
    name(_name), 
    category(_category), 
    type(_type), 
    power(_power), 
    uses(_uses), 
    accuracy(_accuracy) 
{ nextUID++; }

void MoveList::addMove(CreatureMove* newMove) {
    moves.insert(make_pair(newMove->getUID(), newMove));
}

// Maybe don't need this?
/* std::vector<CreatureMove*> MoveList::getMovePool(Creature*) {

} */

void MoveList::addCreatureMovePool(Creature* creature) {
    creatureMovePools.insert(make_pair(creature, vector<CreatureMove*>()));
}

void MoveList::addCreatureMovePool(Creature* creature, vector<CreatureMove*> &moves) {
    creatureMovePools.insert(make_pair(creature, moves));
}