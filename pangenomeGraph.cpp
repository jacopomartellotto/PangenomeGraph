#include "pangenomeGraph.hpp"

using namespace std;

// Costruttore della classe PangenomeGraph
PangenomeGraph::PangenomeGraph() {}

// Distruttore della classe PangenomeGraph
PangenomeGraph::~PangenomeGraph() {}

// Aggiunge un segmento al grafo
void PangenomeGraph::addSegment(Segment segment) {
    // Verifica se il segmento è già presente nel grafo utilizzando il suo nome
    if (segmentIndex.find(segment.name) != segmentIndex.end()) {
        cout << "Segment " << segment.name << " already in the graph" << endl;
        return;
    }
    // Assegna un ID al segmento basato sulla dimensione attuale del vettore segments
    segment.id = segments.size();
    // Aggiorna la mappa segmentIndex per associare il nome del segmento al suo ID
    segmentIndex[segment.name] = segments.size();
    // Aggiunge il segmento al vettore segments
    segments.push_back(segment);
    // Inizializza una lista vuota di link per questo segmento
    links.push_back(vector<Link>());
}

// Metodo per aggiungere un segmento al grafo utilizzando nome e sequenza
void PangenomeGraph::addSegment(string name, string sequence) {
    Segment segment;
    segment.id = segments.size();
    segment.name = name;
    segment.sequence = sequence;
    // Aggiunge il nuovo segmento al vettore segments
    segments.push_back(segment);
    // Aggiorna la mappa segmentIndex con il nuovo segmento
    segmentIndex[name] = segment.id;
    // Inizializza una lista vuota di link per questo segmento
    links.push_back(vector<Link>());
}

// Metodo per aggiungere un link tra due segmenti nel grafo
void PangenomeGraph::addLink(string from, char fromOrient, string to, char toOrient, string overlap) {
    Link link;
    // Imposta gli attributi del link utilizzando gli ID dei segmenti coinvolti
    link.from = segmentIndex[from];
    link.fromOrient = fromOrient;
    link.to = segmentIndex[to];
    link.toOrient = toOrient;
    link.overlap = overlap;
    // Aggiunge il link alla lista di link del segmento di partenza
    links[segmentIndex[from]].push_back(link);
}

// Metodo per stampare tutti i segmenti del grafo
void PangenomeGraph::printSegments() {
    for (Segment segment : segments) {
        cout << "Segmento: " << segment.id << " " << segment.name << " " << segment.sequence << endl;
    }
}

// Metodo per stampare tutti i link del grafo
void PangenomeGraph::printLinks() {
    for (int i = 0; i < links.size(); i++) {
        for (Link link : links[i]) {
            cout << "Link: " << getSegmentName(link.from) << " " << link.fromOrient << " "
                 << getSegmentName(link.to) << " " << link.toOrient << " " << link.overlap << endl;
        }
    }
}

// Metodo per stampare i link associati a un determinato segmento
void PangenomeGraph::printLinks(string segmentName) {
    int segmentId = segmentIndex[segmentName];
    for (Link link : links[segmentId]) {
        cout << "Link: " << getSegmentName(segmentId) << " " << link.fromOrient << " "
             << getSegmentName(link.to) << " " << link.toOrient << " " << link.overlap << endl;
    }
}

// Metodo per ottenere il nome di un segmento dato il suo ID
string PangenomeGraph::getSegmentName(int id) {
    return segments[id].name;
}

// Metodo per verificare se un segmento con una certa orientazione è stato visitato
bool PangenomeGraph::isSegmentVisitedWithOrientation(int seg_id, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus) {
    if (orientation == '+') {
        return visitedPlus[seg_id];
    } else {
        return visitedMinus[seg_id];
    }
}

// Metodo per impostare il valore di visita per un segmento con una certa orientazione
void PangenomeGraph::setVectorWithOrientation(int seg_id, char orientation, vector<bool> &vectorPlus, vector<bool> &vectorMinus, bool value) {
    if (orientation == '+') {
        vectorPlus[seg_id] = value;
    } else {
        vectorMinus[seg_id] = value;
    }
}

