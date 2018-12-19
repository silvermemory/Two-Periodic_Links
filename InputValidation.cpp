//
//  InputTextValidation.cpp
//  FinalYearProject_Higher
//
//  Created by Constantin on 22/10/2018.
//  Copyright © 2018 Yijia.Zhao. All rights reserved.
//

#include "InputValidation.hpp"
#include "FreeVectors.hpp"

FreeVectors FV;

InputValidation::InputValidation()
{
    
}

bool InputValidation::isRepeatPoint(std::vector<std::tuple<char,int,char>> *vct, int *index) {
    bool repeat = false;
    
    for (std::vector<std::tuple<char,int,char>>::iterator it = vct->begin(); it != vct->end(); ++it) {
        if (*index == std::get<1>(*it))
            repeat = true;
    }
    
    return repeat;
}

bool InputValidation::basic_Validation(std::string codes) {
    std::string basic_pattern = "(([hvou][[:digit:]]+(\\+|-))*(\\,)*)+";
    std::regex reg(basic_pattern);
    
    if (regex_match(codes,reg)) {
        std::cout << "The basic format of codes is valid." << std::endl;
        return true;
    } else {
        std::cout << "The basic format of codes is not valid." << std::endl;
        return false;
    }
}

bool InputValidation::second_Validation(std::string codes,std::vector<std::tuple<char,int,bool>> *hor_vertice, std::vector<std::tuple<char,int,bool>> *ver_vertice, std::vector<std::tuple<char,int,bool>> *cross_vertice) {
    bool valid = true;
    bool repeat = false;
    std::vector<std::tuple<char,int,char>> *vertical_point = new std::vector<std::tuple<char,int,char>>();
    std::vector<std::tuple<char,int,char>> *horizontal_point = new std::vector<std::tuple<char,int,char>>();
    std::vector<std::tuple<char,int,char>> *over_crossing_point = new std::vector<std::tuple<char,int,char>>();
    std::vector<std::tuple<char,int,char>> *under_crossing_point = new std::vector<std::tuple<char,int,char>>();
    int ocp_index_max = 0, ucp_index_max = 0, vp_index_max = 0, hp_index_max = 0;
    
    std::string third_pattern = "[hvou][[:digit:]]+(\\+|-)";
    std::regex reg(third_pattern);
    
    std::sregex_iterator currentMatch(codes.begin(), codes.end(), reg);
    std::sregex_iterator lastMatch;
    
    while (currentMatch != lastMatch) {
        char type;
        int index;
        char sign;
        
        std::smatch match = *currentMatch;
        std::stringstream ss(match.str());
        ss >> type >> index >> sign;
        
        if (type == 'o') {
            if (!(repeat= isRepeatPoint(over_crossing_point, &index))) {
                if (ocp_index_max < index)
                    ocp_index_max = index;
                over_crossing_point->push_back(std::make_tuple(type,index,sign));
            }
        } else if (type == 'u') {
            if (!(repeat = isRepeatPoint(under_crossing_point, &index))) {
                if (ucp_index_max < index)
                    ucp_index_max = index;
                under_crossing_point->push_back(std::make_tuple(type,index,sign));
            }
        } else if (type == 'v') {
            if (!(repeat = isRepeatPoint(vertical_point, &index))) {
                if (vp_index_max < index)
                    vp_index_max = index;
                vertical_point->push_back(std::make_tuple(type, index,sign));
            }
        } else if (type == 'h') {
            if (!(repeat = isRepeatPoint(horizontal_point, &index))) {
                if (hp_index_max < index)
                    hp_index_max = index;
                horizontal_point->push_back(std::make_tuple(type, index,sign));
            }
        }
        
        if (repeat) {
            valid = false;
            std::cout << "Invalid. There point " << type << index << sign << " input more than once." << std::endl;
            break;
        }
        
        currentMatch++;
    }

    if (ocp_index_max != over_crossing_point->size()) {
        valid = false;
        std::cout << "Invalid. The maximun index of over-crossing point is " << ocp_index_max <<"." << std::endl;
        std::cout << "But the size of collection of over-crossing points is " << over_crossing_point->size() << "." << std::endl;
        std::cout << "Some points are missed." << std::endl;
    }

    if (ucp_index_max != under_crossing_point->size()) {
        valid = false;
        std::cout << "Invalid. The maximun index of under-crossing point is " << ucp_index_max <<"." << std::endl;
        std::cout << "But the size of collection of under-crossing points is " << under_crossing_point->size() << "." << std::endl;
        std::cout << "Some points are missed." << std::endl;
    }

    if (vp_index_max != vertical_point->size()) {
        valid = false;
        std::cout << "Invalid. The maximun index of under-crossing point is " << vp_index_max <<"." << std::endl;
        std::cout << "But the size of collection of under-crossing points is " << vertical_point->size() << "." << std::endl;
        std::cout << "Some are missed." << std::endl;
    }

    if (hp_index_max != horizontal_point->size()) {
        valid = false;
        std::cout << "Invliad. The maximun index of under-crossing point is " << hp_index_max <<"." << std::endl;
        std::cout << "But the size of collection of under-crossing points is " << horizontal_point->size() << "." << std::endl;
        std::cout << "Some points are missed." << std::endl;
    }
    
    if (valid == true && over_crossing_point->size() == under_crossing_point->size()) {
        if (over_crossing_point->size() > 0) {
            for (std::vector<std::tuple<char,int,char>>::iterator it = over_crossing_point->begin(); it != over_crossing_point->end(); ++it) {
                int opst_pos = 0;
                for (std::vector<std::tuple<char,int,char>>::iterator it_opst = under_crossing_point->begin(); it_opst != under_crossing_point->end(); ++it_opst) {
                    if (std::get<1>(*it) == std::get<1>(*it_opst)) {
                        if (std::get<2>(*it) != std::get<2>(*it_opst)) {
                            valid = false;
                            std::cout << "There are some mistake about signs for crossing point:" << std::endl;
                            std::cout << std::get<0>(*it) << std::get<1>(*it) << std::get<2>(*it) << " and " << std::get<0>(*it_opst) << std::get<1>(*it_opst) << std::get<2>(*it_opst);
                            std::cout << " should have the same sign of orientation, both + or both -. Invalid." << std::endl;
                        } else if (std::get<2>(*it) == std::get<2>(*it_opst)) {
                            under_crossing_point->erase(under_crossing_point->begin()+opst_pos);
                            break;
                        }
                    }
                    opst_pos++;
                }
            }
        }
    } else if (valid == true && over_crossing_point->size() != under_crossing_point->size()) {
        valid = false;
        std::cout << "Invalid. Not all crossing points have its own opposite points." << std::endl;
    }
    
    if (valid == true) {
        std::cout << "Second valid" << std::endl;
        
        for (std::vector<std::tuple<char,int,char>>::iterator it = horizontal_point->begin(); it != horizontal_point->end(); ++it) {
            if (std::get<2>(*it) == '+')
                hor_vertice->push_back(std::make_tuple(std::get<0>(*it),std::get<1>(*it),true));
            else
                hor_vertice->push_back(std::make_tuple(std::get<0>(*it),std::get<1>(*it),false));
        }
        
        for (std::vector<std::tuple<char,int,char>>::iterator it = vertical_point->begin(); it != vertical_point->end(); ++it) {
            if (std::get<2>(*it) == '+')
                ver_vertice->push_back(std::make_tuple(std::get<0>(*it),std::get<1>(*it),true));
            else
                ver_vertice->push_back(std::make_tuple(std::get<0>(*it),std::get<1>(*it),false));
        }
        
        for (std::vector<std::tuple<char,int,char>>::iterator it = over_crossing_point->begin(); it != over_crossing_point->end(); ++it) {
            if (std::get<2>(*it) == '+')
                cross_vertice->push_back(std::make_tuple('c',std::get<1>(*it),true));
            else
                cross_vertice->push_back(std::make_tuple('c',std::get<1>(*it),false));
        }
        
    }
    
    FV.freeVector(over_crossing_point);
    FV.freeVector(under_crossing_point);
    FV.freeVector(vertical_point);
    FV.freeVector(horizontal_point);
    
    return valid;
}

