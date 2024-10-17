#include <cmath>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "karpRabin.hpp"
#include "path.hpp"
#include "pangenomeGraph.hpp"

using namespace std;

// Costruttore della classe KarpRabin
KarpRabin::KarpRabin(uint16_t alphabet_size, uint64_t prime_number)
{
    // Inizializza i parametri dell'algoritmo
    this->prime_number = prime_number;    // Numero primo per il modulo nell'hash
    this->alphabet_size = alphabet_size;  // Dimensione dell'alfabeto (es. 4 per A, C, G, T)
}

// Calcola l'hash di una stringa utilizzando l'algoritmo di Karp-Rabin
uint64_t KarpRabin::hashString(string sequence)
{
    uint32_t sequence_size = sequence.size();
    uint64_t hash = 0;
    // Itera su ogni carattere della sequenza
    for (int i = 0; i < sequence.size(); i++)
    {
        // Calcola hash = (hash + carattere * (dimensione_alfabeto ^ (lunghezza_sequenza - i - 1))) modulo numero_primo
        hash += (static_cast<uint64_t>(sequence[i]) * static_cast<uint64_t>(pow(alphabet_size, sequence_size - i - 1))) % prime_number;
    }
    // Restituisce l'hash modulo numero_primo
    return hash % prime_number;
}

// Aggiorna l'hash per la nuova finestra di testo senza ricalcolare da zero
uint64_t KarpRabin::rehash(uint64_t old_hash, char old_char, char new_char, uint32_t pattern_size)
{
    // Gestisce la sottrazione in modulo per evitare risultati negativi
    // (a - b) % p = (a % p - b % p + p) % p
    int64_t new_hash = (old_hash - (static_cast<uint64_t>(old_char) * static_cast<uint64_t>(pow(alphabet_size, pattern_size - 1))) % prime_number + prime_number) % prime_number;
    new_hash = (new_hash * alphabet_size + static_cast<uint64_t>(new_char)) % prime_number;
    return new_hash;
}

// Confronta due stringhe per verificarne l'uguaglianza
bool KarpRabin::string_compare(string text, string pattern)
{
    return !text.compare(pattern);  // Restituisce True se le stringhe sono uguali
}

// Esegue l'algoritmo di Karp-Rabin
bool KarpRabin::run(string text, string pattern)
{
    uint32_t pattern_size = pattern.size();
    uint32_t text_size = text.size(); //text nel nostro caso è il path
    uint64_t pattern_hash = hashString(pattern);  // Calcola l'hash del pattern
    uint64_t text_hash = hashString(text.substr(0, pattern_size)); 

    // Confronta l'hash del pattern con quello della prima sottostringa del  path
    if (pattern_hash == text_hash && string_compare(text.substr(0, pattern_size), pattern))
    {
        return true;  // Pattern trovato all'inizio del testo
    }

    char old_char;
    char new_char;
    // Scorre il testo di una posizione alla volta
    for (int i = 0; i < text_size - pattern_size; i++)
    {
        old_char = text[i];  // Carattere che esce dalla finestra
        new_char = text[i + pattern_size];  // Carattere che entra nella finestra
        // Aggiorna l'hash per la nuova finestra
        text_hash = rehash(text_hash, old_char, new_char, pattern_size);
        // Se gli hash corrispondono, confronta le stringhe per evitare hash collision
        if (pattern_hash == text_hash && string_compare(text.substr(i + 1, pattern_size), pattern))
        {
            return true;  // Pattern trovato nel testo
        }
    }
    return false;  // Pattern non trovato
}

