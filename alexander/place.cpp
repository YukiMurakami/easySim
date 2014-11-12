//
//  place.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#include "place.h"

Place::Place(string name,vector<string> next,vector<Person> person) {
    _name = name;
    _nextPlaces = next;
    _persons = person;
}

Place::Place() {
    _nextPlaces.clear();
    _name = "";
    _persons.clear();
}