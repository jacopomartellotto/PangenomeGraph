#include <iostream>
#include "pangenomeGraph.hpp"
#include "datasetParser.hpp"
#include "karpRabin.hpp"
#include "util.hpp"
#include <random>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;


int main() {
    // Inizializzo il parser del dataset con il percorso del file del grafo
    DatasetParser parser("datasets/DRB1.gfa");

    //DatasetParser parser("datasets/chrY.hprc-v1.0-pggb.gfa");

    cout << "[+] Parsing del grafo..." << endl;
    // Eseguo il parsing del grafo dal dataset
    PangenomeGraph *graph = parser.parse();
    cout << "[+] Parsing completato" << endl;

    // Verifico se il grafo è ciclico
    if (graph->isCyclic()){
        cout << "[+] Il grafo e' ciclico" << endl;
        cout << "[+] Rimozione dei back edges" << endl;
        // Rimuovo i collegamenti all'indietro per convertire il grafo in aciclico
        graph->removeBackwardLinks();
    } 
    else {
        cout << "[+] Il grafo e' aciclico" << endl;
    }

    vector<string> sode = getSDPairs(graph,"output.txt");

    string filename = "s1_d1.txt";
    ifstream file_check(filename);
    string so_str, de_str;

    if (file_check) {
        cout << "Il file esiste." << endl;

        // Leggi la prima riga del file
        string line;
        if (getline(file_check, line)) {
            // Estrai i valori so_str e de_str dalla riga letta
            istringstream iss(line);
            iss >> so_str >> de_str;

            cout << "Contenuto della prima riga:" << endl;
            cout << "so_str: " << so_str << ", de_str: " << de_str << endl;
        }
        file_check.close();
    }
    else {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Errore nell'apertura del file: " << filename << endl;
            return 1;
        }
        // Genero un numero casuale tra sode[0] e sode[1] per la sorgente e la destinazione
        int distMAX = 6; // Limitiamo la distanza in maniera arbitraria per evitare che l'algoritmo impieghi troppo tempo
        random_device rd;  // Seed
        mt19937 gen(rd()); // Generatore Mersenne Twister
        int sode0 = stoi(sode[0]); // Converto la stringa in intero
        int sode1 = stoi(sode[1]);
        uniform_int_distribution<> distrib(sode0, sode1); 
        int so_int = distrib(gen);
        int de_int = so_int + distMAX;
        string so_str = to_string(so_int); // Converto l'intero in stringa
        string de_str = to_string(de_int);
        outFile << so_str << " " << de_str << endl;
        outFile.close();
        cout << "File creato e scrittura completata con successo!" << endl;
    }


    // Definisco arbitrariamente i nodi di origine e destinazione per la ricerca del percorso
    string source  = so_str; //"20"; "465"; "303"
    string destination = de_str; //"28"; "469"; "309"

    // Verifico se esiste un percorso tra la sorgente e la destinazione
    if ( graph->pathExists(source, destination) ){
        cout << "Il percorso da " << source << " a " << destination << " esiste" << endl;
    } else {
        cout << "Il percorso da " << source << " a " << destination << " non esiste" << endl;
    }

    int n = 10; //n e' il numero di percorsi che voglio trovare
    cout << "[+] Stampo al massimo "<< n << " percorsi" << endl;

    // Trovo fino a n percorsi tra sorgente e destinazione
    vector<Path> paths = graph->findNPaths(source, destination, n);
    cout << "Numero di percorsi trovati: " << paths.size() << endl;

    // Itero e visualizzo ciascun percorso con il suo contatore e la sua sequenza
    for (size_t i = 0; i < paths.size(); ++i) {
        cout << "[-] Percorso " << (i + 1) << ":" << endl;
        paths[i].printPath();
        cout << paths[i].getSequence() << endl;
    }

    // Inizializzo l'algoritmo di Karp-Rabi-n con la lunghezza del pattern e un numero primo
    KarpRabin kr(4, 101);
    string pattern = "GATA"; // pattern da cercare

    /*
    ***** PER DILETTO *****
    graph->dijkstra(source, '+', destination, '+').printPath();
    */
     
    // Eseguo l'algoritmo di Karp-Rabin sui percorsi per trovare il pattern
    bool A = kr.run(paths, pattern);
    cout << "kr.run(paths, pattern): " << A << endl;  

    // Libero la memoria e pulisce il grafo
    delete graph;
    return 0;
}