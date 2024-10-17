#include "datasetParser.hpp"

using namespace std;

// Costruttore della classe DatasetParser
DatasetParser::DatasetParser(string filename) {
    this->filename = filename;
    openFile(filename);
}

// Distruttore della classe DatasetParser
DatasetParser::~DatasetParser() {
    file.close();
}

// Metodo per aprire il file specificato
void DatasetParser::openFile(string filename) {
    file.open(filename);
    if (!file.is_open()) {
        cout << "[X] Errore: Impossibile aprire il file" << endl;
        exit(1);
    }
}

// Metodo per chiudere il file aperto
void DatasetParser::closeFile() {
    file.close();
}

// Metodo per il parsing del dataset
PangenomeGraph* DatasetParser::parse(){
    PangenomeGraph *graph = new PangenomeGraph();
    readSegments(graph);
    file.clear();
    file.seekg(0); // Riporta il cursore (immaginario) all'inizio del file
    readLinks(graph);
    return graph;
}

// Legge i segmenti dal file e li aggiunge al grafo
void DatasetParser::readSegments(PangenomeGraph *graph) {
    string line;
    while (getline(file, line)) {
        if (line[0] == 'S') {
            handleSegment(graph, line);
        }
    }
}

// Legge i link dal file e li aggiunge al grafo
void DatasetParser::readLinks(PangenomeGraph *graph){
    string line;
    while (getline(file, line)) {
        if (line[0] == 'L') {
            handleLink(graph, line);
        }
    }
}

// Gestisce una linea di tipo segmento e la processa
void DatasetParser::handleSegment(PangenomeGraph *graph, string line) {
    vector<string> data = split(line, '\t');
    string name = data[1];
    string sequence = data[2];
    graph->addSegment(name, sequence);
}

// Gestisce una linea di tipo link e la processa
void DatasetParser::handleLink(PangenomeGraph *graph, string line) {
    vector<string> data = split(line, '\t');
    if (data.size() != 6) { // Se non è 6, la linea del link ("L") è corrotta
        cout << "[X] Errore: Link line corrotta" << endl;
        exit(1);
    }
    // Decodifica la linea di tipo link
    string from = data[1];
    string fromOrient = data[2];
    string to = data[3];
    string toOrient = data[4];
    string overlap = data[5];
    graph->addLink(from, fromOrient[0], to, toOrient[0], overlap);
}

// Divide una stringa in un vettore di stringhe sulla base di un delimitatore
vector<string> DatasetParser::split(std::string line, char delimiter) {
    vector<string> tokens;
    string token;
    for (char c : line) {
        if (c == delimiter) {
            tokens.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    tokens.push_back(token);
    return tokens;
}
