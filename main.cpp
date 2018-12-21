//
//  main.cpp
//  FinalYearProject
//
//  Created by Yijia Zhao on 18/09/2018.
//  Copyright © 2018 Yijia Zhao. All rights reserved.
//

#include "InputValidation.hpp"
#include "FreeVectors.hpp"

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <algorithm>

// vertex name property
typedef boost::property<boost::vertex_name_t, std::tuple<char,int,bool>> vertices;
// define edges between two vertices
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,vertices> Graph;
typedef boost::graph_traits<Graph>::vertex_iterator Vertice_Iter;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertice_Descrip;
typedef boost::property_map<Graph, boost::vertex_name_t>::type Vertice_Type;
typedef std::pair<Vertice_Iter, Vertice_Iter> Vertice_Iter_Pair;
typedef boost::graph_traits<Graph>::edge_iterator Edge_Iter;

typedef std::pair<std::pair<std::tuple<char,int,bool>, std::tuple<char,int,bool>>,bool> Edge_with_Sign;
typedef std::vector<Edge_with_Sign> Edge_Pair;
typedef std::tuple<char,int,bool> Vertice_Property;
typedef std:: vector<std::vector<Edge_with_Sign>> Circle;

Edge_with_Sign getNextEdge_Edge(Vertice_Property &point, bool &arcsign, Edge_Pair *edgePair, Vertice_Property &previous, bool &h_not_exist, bool &v_not_exist) {
    Edge_with_Sign nextEdge;
    
    if (get<0>(point) == 'h') {
        for (Edge_Pair::iterator it = edgePair->begin(); it != edgePair->end(); ++it) {
            if ((*it).first.first == point) {
                if ((*it).second == arcsign) {
                    if (get<0>(previous) == 'o' || get<0>(previous) == 'u') {
                        if (get<0>((*it).first.second) == 'c' || get<0>((*it).first.second) == 'h') {
                            nextEdge = (*it);
                            break;
                        }
                    } else if (get<0>(previous) == 'h' && get<1>(point) == get<1>(previous)) {
                        if (get<0>((*it).first.second) == 'c') {
                            nextEdge = (*it);
                            break;
                        } else if (get<0>((*it).first.second) == 'h') {
                            if (get<1>(point) != get<1>((*it).first.second)) {
                                nextEdge = (*it);
                                break;
                            }
                        }
                    } else if (get<0>(previous) == 'c' || (get<0>(previous) == 'h' && get<1>(point) != get<1>(previous))) {
                        if (get<0>((*it).first.second) == 'o' || get<0>((*it).first.second) == 'u') {
                            nextEdge = (*it);
                            break;
                        } else if (get<0>((*it).first.second) == 'h') {
                            if (get<1>(point) == get<1>((*it).first.second)) {
                                nextEdge = (*it);
                                break;
                            }
                        }
                    }
                }
            }
        }
    } else if (get<0>(point) == 'v') {
        for (Edge_Pair::iterator it = edgePair->begin(); it != edgePair->end(); ++it) {
            if ((*it).first.first == point) {
                if ((*it).second == arcsign) {
                    if (get<0>(previous) == 'o' || get<0>(previous) == 'u') {
                        if (get<0>((*it).first.second) == 'c' || get<0>((*it).first.second) == 'v') {
                            nextEdge = (*it);
                            break;
                        }
                    } else if (get<0>(previous) == 'v' && get<1>(point) == get<1>(previous)) {
                        if (get<0>((*it).first.second) == 'c') {
                            nextEdge = (*it);
                            break;
                        } else if (get<0>((*it).first.second) == 'v') {
                            if (get<1>(point) != get<1>((*it).first.second)) {
                                nextEdge = (*it);
                                break;
                            }
                        }
                    } else if (get<0>(previous) == 'c' || (get<0>(previous) == 'v' && get<1>(point) != get<1>(previous))) {
                        if (get<0>((*it).first.second) == 'o' || get<0>((*it).first.second) == 'u') {
                            nextEdge = (*it);
                            break;
                        } else if (get<0>((*it).first.second) == 'v') {
                            if (get<1>(point) == get<1>((*it).first.second)) {
                                nextEdge = (*it);
                                break;
                            }
                        }
                    }
                }
            }
        }
    } else if (get<0>(point) == 'c') {
        for (Edge_Pair::iterator it = edgePair->begin(); it != edgePair->end(); ++it) {
            if ((*it).first.first == point) {
                if ((*it).second == arcsign) {
                    if (get<0>(previous) == 'v') {
                        if (get<0>((*it).first.second) == 'h' || get<0>((*it).first.second) == 'c') {
                            nextEdge = (*it);
                            break;
                        }
                    } else if (get<0>(previous) == 'h')  {
                        if (get<0>((*it).first.second) == 'v' || get<0>((*it).first.second) == 'c') {
                            nextEdge = (*it);
                            break;
                        }
                    } else if (get<0>(previous) == 'c') {
                        if (!h_not_exist && v_not_exist) {
                            if (get<0>((*it).first.second) == 'h') {
                                nextEdge = (*it);
                                break;
                            }
                        } else if (!v_not_exist && h_not_exist) {
                            if (get<0>((*it).first.second) == 'v') {
                                nextEdge = (*it);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return nextEdge;
}

Edge_with_Sign getNextEdge_Crossing(Vertice_Property &point, bool &arcsign, Edge_Pair *edgePair) {
    Edge_with_Sign nextEdge;
    
    for (Edge_Pair::iterator it = edgePair->begin(); it != edgePair->end(); ++it) {
        if ((*it).first.first == point) {
            if ((*it).second == arcsign) {
                nextEdge = (*it);
                break;
            }
        }
    }
    
    return nextEdge;
}

void addEdges(Vertice_Descrip *first, Vertice_Descrip *second, Vertice_Type &format, Graph *g) {
    Vertice_Iter_Pair vp;
    Vertice_Iter_Pair vp2;
    for (vp = boost::vertices(*g); vp.first != vp.second; ++vp.first) {
        if (format[*vp.first] == format[*first]) {
            for (vp2 = boost::vertices(*g); vp2.first != vp2.second; ++vp2.first) {
                if (format[*vp2.first] == format[*second]) {
                    boost::add_edge(*vp.first, *vp2.first,*g);
                    break;
                }
            }
            break;
        }
    }
}

bool sortByIndex(std::tuple<char,int,bool> &a, std::tuple<char,int,bool> &b) {
    return (std::get<1>(a) < std::get<1>(b));
}

int main(void)
{
    std::vector<std::vector<std::tuple<char,int,bool>>> *combine_codes = new std::vector<std::vector<std::tuple<char,int,bool>>>();
    const std::tuple<char,int,bool> corner_vertice = std::make_tuple('c',0,true);
    std::vector<std::tuple<char,int,bool>> *hor_vertice = new std::vector<std::tuple<char,int,bool>>();
    std::vector<std::tuple<char,int,bool>> *ver_vertice = new std::vector<std::tuple<char,int,bool>>();
    std::vector<std::tuple<char,int,bool>> *cross_vertice = new std::vector<std::tuple<char,int,bool>>();
    InputValidation IV;
    FreeVectors FV;
    Graph g;

//    std::string codes = "";
//
//    bool all_satisfy = true;
//    bool b_valid = false;
//    std::cout << "Codes are consisting of types, indexs and signs of orientations of points." << std::endl;
//    std::cout << "Please use comma to seperate different circles of point." << std::endl;
//    std::cout << "Type: h(Horizontal), v(Vertical), o(Overcorssing), u(Undercrossing)" << std::endl;
//    std::cout << "e.g: h1+v1-o1-u2+,h2+u1-v2+o2+" << std::endl;
//    std::cout << "Please input your codes:" << std::endl;
//
//    while(!b_valid || !all_satisfy) {
//        if (all_satisfy == false)
//            std::cout << "Please check and input codes again" << std::endl;
//
//        std::getline(std::cin,codes);
//        boost::to_lower(codes);
//        if (codes.empty() == false)
//            codes = IV.checkSpace(codes);
//
//        if ((b_valid = IV.basic_Validation(codes))) {
//            if ((all_satisfy = IV.second_Validation(codes,hor_vertice,ver_vertice,cross_vertice))) {
//                std::cout << "Input codes passed all checks, your code is: ";
//                IV.getCombCodes(combine_codes, codes);
//            }
//        }
//    }
//
//    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin();it != combine_codes->end(); ++it) {
//        if (it->empty() == false) {
//            for (std::vector<std::tuple<char,int,bool>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
//                std::cout << std::get<0>(*it_tp) << std::get<1>(*it_tp) << std::get<2>(*it_tp);
//            }
//            std::cout << std::endl;
//        } else {
//            std::cout << "," << std::endl;
//        }
//    }
    
    std::string codes = "h1+o1+u2+v1-u1+o2+"; //o1+u2+v1-u1+o2+
    boost::to_lower(codes);
    IV.second_Validation(codes,hor_vertice,ver_vertice,cross_vertice);
    IV.getCombCodes(combine_codes, codes);
    bool v_not_exist = false, h_not_exist = false;
    
    // sort vertices by their index in ascending order.
    std::sort(hor_vertice->begin(),hor_vertice->end(),sortByIndex);
    std::sort(ver_vertice->begin(),ver_vertice->end(),sortByIndex);
    std::sort(cross_vertice->begin(),cross_vertice->end(),sortByIndex);
    
    if (hor_vertice->size() == 0)
        h_not_exist = true;
    
    if (ver_vertice->size() == 0)
        v_not_exist = true;
    
    std::cout << "h exists: " << h_not_exist << ", v exists: " << v_not_exist << std::endl;
    int num_vertices = (int)hor_vertice->size() + (int)ver_vertice->size() + (int)cross_vertice->size() + 1;
    std::cout << num_vertices << " vertices" << std::endl;
    
    Vertice_Descrip *vertice_collection = new Vertice_Descrip[num_vertices];
    Vertice_Type format = get(boost::vertex_name, g);
    
    int position = 0;
    // add vertices into the graph
    *vertice_collection = boost::add_vertex(corner_vertice,g);
    position++;
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = hor_vertice->begin(); it != hor_vertice->end(); ++it) {
        *(vertice_collection + position) = boost::add_vertex(*it,g);
        position++;
    }
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = ver_vertice->begin(); it != ver_vertice->end(); ++it) {
        *(vertice_collection + position) = boost::add_vertex(*it,g);
        position++;
    }
    
    for (std::vector<std::tuple<char,int,bool>>::iterator it = cross_vertice->begin(); it != cross_vertice->end(); ++it) {
        *(vertice_collection + position) = boost::add_vertex(*it, g);
        position++;
    }
    
    // check whether vertices are added into the graph
    Vertice_Iter v,vend;
    for (boost::tie(v, vend) = boost::vertices(g); v != vend; ++v) {
        if (get<1>(format[*v]) != 0) {
            if (get<2>(format[*v]) == true)
                std::cout << get<0>(format[*v]) << get<1>(format[*v]) << "+ ";
            else
                std::cout << get<0>(format[*v]) << get<1>(format[*v]) << "- ";
        } else
            std::cout << get<0>(format[*v]) << get<1>(format[*v]) << " ";
    }
    std::cout << "\n\n";
    
    Vertice_Iter_Pair vp;
    Vertice_Iter_Pair vp2;
    
    Edge_Pair *edgePair = new Edge_Pair();
    
    int prefix_pos_v = (int)hor_vertice->size();
    int prefix_pos_c = (int)hor_vertice->size() + (int)ver_vertice->size();
    for (std::vector<std::vector<std::tuple<char,int,bool>>>::iterator it = combine_codes->begin(); it != combine_codes->end(); ++it) {
        if (!it->empty()) {
            for (std::vector<std::tuple<char,int,bool>>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
                int first_pos = 0, second_pos = 0;
                if (it_tp != it->end() - 1) {
                    // get the vertice position in the vertice set
                    if (get<0>(*it_tp) == 'h')
                        first_pos = get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'v')
                        first_pos = prefix_pos_v + get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'u' || get<0>(*it_tp) == 'o')
                        first_pos = prefix_pos_c + get<1>(*it_tp);
                    
                    if (get<0>(*(it_tp + 1)) == 'h')
                        second_pos = get<1>(*(it_tp + 1));
                    else if (get<0>(*(it_tp + 1)) == 'v')
                        second_pos = prefix_pos_v + get<1>(*(it_tp + 1));
                    else if (get<0>(*(it_tp + 1)) == 'u' || get<0>(*(it_tp + 1)) == 'o')
                        second_pos = prefix_pos_c + get<1>(*(it_tp + 1));
                    
                    edgePair->push_back(std::make_pair(std::make_pair(*it_tp, *(it_tp+1)),true));
                    edgePair->push_back(std::make_pair(std::make_pair(*(it_tp+1), *it_tp),false));
                    
                    addEdges((vertice_collection + first_pos), (vertice_collection + second_pos), format, &g);
                } else {
                    if (get<0>(*it_tp) == 'h')
                        first_pos = get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'v')
                        first_pos = prefix_pos_v + get<1>(*it_tp);
                    else if (get<0>(*it_tp) == 'u' || get<0>(*it_tp) == 'o')
                        first_pos = prefix_pos_c + get<1>(*it_tp);
                    
                    if (get<0>(*(it_tp - (it->size() - 1))) == 'h')
                        second_pos = get<1>(*(it_tp - (it->size() - 1)));
                    else if (get<0>(*(it_tp - (it->size() - 1))) == 'v')
                        second_pos = prefix_pos_v + get<1>(*(it_tp - (it->size() - 1)));
                    else if (get<0>(*(it_tp - (it->size() - 1))) == 'u' || get<0>(*(it_tp - (it->size() - 1))) == 'o')
                        second_pos = prefix_pos_c + get<1>(*(it_tp - (it->size() - 1)));
                    
                    edgePair->push_back(std::make_pair(std::make_pair(*it_tp, *(it_tp - (it->size() - 1))), true));
                    edgePair->push_back(std::make_pair(std::make_pair(*(it_tp - (it->size() - 1)), *it_tp), false));
                    
                    addEdges((vertice_collection + first_pos), (vertice_collection + second_pos), format, &g);
                }
            }
        }
    }
    
    position = 0;
    int prefix_pos = 1;
    if (hor_vertice->size() > 0) {
        for (int i = 0; i < hor_vertice->size(); ++i) {
            if (i != hor_vertice->size()-1) {
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*(vertice_collection+prefix_pos + i + 1)]),true));
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection+prefix_pos + i + 1)], format[*(vertice_collection + prefix_pos + i)]),false));
                addEdges((vertice_collection + prefix_pos + i), (vertice_collection+prefix_pos + i + 1), format, &g);
            } else {
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*vertice_collection]),true));
                edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*(vertice_collection + prefix_pos + i)]), false));
                addEdges((vertice_collection + prefix_pos + i), (vertice_collection), format, &g);
                
                edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*(vertice_collection + prefix_pos)]), true));
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection + prefix_pos)], format[*vertice_collection]), false));
                addEdges((vertice_collection), (vertice_collection + prefix_pos), format, &g);
            }
        }
    } else if (hor_vertice->size() == 0) {
        edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*vertice_collection]), true));
        edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*vertice_collection]), false));
        for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
            if (format[*vp.first] == format[*vertice_collection]) {
                boost::add_edge(*vp.first, *vp.first, g);
                break;
            }
        }
    }

    // add vertical edges to the graph and build the vertical edge set
    prefix_pos = (int)hor_vertice->size() + 1;

    if (ver_vertice->size() > 0) {
        for (int i = 0; i < ver_vertice->size(); ++i) {
            if (i != ver_vertice->size()-1) {
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*(vertice_collection+prefix_pos + i + 1)]),true));
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection+prefix_pos + i + 1)], format[*(vertice_collection + prefix_pos + i)]),false));
                addEdges((vertice_collection + prefix_pos + i), (vertice_collection + prefix_pos+i + 1), format, &g);
            } else {
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection + prefix_pos + i)], format[*vertice_collection]),true));
                edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*(vertice_collection + prefix_pos + i)]), false));
                addEdges((vertice_collection + prefix_pos + i), (vertice_collection), format, &g);
                
                edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*(vertice_collection + prefix_pos)]), true));
                edgePair->push_back(std::make_pair(std::make_pair(format[*(vertice_collection + prefix_pos)], format[*vertice_collection]), false));
                addEdges(vertice_collection, (vertice_collection + prefix_pos), format, &g);
            }
        }
    } else if (ver_vertice->size() == 0) {
        edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*vertice_collection]), true));
        edgePair->push_back(std::make_pair(std::make_pair(format[*vertice_collection], format[*vertice_collection]), false));
        for (vp = boost::vertices(g); vp.first != vp.second; ++vp.first) {
            if (format[*vp.first] == format[*vertice_collection]) {
                boost::add_edge(*vp.first, *vp.first, g);
                break;
            }
        }
    }

    // print out all edges in the graph
