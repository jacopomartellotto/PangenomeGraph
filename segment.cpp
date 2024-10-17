#include "segment.hpp"
#include <unordered_map>
#include <string>

// Implementazione del metodo complementary() della classe Segment
string Segment::complementary(){
    // Mappa statica per ottenere il complemento di ogni base nucleotidica
    // Importante: l'adenina(A) si lega sempre alla timina(T) e la citosina(C) si lega sempre alla guanina(G)
    static unordered_map<char, char> complement = {
        {'A', 'T'},
        {'T', 'A'},
        {'C', 'G'},
        {'G', 'C'}
    };
    // Inizializzo una stringa per la sequenza complementare inversa
    string comp(sequence.size(), ' ');
    // Itero sulla sequenza originale per costruire la sequenza complementare inversa
    for (int i = 0; i < sequence.size(); i++){
        // Ottengo il complemento della base corrente e lo posiziona all'indice corretto
        comp[ sequence.size() - i - 1 ] = complement[sequence[i]];
    }
    // Restituisce la sequenza complementare inversa
    return comp;
}
