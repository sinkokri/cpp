/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <string>

#include "Item.h"

class Armor: public Item {
public:
    // decreasing received attack rate by defense number
    int defense;

    Armor(std::string name, std::string description, int defense);

    void printDescription( void ) override;

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;
};


