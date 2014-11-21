//
//  place.cpp
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#include "place.h"
#include <map>
#include <vector>

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


vector<string> makePlaceVectorFromMap(map<string,Place> &place) {
    vector<string> places;
    for(map<string,Place>::iterator it = place.begin();it != place.end();it++) {
        string placeName = (*it).second._name;
        places.push_back(placeName);
    }
    
    return places;
}