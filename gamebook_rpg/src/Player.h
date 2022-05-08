/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include<vector>

#include "Entity.h"
#include "Room.h"
#include "Food.h."

class Player: public Creature {
public:
    // current location of the player
    Room * currentRoom;

    // rooms the player already visited
    std::vector<Room> visitedRooms;

    bool lootRoom ( const Item & item )
    {
        if ( currentRoom -> inventory . hasItem( item . name ) )
        {
            currentRoom -> inventory . removeItem( item );
            this -> inventory . addItem ( item );
            return true;
        }
        return false;
    }

    bool lootCreature ( Creature & creature, const Item & item )
    {
        if ( creature . isAlive )
            return false;

        if ( creature . inventory . hasItem(item . name ) )
        {
            creature . inventory . removeItem( item );
            this -> inventory . addItem( item );
            return true;
        }
        return false;
    }

    // returns increased amount of health by the eaten food's vitality num
    int eatFood( Food & item );

    // changes the current room
    bool goToRoom( const Room & room );


    void printDescription( void ) override;

    void loadJson( const std::string & file ) override;

    void writeJson( const std::string & file ) override;

};
