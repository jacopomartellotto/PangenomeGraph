#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>         
#include "pangenomeGraph.hpp" 

vector<string> getSDPairs(PangenomeGraph* graph, const std::string& filename, int distMAX);

#endif 
