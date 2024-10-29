#include <string>
#include <iostream>
#include <fstream> 
#include "path.hpp"
#include "pangenomeGraph.hpp"

using namespace std;

vector<string> getSDPairs(PangenomeGraph* graph, const string& filename, int distMAX) {
    // Apri un file in cui salvare i risultati
    ofstream outFile(filename);
    
    vector<string> sode = {"0", "0"};
    if (!outFile) {
        cerr << "Errore nell'apertura del file: " << filename << endl;
        return sode;
    }

    // Trovo le coppie sorgente-destinazione
    vector<string> sources = graph->findSources();
    vector<string> dests = graph->findDestinations();

    // Scrivi i risultati nel file
    for (const string& source : sources) {
        for (const string& dest : dests) {
            if (graph->pathExists(source, dest)) {
                outFile << "Path from " << source << " to " << dest << " YES" << endl;
                int source_int = stoi(source);
                int dest_int = stoi(dest);
                if (sode[0]=="0" && (dest_int - source_int) > distMAX) {
                    sode[0]=source;
                    sode[1]=dest;
                }
            } else {
                outFile << "Path from " << source << " to " << dest << " NO" << endl;
            }
        }
    }

    // Chiudi il file dopo aver scritto tutto
    outFile.close();
    cout << "Risultati salvati nel file: " << filename << endl;
    return sode;
}
