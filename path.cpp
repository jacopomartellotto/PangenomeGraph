#include "path.hpp"

// Restituisce la sequenza dei nucleotidi dell'intero percorso
string Path::getSequence(){
    string seq_tot = "";
    for (int i = 0; i < segments.size(); i++){
        if (orientations[i] == '+'){
            seq_tot += segments[i].sequence; // Aggiunge la sequenza del segmento se orientato positivamente
        } else {
            seq_tot += segments[i].complementary(); // Aggiunge la sequenza complementare se orientato negativamente
        }
    }
    return seq_tot;
}

// Stampa il percorso mostrando i nomi dei segmenti e le loro orientazioni
void Path::printPath(){
    for (int i = 0; i < segments.size(); i++){
        cout << segments[i].name << orientations[i] << " "; 
        if (i < segments.size() - 1){ // Se non è l'ultimo segmento
            cout << "-> "; 
        }
    }
    cout << endl;
}