// Funzione di utilità per verificare se il grafo contiene cicli a partire da un nodo specifico
bool PangenomeGraph::isCyclicUtil(int vertex, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus) {
    // Se il nodo non è stato ancora visitato con questa orientazione
    if (!isSegmentVisitedWithOrientation(vertex, orientation, visitedPlus, visitedMinus)) {
        // Segna il nodo come visitato. Inoltre lo aggiunge allo stack di ricorsione per tener traccia di possibili cicli nel grafo
        setVectorWithOrientation(vertex, orientation, visitedPlus, visitedMinus, true);
        setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, true);

        // Itera su tutti i link in uscita dal nodo corrente
        for (Link link : links[vertex]) {
            // Considera solo i link con l'orientazione di partenza corretta
            if (link.fromOrient == orientation) {
                // Se il nodo di destinazione non è stato visitato, chiama ricorsivamente la funzione stessa
                if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus) &&
                    isCyclicUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
                    return true; // Ciclo trovato
                }
                // Se il nodo di destinazione è nello stack di ricorsione, c'è un ciclo
                else {
                    if ((link.toOrient == '+' && recStackPlus[link.to]) ||
                        (link.toOrient == '-' && recStackMinus[link.to])) {
                        return true; // Ciclo trovato
                    }
                }
            }
        }
    }
    // Rimuovi il nodo dallo stack di ricorsione
    setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, false);
    return false; // Nessun ciclo trovato a partire da questo nodo
}

// Metodo per verificare se il grafo contiene cicli
bool PangenomeGraph::isCyclic() {
    // Vettori per tracciare i nodi visitati e lo stack di ricorsione per entrambe le orientazioni
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    vector<bool> recStackPlus(segments.size(), false);
    vector<bool> recStackMinus(segments.size(), false);

    // Verifica la presenza di cicli a partire da ogni segmento con entrambe le orientazioni
    for (Segment segment : segments) {
        if (isCyclicUtil(segment.id, '+', visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
            return true; // Ciclo trovato
        }
        if (isCyclicUtil(segment.id, '-', visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
            return true; // Ciclo trovato
        }
    }
    return false; // Nessun ciclo nel grafo
}

// Individua e rimuovi i link che causano cicli
void PangenomeGraph::removeBackwardLinksUtil(int vertex, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove) {
    // Se il nodo non è stato ancora visitato con questa orientazione
    if (!isSegmentVisitedWithOrientation(vertex, orientation, visitedPlus, visitedMinus)) {
        // Segna il nodo come visitato. Inoltre lo aggiunge allo stack di ricorsione per tener traccia di possibili cicli nel grafo
        setVectorWithOrientation(vertex, orientation, visitedPlus, visitedMinus, true);
        setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, true);

        // Itera su tutti i link in uscita dal nodo corrente
        for (Link link : links[vertex]) {
            // Considera solo i link con l'orientazione di partenza corretta
            if (link.fromOrient == orientation) {
                // Se il nodo di destinazione non è stato visitato, chiama ricorsivamente la funzione stessa
                if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus)) {
                    removeBackwardLinksUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
                }
                // Se il nodo di destinazione è nello stack di ricorsione, il link crea un ciclo e va rimosso
                if ((recStackMinus[link.to] && link.toOrient == '-') ||
                    (recStackPlus[link.to] && link.toOrient == '+')) {
                    linksToRemove.push_back(link);
                }
            }
        }
    }
    // Rimuovi il nodo dallo stack di ricorsione
    setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, false);
}

