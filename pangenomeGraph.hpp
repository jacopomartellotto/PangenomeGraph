#ifndef PANGENOMEGRAPH_HPP
#define PANGENOMEGRAPH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <queue>
#include "link.hpp"
#include "segment.hpp"
#include "path.hpp"

using namespace std;

class PangenomeGraph {
    private:
        // Verifica se il grafo contiene cicli
        bool isCyclicUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus);

        // Rimuove link all'indietro dal grafo
        void removeBackwardLinksUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove);

        // Verifica se esiste un percorso tra due nodi
        bool pathExistsUtil(int v, char orientation, int to, vector<bool> &visitedPlus, vector<bool> &visitedMinus);

        // Trova n percorsi tra due nodi
        void findNPathsUtil(int from, char orientation, int to, vector<Path> &paths, Path &currentPath, vector<bool> &recStackPlus, vector<bool> &recStackMinus, int maxLen, int n);
    
    public:
        vector<Segment> segments;                      // Lista dei segmenti nel grafo
        unordered_map<string, int> segmentIndex;       // Mappa dei nomi dei segmenti agli indici
        vector<vector<Link>> links;                    // Lista dei link per ogni segmento
        
        // Costruttore
        PangenomeGraph();
        // Distruttore
        ~PangenomeGraph();

        // Aggiunge un segmento al grafo
        void addSegment(Segment segment);

        // Aggiunge un segmento al grafo utilizzando nome e sequenza
        void addSegment(string name, string sequence);

        // Aggiunge un link tra due segmenti nel grafo
        void addLink(string from, char fromOrient, string to, char toOrient, string overlap);

        // Stampa i segmenti del grafo
        void printSegments();

        // Stampa tutti i link del grafo
        void printLinks();

        // Stampa i link associati a un determinato segmento
        void printLinks(string segmentName);

        // Restituisce il nome del segmento dato il suo id
        string getSegmentName(int id);

        // Verifica se il grafo contiene cicli
        bool isCyclic();

        // Rimuove i link che creano cicli nel grafo
        void removeBackwardLinks();

        // Trova i nodi di destinazione (senza uscite)
        vector<string> findDestinations();

        // Trova i nodi sorgente (senza entrate)
        vector<string> findSources();

        // Verifica se esiste un percorso tra due segmenti
        bool pathExists(string from, string to);

        // Trova n percorsi tra due segmenti
        vector<Path> findNPaths(string from, string to, int n);

        // Verifica se un segmento con una certa orientazione è stato visitato
        bool isSegmentVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus);

        // Imposta il vettore di visita per un segmento con una certa orientazione
        void setVectorWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, bool value);

        // ******* PER DILETTO *******
        // Trova il percorso più breve tra due segmenti utilizzando Dijkstra
        Path dijkstra(string from, char fromOrient, string to, char toOrient);
};

#endif
