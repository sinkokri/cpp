/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <string>

#include "Item.h"

class Weapon: public Item {
public:
    // increasing attack rate by damage number (accumulated with strength)
    int damage;

    Weapon(std::string name, std::string description, int damage);

    void printDescription( void ) override;

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;
};

