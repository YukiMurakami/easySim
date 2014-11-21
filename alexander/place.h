//
//  place.h
//  easySimu
//
//  Created by Yuki Murakami on 2014/06/17.
//
//

#ifndef __easySimu__place__
#define __easySimu__place__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "person.h"

using namespace std;

class Place {
public:
    string _name;
    vector<string> _nextPlaces;
    vector<Person> _persons;
    
    Place(string name,vector<string> next,vector<Person> persons);
    Place();
};

vector<string> makePlaceVectorFromMap(map<string,Place> &place);

#endif /* defined(__easySimu__place__) */
