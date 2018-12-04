//
//  InputTextValidation.hpp
//  FinalYearProject_Higher
//
//  Created by Constantin on 22/10/2018.
//  Copyright © 2018 Yijia.Zhao. All rights reserved.
//

#ifndef InputValidation_hpp
#define InputValidation_hpp

#include <iostream>
#include <sstream>
#include <regex>

class InputValidation
{
public:
    InputValidation();
    bool isRepeatPoint(std::vector<std::tuple<char,int,char>> *vct, int *index);
    bool basic_Validation(std::string codes);
    bool second_Validation(std::string codes,std::vector<std::tuple<char,int,bool>> *hor_vertice, std::vector<std::tuple<char,int,bool>> *ver_vertice, std::vector<std::tuple<char,int,bool>> *cross_vertice);
    void getCombCodes(std::vector<std::vector<std::tuple<char,int,bool>>> *combine_codes,std::string codes);
//    void getCombCodes_2(vector<vector<tuple<char,int,bool>>> *combine_codes,string codes);
    std::string checkSpace(std::string input);
};

#endif /* InputValidation_hpp */
