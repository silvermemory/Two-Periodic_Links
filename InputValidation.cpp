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

bool InputValidation::isRepeatPoint(vector<tuple<char,int,char>> *vct, int *index) {
    bool repeat = false;
    
    for (vector<tuple<char,int,char>>::iterator it = vct->begin(); it != vct->end(); ++it) {
        if (*index == get<1>(*it))
            repeat = true;
    }
    
    return repeat;
}

bool InputValidation::basic_Validation(string codes) {
    string basic_pattern = "(([HhVvOoUu][[:digit:]]+(\\+|-))*(\\,)*)+";
    regex reg(basic_pattern);
    
    if (regex_match(codes,reg)) {
        cout << "The basic format of codes is valid." << endl;
        return true;
    } else {
        cout << "The basic format of codes is not valid." << endl;
        return false;
    }
}

bool InputValidation::second_Validation(string codes) {
    bool valid = true;
    bool repeat = false;
    vector<tuple<char,int,char>> *vertical_point = new vector<tuple<char,int,char>>();
    vector<tuple<char,int,char>> *horizontal_point = new vector<tuple<char,int,char>>();
    vector<tuple<char,int,char>> *over_crossing_point = new vector<tuple<char,int,char>>();
    vector<tuple<char,int,char>> *under_crossing_point = new vector<tuple<char,int,char>>();
    int ocp_index_max = 0, ucp_index_max = 0, vp_index_max = 0, hp_index_max = 0;
    
    string third_pattern = "[HhVvOoUu][[:digit:]]+(\\+|-)";
    regex reg(third_pattern);
    
    sregex_iterator currentMatch(codes.begin(), codes.end(), reg);
    sregex_iterator lastMatch;
    
    while (currentMatch != lastMatch) {
        char type;
        int index;
        char sign;
        
        smatch match = *currentMatch;
        stringstream ss(match.str());
        ss >> type >> index >> sign;
        
        if (type == 'o' || type == 'O') {
            if (!(repeat= isRepeatPoint(over_crossing_point, &index))) {
                if (ocp_index_max < index)
                    ocp_index_max = index;
                over_crossing_point->push_back(make_tuple(type,index,sign));
            }
        } else if (type == 'u' || type == 'U') {
            if (!(repeat = isRepeatPoint(under_crossing_point, &index))) {
                if (ucp_index_max < index)
                    ucp_index_max = index;
                under_crossing_point->push_back(make_tuple(type,index,sign));
            }
        } else if (type == 'v' || type == 'V') {
            if (!(repeat = isRepeatPoint(vertical_point, &index))) {
                if (vp_index_max < index)
                    vp_index_max = index;
                vertical_point->push_back(make_tuple(type, index,sign));
            }
        } else if (type == 'h' || type == 'H') {
            if (!(repeat = isRepeatPoint(horizontal_point, &index))) {
                if (hp_index_max < index)
                    hp_index_max = index;
                horizontal_point->push_back(make_tuple(type, index,sign));
            }
        }
        
        if (repeat) {
            valid = false;
            cout << "Invalid. There point " << type << index << sign << " input more than once." << endl;
            break;
        }
        
        currentMatch++;
    }

    if (ocp_index_max != over_crossing_point->size()) {
        valid = false;
        cout << "Invalid. The maximun index of over-crossing point is " << ocp_index_max <<"." << endl;
        cout << "But the size of collection of over-crossing points is " << over_crossing_point->size() << "." << endl;
        cout << "Some points are missed." << endl;
    }

    if (ucp_index_max != under_crossing_point->size()) {
        valid = false;
        cout << "Invalid. The maximun index of under-crossing point is " << ucp_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << under_crossing_point->size() << "." << endl;
        cout << "Some points are missed." << endl;
    }

    if (vp_index_max != vertical_point->size()) {
        valid = false;
        cout << "Invalid. The maximun index of under-crossing point is " << vp_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << vertical_point->size() << "." << endl;
        cout << "Some are missed." << endl;
    }

    if (hp_index_max != horizontal_point->size()) {
        valid = false;
        cout << "Invliad. The maximun index of under-crossing point is " << hp_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << horizontal_point->size() << "." << endl;
        cout << "Some points are missed." << endl;
    }
    
    if (valid == true && over_crossing_point->size() == under_crossing_point->size()) {
        if (over_crossing_point->size() > 0) {
            for (vector<tuple<char,int,char>>::iterator it = over_crossing_point->begin(); it != over_crossing_point->end(); ++it) {
                int opst_pos = 0;
                for (vector<tuple<char,int,char>>::iterator it_opst = under_crossing_point->begin(); it_opst != under_crossing_point->end(); ++it_opst) {
                    if (get<1>(*it) == get<1>(*it_opst)) {
                        if (get<2>(*it) != get<2>(*it_opst)) {
                            valid = false;
                            cout << "There are some mistake about signs for crossing point:" << endl;
                            cout << get<0>(*it) << get<1>(*it) << get<2>(*it) << " and " << get<0>(*it_opst) << get<1>(*it_opst) << get<2>(*it_opst);
                            cout << " should have the same sign of orientation, both + or both -. Invalid." << endl;
                        } else if (get<2>(*it) == get<2>(*it_opst)) {
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
        cout << "Invalid. Not all crossing points have its own opposite points." << endl;
    }
    
    
    FV.freeVector(over_crossing_point);
    FV.freeVector(under_crossing_point);
    FV.freeVector(vertical_point);
    FV.freeVector(horizontal_point);
    
    if (valid == true)
        cout << "Second valid" << endl;
    
    return valid;
}

void InputValidation::getCombCodes(vector<vector<tuple<char,int,bool>>> *combine_codes,string codes) {
    size_t find;
    bool endWithComma = false;
    vector<string> *str = new vector<string>();
    vector<tuple<char,int,bool>> *paragraph = new vector<tuple<char,int,bool>>;
    
    if (codes.length() == 0) {
        combine_codes->push_back(vector<tuple<char,int,bool>>());
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
        
        for (vector<string>::iterator it_str = str->begin(); it_str != str->end(); ++it_str) {
            if (it_str->length() == 1) {
                combine_codes->push_back(vector<tuple<char,int,bool>>());
            } else {
                string pattern = "[HhVvOoUu][[:digit:]]+(\\+|-))";
                regex reg(pattern);
                
                sregex_iterator currentMatch(it_str->begin(), it_str->end(), reg);
                sregex_iterator lastMatch;
                while (currentMatch != lastMatch) {
                    char type;
                    int index;
                    char char_sign;
                    bool sign;
                    smatch match = *currentMatch;
                    stringstream ss(match.str());
                    ss >> type >> index >> char_sign;
                    
                    if (char_sign == '+')
                        sign = true;
                    else
                        sign = false;
                    
                    paragraph->push_back(make_tuple(type,index,sign));
                    currentMatch++;
                }
                combine_codes->push_back(*paragraph);
                paragraph->clear();
            }
        }
    }
    
    if (endWithComma)
        combine_codes->push_back(vector<tuple<char,int,bool>>());
    
    FV.freeVector_Bool(paragraph);
    FV.freeStrVector(str);
}

string InputValidation::checkSpace(string codes) {
    codes.erase(remove_if(codes.begin(), codes.end(), [](unsigned char x){ return isspace(x);}),codes.end());
    return codes;
}