// Funzione ricorsiva che esegue l'algoritmo di Karp-Rabin sul grafo
bool KarpRabin::runUtil(PangenomeGraph graph, string sequence,
                        int sequence_offset, uint64_t hash, int segment_index, char orientation,
                        int prev_size, vector<bool> &visitedPlus, vector<bool> &visitedMinus)
{
    // Aggiunge il segmento alla sequenza tenendo conto dell'orientazione
    if (orientation == '+')
    {
        sequence += graph.segments[segment_index].sequence;
    }
    else
    {
        sequence += graph.segments[segment_index].complementary();
    }

    // Se la sequenza è vuota (prima chiamata), calcola l'hash della prima sottostringa
    if (sequence.empty())
    {
        hash = hashString(sequence.substr(0, pattern.size()));
    }

    // Questo ciclo controlla se nella sequenza corrente è presente il pattern
    while (sequence_offset + pattern.size() <= sequence.size())
    {
        // Alla prima iterazione, hash è -1, quindi calcola l'hash della prima sottostringa
        if (hash == -1)
        {
            hash = hashString(sequence.substr(sequence_offset, pattern.size()));
        }
        // Nelle iterazioni successive, aggiorna l'hash usando la funzione rehash
        else
        {
            hash = rehash(hash, sequence[sequence_offset - 1], sequence[sequence_offset - 1 + pattern.size()], pattern.size());
        }
        // Controlla se il segmento è già stato visitato con questa orientazione e se è già stata superata la parte precedente della sequenza
        if (graph.isSegmentVisitedWithOrientation(segment_index, orientation, visitedPlus, visitedMinus) && sequence_offset >= prev_size)
        {
            cout << "Segmento già visitato: " << graph.getSegmentName(segment_index) << endl;
            return false;
        }
        // Se gli hash corrispondono, confronta le sottostringhe per confermare la corrispondenza
        if (hash == pattern_hash && string_compare(sequence.substr(sequence_offset, pattern.size()), pattern))
        {
            cout << "Pattern trovato nella sequenza: " << sequence << endl;
            return true;  // Pattern trovato
        }
        // Incrementa l'offset per controllare la prossima sottostringa
        sequence_offset++;
    }

    // Esplora ricorsivamente i link uscenti dal segmento corrente
    for (Link link : graph.links[segment_index])
    {
        if (link.fromOrient == orientation)
        {
            // Chiamata ricorsiva per il segmento successivo
            if (runUtil(graph, sequence, sequence_offset, hash, link.to, link.toOrient, sequence.size(), visitedPlus, visitedMinus))
            {
                return true;  // Pattern trovato in un percorso successivo
            }
        }
    }
    // Se il segmento è stato esplorato completamente, viene segnato come visitato per evitare cicli
    graph.setVectorWithOrientation(segment_index, orientation, visitedPlus, visitedMinus, true);
    return false;  // Pattern non trovato in questo percorso
}

// Costruisce un grafo a partire da una lista di percorsi
void KarpRabin::getGraphFromPaths(PangenomeGraph &graph, vector<Path> paths)
{
    unordered_map<string, bool> added_segments;  // Mappa per tenere traccia dei segmenti aggiunti
    unordered_map<string, bool> added_paths;     // Mappa per tenere traccia dei link aggiunti
    // Aggiunge i segmenti al grafo dei percorsi
    for (Path path : paths)
    {
        for (int i = 0; i < path.segments.size(); i++)
        {
            // Se il segmento non è già stato aggiunto
            if (added_segments.find(path.segments[i].name) == added_segments.end())
            {
                graph.addSegment(path.segments[i]);  // Aggiunge il segmento al grafo
                added_segments[path.segments[i].name] = true;  // Segna il segmento come aggiunto
            }
        }
    }
    // Aggiunge i link al grafo dei percorsi
    for (Path path : paths)
    {
        for (int i = 0; i < path.segments.size(); i++)
        {
            if (i < path.segments.size() - 1)
            {
                string link_name = path.segments[i].name + path.orientations[i] + path.segments[i + 1].name + path.orientations[i + 1];
                // Se il link non è già stato aggiunto
                if (added_paths.find(link_name) == added_paths.end())
                {
                    // Aggiunge il link al grafo
                    graph.addLink(path.segments[i].name, path.orientations[i], path.segments[i + 1].name, path.orientations[i + 1], "");
                    added_paths[link_name] = true;  // Segna il link come aggiunto
                }
            }
        }
    }
}

// Esegue l'algoritmo di Karp-Rabin su una lista di percorsi e un pattern
bool KarpRabin::run(vector<Path> paths, string pattern)
{
    this->pattern = pattern;
    pattern_hash = hashString(pattern);  // Calcola l'hash del pattern
    uint32_t pattern_size = pattern.size();

    // Crea il grafo dei percorsi sorgente-destinazione
    PangenomeGraph graph;
    getGraphFromPaths(graph, paths);

    // (Opzionale) Stampa i link e i segmenti del grafo per debugging
    graph.printLinks();
    graph.printSegments();

    // Inizializza i vettori per tenere traccia dei segmenti visitati con entrambe le orientazioni
    vector<bool> visitedPlus(graph.segments.size(), false);
    vector<bool> visitedMinus(graph.segments.size(), false);
    string sequence = "";
    // Avvia la ricerca del pattern a partire dal primo segmento con entrambe le orientazioni
    if (runUtil(graph, sequence, 0, -1, 0, '+', 0, visitedPlus, visitedMinus) ||
        runUtil(graph, sequence, 0, -1, 0, '-', 0, visitedPlus, visitedMinus))
    {
        return true;  // Pattern trovato nel grafo
    }
    return false;  // Pattern non trovato
}
