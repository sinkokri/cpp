/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <string>

#include "Item.h"

class Food: public Item {
public:
    // increasing health by vitality number after item consumption
    int vitality;

    Food(std::string name, std::string description, int vitality);

    void printDescription( void ) override;

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;
};

