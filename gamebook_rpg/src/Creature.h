/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include "Entity.h"
#include "Inventory.h"

class Creature: public Entity {
public:
    // health can be increased by consuming some food if any; decreased in battles
    int health;

    // strength is default damage rate
    int strength;

    // zero health = feeling not well
    bool isAlive;

    // list of all possessed items (weapons, armors, food, other items)
    Inventory inventory;

    //attacking a target creature; returns target's reduced by this attack health
    int attack( Creature & target );

    //defending from a source creature; returns reduced own health
    int defend ( Creature & source );

    Creature( std::string & name, int health, int strength, bool isAlive, Inventory inventory);

    void printDescription() override;

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;
};
