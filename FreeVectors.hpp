//
//  FreeVectors.hpp
//  FinalYearProject_Higher
//
//  Created by Constantin on 22/10/2018.
//  Copyright © 2018 Yijia.Zhao. All rights reserved.
//

#ifndef FreeVectors_hpp
#define FreeVectors_hpp

#include <vector>
#include <string>

class FreeVectors
{
public:
    FreeVectors();
    void freeVector(std::vector<std::tuple<char,int,char>> *vectors);
    void freeStrVector(std::vector<std::string> *str);
    void freeTDVector(std::vector<std::vector<std::tuple<char,int,bool>>> *vectors);
    void freeVector_Bool(std::vector<std::tuple<char,int,bool>> *vectors);
};

#endif /* FreeVectors_hpp */
