#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <iostream>
#include <string>

using namespace std;

// Classe che rappresenta un segmento nel grafo
class Segment{
    public:
        int id;                  // Id del segmento
        string name;             // Nome del segmento
        string sequence;         // Sequenza di nucleotidi associata al segmento
        string complementary();  // Restituisce la sequenza complementare del segmento
};

#endif
