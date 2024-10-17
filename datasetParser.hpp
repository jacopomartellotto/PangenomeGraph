#ifndef DATASETPARSER_HPP
#define DATASETPARSER_HPP

#include <iostream>     
#include <fstream>      
#include <string>       
#include <vector>      
#include "pangenomeGraph.hpp" 


class DatasetParser {
    private:
        std::string filename;
        std::ifstream file;

    public:
        // Costruttore della classe DatasetParser.
        DatasetParser(std::string filename);

        // Distruttore della classe DatasetParser.
        // Viene chiamato automaticamente quando l'oggetto viene distrutto (ad esempio, quando esce dallo scope).
        ~DatasetParser();

        // Metodi per aprire e chiudere il file GFA.
        void openFile(std::string filename);
        void closeFile();

        // Legge il file e costruisce un oggetto PangenomeGraph basato sui dati trovati. 
        // Restituisce:
        //   - Un puntatore a un oggetto PangenomeGraph che rappresenta il grafo costruito.
        PangenomeGraph* parse();

        // Metodo per leggere tutti i segmenti (nodi) dal file GFA e aggiungerli al grafo.
        void readSegments(PangenomeGraph *graph);

        // Metodo per leggere tutti i collegamenti (archi) dal file GFA e aggiungerli al grafo.
        void readLinks(PangenomeGraph *graph);

        // Metodo per gestire una linea che definisce un segmento (nodo) nel file GFA.
        // Tale linea comincia con 'S'
        // Individua l'ID del segmento ('S') e la sequenza di nucleotidi associata.
        void handleSegment(PangenomeGraph *graph, std::string line);

        // Metodo per gestire una linea che definisce un collegamento (arco) nel file GFA.
        // Tale linea comincia con 'L'
        // Individua l'ID del segmento ('L') e come i segmenti sono connessi tra loro.
        void handleLink(PangenomeGraph *graph, std::string line);

        // Metodo per dividere una stringa in base a un delimitatore.
        std::vector<std::string> split(std::string line, char delimiter);
};

#endif 