// Metodo per rimuovere i link che causano cicli nel grafo
void PangenomeGraph::removeBackwardLinks() {
    auto start = std::chrono::high_resolution_clock::now();
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    vector<bool> recStackPlus(segments.size(), false);
    vector<bool> recStackMinus(segments.size(), false);
    vector<Link> linksToRemove;

    // Esegue la ricerca dei link da rimuovere a partire da ogni segmento
    for (Segment segment : segments) {
        removeBackwardLinksUtil(segment.id, '+', visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
        removeBackwardLinksUtil(segment.id, '-', visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
    }

    cout << "Links da rimuovere: " << linksToRemove.size() << endl;

    // Rimuove i link identificati dalla struttura dei link del grafo
    for (const auto linkPtr : linksToRemove) {
        links[linkPtr.from].erase(
            std::remove_if(links[linkPtr.from].begin(), links[linkPtr.from].end(),
                [linkPtr](const Link& link) {
                    return link.from == linkPtr.from &&
                           link.fromOrient == linkPtr.fromOrient &&
                           link.to == linkPtr.to &&
                           link.toOrient == linkPtr.toOrient &&
                           link.overlap == linkPtr.overlap;
                }
            ),
            links[linkPtr.from].end()
        );
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "Tempo di esecuzione della funzione: " << duration.count() << " microsecondi" << endl;
}

// Metodo per trovare i nodi di destinazione nel grafo (nodi senza link in uscita)
vector<string> PangenomeGraph::findDestinations() {
    vector<string> destinations;
    for (Segment segment : segments) {
        if (links[segment.id].empty()) {
            destinations.push_back(segment.name); // Nodo senza link in uscita
        }
    }
    return destinations;
}

// Metodo per trovare i nodi sorgente nel grafo (nodi senza link in entrata)
vector<string> PangenomeGraph::findSources() {
    vector<bool> isSource(segments.size(), true);
    vector<string> sources;

    // Itera su tutti i link e marca i nodi che hanno link in entrata
    for (Segment segment : segments) {
        for (Link link : links[segment.id]) {
            isSource[link.to] = false; // Nodo con link in entrata, non è una sorgente
        }
    }
    // I nodi rimasti marcati come true sono sorgenti
    for (int i = 0; i < isSource.size(); i++) {
        if (isSource[i]) {
            sources.push_back(segments[i].name);
        }
    }
    return sources;
}

// Verifica se esiste un percorso tra due nodi
bool PangenomeGraph::pathExistsUtil(int vertex, char orientation, int to, vector<bool> &visitedPlus, vector<bool> &visitedMinus) {
    // Condizione di arrivo: se il nodo corrente è il nodo di destinazione
    if (vertex == to) {
        return true; // Percorso trovato
    }
    // Segna il nodo come visitato con l'orientazione corrente
    setVectorWithOrientation(vertex, orientation, visitedPlus, visitedMinus, true);
    // Itera su tutti i link in uscita dal nodo corrente
    for (Link link : links[vertex]) {
        if (link.fromOrient == orientation) {
            // Se il nodo di destinazione non è stato visitato con la sua orientazione, richiama ricorsivamente la funzione stessa
            if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus) &&
                pathExistsUtil(link.to, link.toOrient, to, visitedPlus, visitedMinus)) {
                return true; // Percorso trovato
            }
        }
    }
    // Nessun percorso trovato a partire da questo nodo
    return false;
}

// Metodo per verificare se esiste un percorso tra due segmenti dati i loro nomi
bool PangenomeGraph::pathExists(string from, string to) {
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    // Verifica se esiste un percorso sia con orientazione '+' che '-'
    return pathExistsUtil(fromId, '+', toId, visitedPlus, visitedMinus) ||
           pathExistsUtil(fromId, '-', toId, visitedPlus, visitedMinus);
}

// Trova n percorsi tra due nodi
void PangenomeGraph::findNPathsUtil(int from, char orientation, int to, vector<Path> &paths, Path &currentPath, vector<bool> &recStackPlus, vector<bool> &recStackMinus, int maxLen, int n) {
    // Condizione di uscita: se abbiamo già trovato n percorsi
    if (paths.size() >= n) {
        return;
    }
    // Aggiunge il nodo corrente al percorso e lo segna nello stack di ricorsione
    setVectorWithOrientation(from, orientation, recStackPlus, recStackMinus, true);
    currentPath.segments.push_back(segments[from]);
    currentPath.orientations.push_back(orientation);

    // Se il nodo corrente è il nodo di destinazione, aggiunge il percorso alla lista
    if (from == to){
        paths.push_back(currentPath);
    }
    // Se il percorso attuale non ha superato la lunghezza massima, continua la ricerca
    else if (currentPath.segments.size() < maxLen) {
        // Itera su tutti i link in uscita dal nodo corrente
        for (Link link: links[from]) {
            if (link.fromOrient == orientation) {
                // Ricorre sul nodo di destinazione del link
                findNPathsUtil(link.to, link.toOrient, to, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);
            }
        }
    }
    // Backtracking: rimuove il nodo corrente dal percorso e dallo stack di ricorsione
    currentPath.segments.pop_back();
    currentPath.orientations.pop_back();
    setVectorWithOrientation(from, orientation, recStackPlus, recStackMinus, false);
}

// Metodo per trovare n percorsi tra due segmenti dati i loro nomi
vector<Path> PangenomeGraph::findNPaths(string from, string to, int n) {
    int maxLen = INT_MAX; // Lunghezza massima del percorso
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];

    vector<Path> paths;          // Lista dei percorsi trovati
    Path currentPath;            // Percorso corrente durante la ricerca
    vector<bool> recStackPlus(segments.size(), false);  // Stack di ricorsione per orientazione '+'
    vector<bool> recStackMinus(segments.size(), false); // Stack di ricorsione per orientazione '-'

    // Avvia la ricerca dei percorsi con entrambe le orientazioni
    findNPathsUtil(fromId, '+', toId, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);
    findNPathsUtil(fromId, '-', toId, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);

    return paths;
}
//--------------------------------------------------------------------------------------------------------------------------------
// --- Implementazione dell'algoritmo di Dijkstra per trovare il percorso più breve **** PER DILETTO**** ---

