/**
* Created by Sinkovskaya Kristina on 5/7/2022.
* <sinkokri@fit.cvut.cz>
*/

#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "Item.h"

class Inventory {
public:
    // list of possessed items (weapons, armors, food, other items) and its count
    std::vector<std::pair<std::shared_ptr<Item>, int>> listOfItems;

    // amount of items in container
    int size;

    //
    bool hasItem ( std::string itemName ) const
    {
        for ( const auto & item: listOfItems )
        {
            if ( item . first -> name == itemName )
                return true;
        }
        return false;
    }

    // printing all items in container
    void printItems() const
    {
        if ( listOfItems . empty() )
            std::cout << "Nothing is here." << std::endl;

        std::cout << "Here you can see: " << std::endl;
        for ( const auto & item: listOfItems )
        {
            item .first -> printDescription();
            std::cout << " with the amount " << item . second << std::endl;
        }
    }

    // adding one more item (weapons, armors, food, other items)
    bool addItem( const Item & item )
    {
        for ( const auto & i = listOfItems . begin(); i != listOfItems . end(); )
        {
            if ( i -> first -> name == item . name )
            {
                i -> second += 1;
                return true;
            }
        }
        listOfItems . push_back(std::make_pair (std::make_shared<Item>(item), 1 ) );
        return true;
    }

    // removing one item
    bool removeItem( const Item & item )
    {
        for ( const auto & i = listOfItems . begin(); i != listOfItems . end(); )
        {
            if ( i -> first -> name == item . name )
            {
                listOfItems . erase( i );
                return true;
            }
        }
        return false;
    }

    Inventory( void ){}
};