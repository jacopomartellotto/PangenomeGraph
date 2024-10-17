#ifndef LINK_HPP
#define LINK_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Struttura che rappresenta un collegamento (link) tra due segmenti nel grafo
struct Link {
    int from;            // Id del segmento di partenza
    char fromOrient;     // Orientamento del segmento di partenza ('+' o '-')
    int to;              // Id del segmento di arrivo
    char toOrient;       // Orientamento del segmento di arrivo ('+' o '-')
    string overlap;      // Sequenza di overlap tra i due segmenti
};

#endif
