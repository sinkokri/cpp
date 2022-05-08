/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <string>
#include <iostream>

#include "Entity.h"

class Item: public Entity {
public:
    std::string description;

    Item( void );

    void printDescription( void ) override
    {
        std::cout << this->name << ": " << this->description << std::endl;
    }

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;
};