//    Edge_Iter  ei, ei_end ;
//    std::cout << "Edge in g  = [";
//    for(boost::tie(ei, ei_end)=boost::edges(g); ei != ei_end; ++ei) {
//        if (get<2>(format[boost::source(*ei,g)]) == true)
//            std::cout<<" ("<< get<0>(format[boost::source(*ei, g)]) << get<1>(format[boost::source(*ei,g)]) << "+" << ", ";
//        else if (get<2>(format[boost::source(*ei,g)]) == false)
//            std::cout<<" ("<< get<0>(format[boost::source(*ei, g)]) << get<1>(format[boost::source(*ei,g)]) << "-" << ", ";
//
//        if (get<2>(format[boost::target(*ei,g)]) == true)
//            std::cout << get<0>(format[boost::target(*ei, g)]) << get<1>(format[boost::target(*ei,g)]) << "+" <<") ";
//        else if (get<2>(format[boost::target(*ei,g)]) == false)
//            std::cout << get<0>(format[boost::target(*ei, g)]) << get<1>(format[boost::target(*ei,g)]) << "-" <<") ";
//    }
//    std::cout<<"]\n"<<std::endl;
    
    // print out all edges including edges whose orientations against original directions
    std::cout << "Number of edges in the Graph:" << (edgePair->size() / 2) << std::endl;
    std::cout << "Total number of edges both with original : " << edgePair->size() << std::endl;
    std::cout << "{ ";
    for (Edge_Pair::iterator it = edgePair->begin(); it != edgePair->end(); ++it) {
        if (get<2>((*it).first.first) == true)
            std::cout << "(" << get<0>((*it).first.first) << get<1>((*it).first.first) << "+" << ",";
        else if (get<2>(get<0>(*it).first) == false)
            std::cout << "(" << get<0>((*it).first.first) << get<1>((*it).first.first) << "-" << ",";

        if (get<2>((*it).first.second) == true)
            std::cout << get<0>((*it).first.second) << get<1>((*it).first.second) << "+" << ")";
        else if (get<2>(get<0>(*it).second) == false)
            std::cout << get<0>((*it).first.second) << get<1>((*it).first.second) << "-" << ")";

        if ((*it).second == true)
            std::cout << "+,";
        else if ((*it).second == false)
            std::cout << "-,";
    }
    std::cout << "}" << std::endl;
    
    int cycle = 0;
    Edge_Pair *usedPair = new Edge_Pair();
    Edge_with_Sign inputPair, outputPair;
    Edge_Pair tempCircle;
    Circle *circleSet = new Circle();

    while (usedPair->size() != edgePair->size()) {
        for (Edge_Pair::iterator it = edgePair->begin(); it != edgePair->end(); ++it) {
            if (std::find(usedPair->begin(),usedPair->end(),(*it)) == usedPair->end()) {
                inputPair = (*it);
                break;
            }
        }

        if (tempCircle.size() != 0) {
            tempCircle.clear();
            tempCircle.resize(0);
        }

        while (std::find(usedPair->begin(),usedPair->end(),inputPair) == usedPair->end()) {
            char type = get<0>(inputPair.first.second);
            int index = get<1>(inputPair.first.second);
            int sign = get<2>(inputPair.first.second);
            bool arcsign;
            Vertice_Property outPoint;

            if (get<0>(inputPair.first.second) == 'o') {
                type = 'u';
                if (sign == true)
                    arcsign = inputPair.second;
                else if (sign == false) {
                    if (inputPair.second == true)
                        arcsign = false;
                    else if (inputPair.second == false)
                        arcsign = true;
                }
            } else if (get<0>(inputPair.first.second) == 'u') {
                type = 'o';
                if (sign == true) {
                    if (inputPair.second == true)
                        arcsign = false;
                    else if (inputPair.second == false)
                        arcsign = true;
                } else if (sign == false)
                    arcsign = inputPair.second;
            } else if (get<0>(inputPair.first.second) == 'h') {
                if (sign == true) {
                    if (get<0>(inputPair.first.first) == 'c')
                        arcsign = inputPair.second;
                    else if (get<0>(inputPair.first.first) == 'o' || get<0>(inputPair.first.first) == 'u') {
                        if (inputPair.second == true)
                            arcsign = false;
                        else if (inputPair.second == false)
                            arcsign = true;
                    } else if (get<0>(inputPair.first.first) == 'h') {
                        if (get<1>(inputPair.first.first) == get<1>(inputPair.first.second)) {
                            if (inputPair.second == true)
                                arcsign = false;
                            else if (inputPair.second == false)
                                arcsign = true;
                        } else if (get<1>(inputPair.first.first) != get<1>(inputPair.first.second))
                            arcsign = inputPair.second;
                    }
                } else if (sign == false) {
                    if (get<0>(inputPair.first.first) == 'c') {
                        if (inputPair.second == true)
                            arcsign = false;
                        else if (inputPair.second == false)
                            arcsign = true;
                    } else if (get<0>(inputPair.first.first) == 'o' || get<0>(inputPair.first.first) == 'u')
                        arcsign = inputPair.second;
                    else if (get<0>(inputPair.first.first) == 'h') {
                        if (get<1>(inputPair.first.first) != get<1>(inputPair.first.second)) {
                            if (inputPair.second == true)
                                arcsign = false;
                            else if (inputPair.second == false)
                                arcsign = true;
                        } else if (get<1>(inputPair.first.first) == get<1>(inputPair.first.second))
                            arcsign = inputPair.second;
                    }
                }
            } else if (get<0>(inputPair.first.second) == 'v') {
                if (sign == true) {
                    
                    if (get<0>(inputPair.first.first) == 'c') {
                        if (inputPair.second == false)
                            arcsign = true;
                        else if (inputPair.second == true)
                            arcsign = false;
                    } else if (get<0>(inputPair.first.first) == 'o' || get<0>(inputPair.first.first) == 'u')
                        arcsign = inputPair.second;
                    else if (get<0>(inputPair.first.first) == 'v') {
                        if (get<1>(inputPair.first.first) != get<1>(inputPair.first.second)) {
                            if (inputPair.second == true)
                                arcsign = false;
                            else if (inputPair.second == false)
                                arcsign = true;
                        } else if (get<1>(inputPair.first.first) == get<1>(inputPair.first.second))
                            arcsign = inputPair.second;
                    }
                } else if (sign == false) {
                    if (get<0>(inputPair.first.first) == 'c')
                        arcsign = inputPair.second;
                    else if (get<0>(inputPair.first.first) == 'o' || get<0>(inputPair.first.first) == 'u') {
                        if (inputPair.second == false)
                            arcsign = true;
                        else if (inputPair.second == true)
                            arcsign = false;
                    } else if (get<0>(inputPair.first.first) == 'v') {
                        if (get<1>(inputPair.first.first) == get<1>(inputPair.first.second)) {
                            if (inputPair.second == true)
                                arcsign = false;
                            else if (inputPair.second == false)
                                arcsign = true;
                        } else if (get<1>(inputPair.first.first) != get<1>(inputPair.first.second))
                            arcsign = inputPair.second;
                    }
                }
            } else if (get<0>(inputPair.first.second) == 'c') {
                if (get<0>(inputPair.first.first) == 'v') {
                    if (inputPair.second == true)
                        arcsign = false;
                    else if (inputPair.second == false)
                        arcsign = true;
                } else if (get<0>(inputPair.first.first) == 'h')
                    arcsign = inputPair.second;
                else if (get<0>(inputPair.first.first) == 'c') {
                    if (!h_not_exist && v_not_exist) {
                        if (inputPair.second == true)
                            arcsign = false;
                        else if (inputPair.second == false)
                            arcsign = true;
                    } else if (!v_not_exist && h_not_exist)
                        arcsign = inputPair.second;
                }
            }
            
            outPoint = std::make_tuple(type,index,sign);
            if (get<0>(inputPair.first.second) == 'o' || get<0>(inputPair.first.second) == 'u')
                outputPair = getNextEdge_Crossing(outPoint, arcsign, edgePair);
            else
                outputPair = getNextEdge_Edge(outPoint, arcsign, edgePair, inputPair.first.first, h_not_exist, v_not_exist);

            usedPair->push_back(inputPair);
            tempCircle.push_back(inputPair);
            inputPair = outputPair;
        }
        circleSet->push_back(tempCircle);
        cycle += 1;
    }

    std::cout << "Number of circles: " << cycle << "; number of cirecles in the set: " << circleSet->size() << std::endl;
    for (Circle::iterator it = circleSet->begin(); it != circleSet->end(); ++it) {
        std::cout << "{ ";
        for (std::vector<Edge_with_Sign>::iterator it_tp = it->begin(); it_tp != it->end(); ++it_tp) {
            if (get<2>((*it_tp).first.first) == true)
                std::cout << "(" << get<0>((*it_tp).first.first) << get<1>((*it_tp).first.first) << "+" << ",";
            else if (get<2>(get<0>(*it_tp).first) == false)
                std::cout << "(" << get<0>((*it_tp).first.first) << get<1>((*it_tp).first.first) << "-" << ",";

            if (get<2>((*it_tp).first.second) == true)
                std::cout << get<0>((*it_tp).first.second) << get<1>((*it_tp).first.second) << "+" << ")";
            else if (get<2>(get<0>(*it_tp).second) == false)
                std::cout << get<0>((*it_tp).first.second) << get<1>((*it_tp).first.second) << "-" << ")";

            if ((*it_tp).second == true)
                std::cout << "+,";
            else if ((*it_tp).second == false)
                std::cout << "-,";
        }
        std::cout << "}" << std::endl;
    }
    
    FV.freeVector_Bool(hor_vertice);
    FV.freeVector_Bool(ver_vertice);
    FV.freeVector_Bool(cross_vertice);
    FV.freeTDVector(combine_codes);
    FV.freeEdgePair(edgePair);
    FV.freeEdgePair(usedPair);
    FV.freeCycleSet(circleSet);

    // delete pointers
    delete[] vertice_collection;
    vertice_collection = NULL;
    
    return 0;
}
