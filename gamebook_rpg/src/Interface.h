/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <string>
#include <vector>

#include "Creature.h"

class Interface {
public:
    void mainMenu();
private:
    // menu interfaces to examine the examinable to choose the action
    void player();
    void inventory();
    void room();
    void creature();
    void fight( Creature & enemy );
};
