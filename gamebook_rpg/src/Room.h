/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <vector>
#include <string>

#include "Entity.h"
#include "Creature.h"
#include "Item.h"
#include "Interface.h"

class Room: public Entity {
public:
    std::string description;
    std::vector<Room> neighbours;
    std::vector<Creature> creatures;
    Inventory inventory;

    void printDescription( void ) override;

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;

};


