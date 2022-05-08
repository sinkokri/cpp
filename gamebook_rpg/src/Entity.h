/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <string>

class Entity {
public:
    std::string name;

    Entity ( std::string name ): name ( name ){};

    virtual void printDescription( void ) = 0;

    virtual void loadJson( const std::string & file ) = 0;

    virtual void writeJson( const std::string & file ) = 0;
};


