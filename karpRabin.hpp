#ifndef KARP_RABIN_HPP
#define KARP_RABIN_HPP

#include <iostream>
#include <string>
#include <vector>
#include "path.hpp"
#include "pangenomeGraph.hpp"

using namespace std;

// Classe che implementa l'algoritmo di Karp-Rabin per la ricerca della sequenza nucleotidica nel pangenoma
class KarpRabin {
    private:
        uint64_t prime_number;   // Numero primo utilizzato per il calcolo degli hash
        uint64_t hash;           // Hash corrente calcolato
        uint16_t alphabet_size;  // Dimensione dell'alfabeto (numero di caratteri possibili)
        string pattern;          // Pattern da cercare
        uint64_t pattern_hash;   // Hash del pattern da cercare
        
    public:
        // Costruttore della classe KarpRabin che inizializza la dimensione dell'alfabeto e il numero primo
        KarpRabin(uint16_t alphabet_size, uint64_t prime_number);
        
        // Calcola l'hash di una data sequenza di caratteri
        uint64_t hashString(string sequence);
        
        // Aggiorna l'hash in modo incrementale quando si scorre la sequenza
        uint64_t rehash(uint64_t old_hash, char old_char, char new_char, uint32_t pattern_size);
        
        // Esegue l'algoritmo di Karp-Rabin su un testo e un pattern forniti
        bool run(string text, string pattern);
        
        // Esegue l'algoritmo di Karp-Rabin su una lista di percorsi e un pattern
        bool run(vector<Path> paths, string pattern);
        
        // Funzione ricorsiva di utilità per eseguire l'algoritmo su un grafo pangenomico
        bool runUtil(PangenomeGraph graph, string sequence, int sequence_offset, uint64_t hash, int segment_index, char orientation, int prev_size, vector<bool> &visitedPlus, vector<bool> &visitedMinus);
        
        // Confronta due stringhe per verificare se sono identiche
        bool string_compare(string text, string pattern);
        
        // Costruisce un grafo a partire da una lista di percorsi forniti
        void getGraphFromPaths(PangenomeGraph &graph, vector<Path> paths);
};

#endif
