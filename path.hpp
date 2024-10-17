#ifndef PATH_HPP
#define PATH_HPP

#include <iostream>
#include <vector>
#include <string>
#include "segment.hpp"

using namespace std;

// Classe che rappresenta un percorso nel grafo
class Path {
    public:
        string from;                 // Nodo di partenza del percorso
        string to;                   // Nodo di arrivo del percorso
        vector<Segment> segments;    // Lista dei segmenti che compongono il percorso
        vector<char> orientations;   // Orientamenti dei segmenti nel percorso
        string getSequence();        // Restituisce la sequenza associata al percorso
        void printPath();            
};

#endif
