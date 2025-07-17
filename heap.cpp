#include <bits/stdc++.h>
using namespace std;

template <class T>
class Heap {
    vector<T> data;
    unordered_map<T, int> indexMap;

    int isLarger(const T& a, const T& b) {
        return a > b; // max-heap
    }

    void swapElements(int i, int j) {
        swap(data[i], data[j]);
        indexMap[data[i]] = i;
        indexMap[data[j]] = j;
    }

    void upheapify(int ci) {
        int pi = (ci - 1) / 2;
        if (ci > 0 && isLarger(data[ci], data[pi])) {
            swapElements(ci, pi);
            upheapify(pi);
        }
    }

    void downheapify(int pi) {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int maxi = pi;

        if (lci < data.size() && isLarger(data[lci], data[maxi])) {
            maxi = lci;
        }
        if (rci < data.size() && isLarger(data[rci], data[maxi])) {
            maxi = rci;
        }

        if (maxi != pi) {
            swapElements(pi, maxi);
            downheapify(maxi);
        }
    }

public:
    void add(T item) {
        data.push_back(item);
        indexMap[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    T remove() {
        swapElements(0, data.size() - 1);
        T removed = data.back();
        data.pop_back();
        indexMap.erase(removed);
        if (!data.empty()) downheapify(0);
        return removed;
    }

    T get() {
        return data[0];
    }

    bool isEmpty() {
        return data.empty();
    }

    int size() {
        return data.size();
    }

    void display() {
        for (const auto& item : data) {
            cout << item << " ";
        }
        cout << endl;
    }

    void updatePriority(T item) {
        if (indexMap.find(item) != indexMap.end()) {
            int idx = indexMap[item];
            upheapify(idx);
        }
    }
};
