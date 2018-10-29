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
    if (!codes.empty()) {
        //string basic_pattern = "(([[:space:]]*[DdHhVvOoUu][[:digit:]]+(\\+|-)[[:space:]]*)+(\\,)?)+";
        string basic_pattern = "(([[:space:]]*[HhVvOoUu][[:digit:]]+(\\+|-)[[:space:]]*)+(\\,)?)+";
        regex reg(basic_pattern);
        
        if (regex_match(codes,reg)) {
            //cannot end with comma will be discard when disjoint links are added
            if (codes.back() == ',') {
                cout << "The codes cannot end with comma(,). Please check your code." << endl;
                return false;
            } else {
                cout << "The basic format of codes is valid." << endl;
                return true;
            }
        } else {
            cout << "The basic format of codes is not valid. Please check your code." << endl;
            return false;
        }
    } else
        return false;
}

/*
bool InputValidation::second_Validation(string codes) {
    bool valid = true;
    codes.append(",");
    size_t find_1,find_2;
    vector<string> str;
    
    while (codes != "\0") {
        find_1 = codes.find(",");
        str.push_back(codes.substr(0,find_1));
        codes.erase(0,find_1+1);
    }
    
    for (int i = 0; i < str.size(); ++i) {
        find_1 = str[i].find("d");
        find_2 = str[i].find("D");
        if (find_1 != string::npos || find_2 != string::npos) {
            string second_pattern = "[Dd][[:digit:]]+(\\+|-)";
            regex reg(second_pattern);
            
            if (!regex_match(str[i],reg)) {
                valid = false;
                break;
            }
        }
    }
    
    str.clear();
    str.resize(0);
    str.shrink_to_fit();
    
    return valid;
}
*/

bool InputValidation::third_Validation(string codes) {
    bool valid = true;
    bool repeat = false;
    vector<tuple<char,int,char>> *vertical_point = new vector<tuple<char,int,char>>();
    vector<tuple<char,int,char>> *horizontal_point = new vector<tuple<char,int,char>>();
    vector<tuple<char,int,char>> *over_crossing_point = new vector<tuple<char,int,char>>();
    vector<tuple<char,int,char>> *under_crossing_point = new vector<tuple<char,int,char>>();
    //vector<tuple<char,int,char>> *disjoint_link = new vector<tuple<char,int,char>>();
    int ocp_index_max = 0, ucp_index_max = 0, vp_index_max = 0, hp_index_max = 0; // dl_index_max = 0;
    
    //string third_pattern = "[DdHhVvOoUu][[:digit:]]+(\\+|-)";
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
        } /* else if (type == 'd' || type == 'D') {
            if (!(repeat = isRepeatPoint(horizontal_point, &index))) {
                if (dl_index_max < index)
                    dl_index_max = index;
                disjoint_link->push_back(make_tuple(type,index,sign));
            }
        } */
        
        if (repeat) {
            valid = false;
            cout << "There point " << type << index << sign << " input more than once. Please check the codes." << endl;
            break;
        }
        
        currentMatch++;
    }

    if (ocp_index_max != over_crossing_point->size()) {
        valid = false;
        cout << "The maximun index of over-crossing point is " << ocp_index_max <<"." << endl;
        cout << "But the size of collection of over-crossing points is " << over_crossing_point->size() << "." << endl;
        cout << "Some over-crossing points are missed. Please check the codes." << endl;
    }

    if (ucp_index_max != under_crossing_point->size()) {
        valid = false;
        cout << "The maximun index of under-crossing point is " << ucp_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << under_crossing_point->size() << "." << endl;
        cout << "Some under-crossing points are missed. Please check the codes." << endl;
    }

    if (vp_index_max != vertical_point->size()) {
        valid = false;
        cout << "The maximun index of under-crossing point is " << vp_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << vertical_point->size() << "." << endl;
        cout << "Some vertical points are missed. Please check the codes." << endl;
    }

    if (hp_index_max != horizontal_point->size()) {
        valid = false;
        cout << "The maximun index of under-crossing point is " << hp_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << horizontal_point->size() << "." << endl;
        cout << "Some horizontal points are missed. Please check the codes." << endl;
    }
    
    /* if (dl_index_max != disjoint_link->size()) {
        valid = false;
        cout << "The maximun index of under-crossing point is " << dl_index_max <<"." << endl;
        cout << "But the size of collection of under-crossing points is " << disjoint_link->size() << "." << endl;
        cout << "Some disjoint links are missed. Please check the codes." << endl;
    } */
    
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
                            cout << " should have the same sign of orientation, both + or both -." << endl;
                            cout << "Please check your code." << endl;
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
        cout << "Not all crossing points have its own opposite points. Please check the codes." << endl;
    }
    
    
    FV.freeVector(over_crossing_point);
    FV.freeVector(under_crossing_point);
    FV.freeVector(vertical_point);
    FV.freeVector(horizontal_point);
    //FV.freeVector(disjoint_link);
    
    if (valid == true)
        cout << "Second valid" << endl;
    
    return valid;
}

void InputValidation::getCombCodes(vector<vector<tuple<char,int,char>>> *combine_codes,string codes) {
    codes.append(",");
    size_t find;
    vector<string> *str = new vector<string>();
    vector<tuple<char,int,char>> *paragraph = new vector<tuple<char,int,char>>;
    
    while (codes != "\0") {
        find = codes.find(",");
        str->push_back(codes.substr(0,find));
        codes.erase(0,find+1);
    }
    
    for (vector<string>::iterator it_str = str->begin(); it_str != str->end(); ++it_str) {
        //string second_pattern = "[DdHhVvOoUu][[:digit:]]+(\\+|-)";
        string pattern = "[HhVvOoUu][[:digit:]]+(\\+|-)";
        regex reg(pattern);
        
        sregex_iterator currentMatch(it_str->begin(), it_str->end(), reg);
        sregex_iterator lastMatch;
        
        while (currentMatch != lastMatch) {
            char type;
            int index;
            char sign;
            
            smatch match = *currentMatch;
            stringstream ss(match.str());
            ss >> type >> index >> sign;
            paragraph->push_back(make_tuple(type,index,sign));
            currentMatch++;
        }
        combine_codes->push_back(*paragraph);
        paragraph->clear();
    }
    
    FV.freeVector(paragraph);
    FV.freeStrVector(str);
}
