#include <bits/stdc++.h>
using namespace std;

struct Edge {
    string destination;
    double distance;
    string lineColor;
};

class Graph {
public:
    unordered_map<string, vector<Edge>> adj;

    void addEdge(string src, string dest, double dist, string color) {
        adj[src].push_back({dest, dist, color});
        adj[dest].push_back({src, dist, color}); // undirected
    }

    void displayStations() {
        int i = 1;
        for (auto &p : adj) {
            cout << i++ << ". " << p.first << "\n";
        }
    }

    void displayMap() {
        cout << "Delhi Metro Map:\n";
        for (auto &p : adj) {
            cout << p.first << " => ";
            for (auto &e : p.second) {
                cout << "(" << e.destination << ", " << e.distance << "km, " << e.lineColor << ") ";
            }
            cout << "\n";
        }
    }

    void getShortestPathWithLines(string src, string dest) {
        unordered_map<string, double> dist;
        unordered_map<string, pair<string, string>> prev; // node -> {parent, lineColor}
        set<pair<double, string>> pq;

        for (auto &p : adj)
            dist[p.first] = DBL_MAX;

        dist[src] = 0;
        pq.insert({0, src});

        while (!pq.empty()) {
            auto [d, u] = *pq.begin();
            pq.erase(pq.begin());

            if (u == dest) break;

            for (auto &e : adj[u]) {
                double newDist = d + e.distance;
                if (newDist < dist[e.destination]) {
                    pq.erase({dist[e.destination], e.destination});
                    dist[e.destination] = newDist;
                    prev[e.destination] = {u, e.lineColor};
                    pq.insert({newDist, e.destination});
                }
            }
        }

        if (dist[dest] == DBL_MAX) {
            cout << "No path found.\n";
            return;
        }

        // Reconstruct path with lines
        vector<tuple<string, string>> path; // {station, lineColor}
        string curr = dest;
        while (curr != src) {
            auto [parent, color] = prev[curr];
            path.push_back({curr, color});
            curr = parent;
        }
        path.push_back({src, ""});
        reverse(path.begin(), path.end());

        cout << "Shortest Distance: " << dist[dest] << " km\n";
        cout << "Path:\n";
        for (size_t i = 0; i < path.size() - 1; ++i) {
            cout << get<0>(path[i]) << " -> ";
            cout << get<0>(path[i + 1]) << " [" << get<1>(path[i + 1]) << "]\n";
        }
    }
};

vector<string> parseCSVLine(const string &line) {
    vector<string> result;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}

void loadMetroFromCSV(Graph &g, const string &filename) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found: " << filename << endl;
        return;
    }
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        vector<string> tokens = parseCSVLine(line);
        if (tokens.size() < 4) continue;
        string src = tokens[0];
        string dest = tokens[1];
        if (src.empty() || dest.empty()) {
            cerr << "Empty src or dest in line: " << line << endl;
            continue;
        }
        if (tokens[2].empty()) {
            cerr << "Empty distance value in line: " << line << endl;
            continue;
        }
        double dist = 0.0;
        try {
            dist = stod(tokens[2]);
        } catch (const std::invalid_argument&) {
            cerr << "Invalid distance value in line: " << line << endl;
            continue;
        } catch (const std::out_of_range&) {
            cerr << "Out of range distance value in line: " << line << endl;
            continue;
        }
        string color = tokens[3];
        g.addEdge(src, dest, dist, color);
    }
    file.close();
}

int main() {
    Graph metro;
    loadMetroFromCSV(metro, "processed_metro_edges_with_colors2.csv");

    cout << "\n**** WELCOME TO THE METRO APP ****\n";
    while (true) {
        cout << "\nMenu:\n"
             << "1. List all stations\n"
             << "2. Show metro map\n"
             << "3. Get shortest distance path with lines\n"
             << "4. Exit\n"
             << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice == 4) break;
        string src, dest;
        switch (choice) {
            case 1:
                metro.displayStations();
                break;
            case 2:
                metro.displayMap();
                break;
            case 3:
                cout << "Enter source station: ";
                getline(cin, src);
                cout << "Enter destination station: ";
                getline(cin, dest);
                metro.getShortestPathWithLines(src, dest);
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }
    }
}