void InputValidation::getCombCodes(std::vector<std::vector<std::tuple<char,int,bool>>> *combine_codes,std::string codes) {
    size_t find;
    bool endWithComma = false;
    std::vector<std::string> *str = new std::vector<std::string>();
    std::vector<std::tuple<char,int,bool>> *paragraph = new std::vector<std::tuple<char,int,bool>>;
    
    if (codes.length() == 0) {
        combine_codes->push_back(std::vector<std::tuple<char,int,bool>>());
    } else {
        if (codes.back() == ',')
            endWithComma = true;
        else
            codes.append(",");
        
        while (codes.empty() == 0) {
            find = codes.find(",");
            str->push_back(codes.substr(0,find+1));
            codes.erase(0,find+1);
        }
        
        for (std::vector<std::string>::iterator it_str = str->begin(); it_str != str->end(); ++it_str) {
            if (it_str->length() == 1) {
                combine_codes->push_back(std::vector<std::tuple<char,int,bool>>());
            } else {
                std::string pattern = "[hvou][[:digit:]]+(\\+|-)";
                std::regex reg(pattern);
                
                std::sregex_iterator currentMatch(it_str->begin(), it_str->end(), reg);
                std::sregex_iterator lastMatch;
                while (currentMatch != lastMatch) {
                    char type;
                    int index;
                    char char_sign;
                    bool sign;
                    std::smatch match = *currentMatch;
                    std::stringstream ss(match.str());
                    ss >> type >> index >> char_sign;
                    
                    if (char_sign == '+')
                        sign = true;
                    else
                        sign = false;
                    
                    paragraph->push_back(std::make_tuple(type,index,sign));
                    currentMatch++;
                }
                combine_codes->push_back(*paragraph);
                paragraph->clear();
            }
        }
    }
    
    if (endWithComma)
        combine_codes->push_back(std::vector<std::tuple<char,int,bool>>());
    
    FV.freeVector_Bool(paragraph);
    FV.freeStrVector(str);
}

std::string InputValidation::checkSpace(std::string codes) {
    codes.erase(remove_if(codes.begin(), codes.end(), [](unsigned char x){ return isspace(x);}),codes.end());
    return codes;
}