// Struttura per contenere una terna di valori (nodo, orientazione, distanza)
struct Triple {
    int first;      // ID del nodo
    char second;    // Orientazione del nodo ('+' o '-')
    int third;      // Distanza dal nodo sorgente

    Triple(int f, char s, int t) : first(f), second(s), third(t) {}
};

struct CompareTriple {
    bool operator()(const Triple& a, const Triple& b) {
        return a.third > b.third;
    }
};

// Metodo per trovare il percorso più breve tra due segmenti utilizzando Dijkstra
Path PangenomeGraph::dijkstra(string from, char fromOrient, string to, char toOrient) {
    // Inizializzazione delle strutture dati per Dijkstra
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];
    vector<int> distPlus(segments.size(), INT_MAX - 2);   // Distanze per orientazione '+'
    vector<int> distMinus(segments.size(), INT_MAX - 2);  // Distanze per orientazione '-'
    vector<int> prevPlus(segments.size(), -1);            // Predecessori per orientazione '+'
    vector<char> prevPlusOrient(segments.size(), ' ');    // Orientazioni dei predecessori '+'
    vector<int> prevMinus(segments.size(), -1);           // Predecessori per orientazione '-'
    vector<char> prevMinusOrient(segments.size(), ' ');   // Orientazioni dei predecessori '-'
    
    // Imposta la distanza iniziale del nodo sorgente
    if (fromOrient == '+') {
        distPlus[fromId] = 0;
    }
    else {
        distMinus[fromId] = 0;
    } 

    // Crea una coda di priorità per gestire i nodi da visitare
    priority_queue<Triple, vector<Triple>, CompareTriple> pq;
    pq.push(Triple(fromId, fromOrient, 0));

    // Inserisce tutti i nodi nella coda di priorità con le loro distanze iniziali
    for (Segment segment: segments){
        pq.push(Triple(segment.id, '+', distPlus[segment.id]));
        pq.push(Triple(segment.id, '-', distMinus[segment.id]));
    }

    // Loop principale dell'algoritmo di Dijkstra
    while (!pq.empty()){
        int nodeId = pq.top().first;
        char orient = pq.top().second;
        int dist = pq.top().third;
        pq.pop();

        // Itera su tutti i link in uscita dal nodo corrente
        for(Link link : links[nodeId]){
            if (link.fromOrient == orient ){
                // Calcola la distanza attraverso questo link e aggiorna se minore
                if (link.toOrient == '+'){
                    if (distPlus[link.to] > dist + 1){
                        distPlus[link.to] = dist + 1;
                        prevPlus[link.to] = nodeId;
                        prevPlusOrient[link.to] = orient;
                        pq.push(Triple(link.to, '+', dist + 1));
                    }
                }
                else{
                    if (distMinus[link.to] > dist + 1){
                        distMinus[link.to] = dist + 1;
                        prevMinus[link.to] = nodeId;
                        prevMinusOrient[link.to] = orient;
                        pq.push(Triple(link.to, '-', dist + 1));
                    }
                }
            }
        }
    }

    // Ricostruzione del percorso più breve dal nodo di destinazione al nodo sorgente
    Path path;
    path.from = from;
    path.to = to;
    vector<Segment> segs;
    vector<char> orients;
    segs.push_back(segments[segmentIndex[to]]);
    orients.push_back(toOrient);

    Segment currentNode = segments[segmentIndex[to]];
    char currentOrient = toOrient;
    Segment prevNode;
    char prevOrient;

    // Risale i predecessori per ricostruire il percorso
    while (currentNode.id != fromId && currentNode.id != -1){
        if (currentOrient == '+'){
            prevNode = segments[prevPlus[currentNode.id]];
            prevOrient = prevPlusOrient[currentNode.id];
            segs.push_back(prevNode);
            orients.push_back(prevOrient);
            currentNode = prevNode;
            currentOrient = prevOrient;
        }
        else{
            prevNode = segments[prevMinus[currentNode.id]];
            prevOrient = prevMinusOrient[currentNode.id];
            segs.push_back(prevNode);
            orients.push_back(prevOrient);
            currentNode = prevNode;
            currentOrient = prevOrient;
        }
    }

    // Inverte il percorso per avere l'ordine corretto dal nodo sorgente al nodo di destinazione
    reverse(segs.begin(), segs.end());
    reverse(orients.begin(), orients.end());
    path.segments = segs;
    path.orientations = orients;
    return path;
}
//-------------------------------------------------------------------------------------------------------------------------------- 
