/*
Sistemas Operacionais - Atividade 4 - Gerência de Memória
Aluno: Luiz Ricardo Brumati De Lima - 2155184
---------------------------------------------------------
> Desafio: Encontrar o número de Page Faults com os algoritmos de
substituição de páginas: FIFO, LRU e Ótimo.

> Extra: Exibir as etapas de paginação para cada algoritmo.
---------------------------------------------------------
-> Unordered Sets auxiliares foram utilizados para permitir
a rápida verificação de existência de um elemento na memória,
visto que sua complexidade de tempo é O(1) e, no pior caso, O(n).
---------------------------------------------------------
Entrada por linha de comando:
./main.exe <framesize> <input>

```
g++ main.cpp -o main.exe
./main.exe 4 70120304230321201701
```
--------------------------------------------------------- */

#include <string.h>

#include <cstdlib>
#include <iostream>
#include <list>
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;
#define YELLOW_TEXT "\033[1;33m"
#define DEFAULT_TEXT "\033[0m"

bool isElementInMemory(char element, unordered_set<char> set) {
    return set.find(element) != set.end();
}

/* FIFO - First In First Out */
pair<int, int> runFIFO(int framesize, char* input) {
    int hits = 0, pageFaults = 0;
    unordered_set<char> set;
    queue<char> indexes;

    for (int it = 0; it < strlen(input); it++) {
        if (!isElementInMemory(input[it], set)) {
            if (set.size() < framesize) {
                set.insert(input[it]);
                indexes.push(input[it]);
            } else {
                set.erase(indexes.front());
                indexes.pop();
                set.insert(input[it]);
                indexes.push(input[it]);
            }
            pageFaults++;
        } else {
            hits++;
        }
    }
    return make_pair(hits, pageFaults);
}

/* LRU - Least Recently Used */
pair<int, int> runLRU(int framesize, char* input) {
    int hits = 0, pageFaults = 0;
    unordered_set<char> set;
    list<char> indexes;

    for (int it = 0; it < strlen(input); it++) {
        if (!isElementInMemory(input[it], set)) {
            if (set.size() < framesize) {
                set.insert(input[it]);
                indexes.push_front(input[it]);
            } else {
                set.erase(indexes.back());
                indexes.pop_back();
                set.insert(input[it]);
                indexes.push_front(input[it]);
            }
            pageFaults++;
        } else {
            hits++;
            indexes.remove(input[it]);
            indexes.push_front(input[it]);
        }
    }

    return make_pair(hits, pageFaults);
}

int optimalPredict(char* input, unordered_set<char> set, int index) {
    int result = -1;
    int farthest = index;
    unordered_set<char>::iterator it;
    int it2;

    for (it = set.begin(); it != set.end(); it++) {
        for (it2 = index; it2 < strlen(input); it2++) {
            if (*it == input[it2]) {
                if (it2 > farthest) {
                    farthest = it2;
                    result = *it;
                }
                break;
            }
        }
        if (it2 == strlen(input)) return *it;
    }

    return result == -1 ? 0 : result;
}

/* Optimal */
pair<int, int> runOptimal(int framesize, char* input) {
    int hits = 0, pageFaults = 0;
    unordered_set<char> set;
    list<char> indexes;

    for (int it = 0; it < strlen(input); it++) {
        if (!isElementInMemory(input[it], set)) {
            if (set.size() < framesize) {
                set.insert(input[it]);
                indexes.push_front(input[it]);
                pageFaults++;
            } else {
                char predicted = optimalPredict(input, set, it + 1);
                set.erase(predicted);
                set.insert(input[it]);
                indexes.remove(predicted);
                indexes.push_front(input[it]);
                pageFaults++;
            }
        } else {
            hits++;
        }
    }
    return make_pair(hits, pageFaults);
}

int main(int argc, char* argv[]) {
    int framesize;
    char* input;

    if (argc == 3) {
        framesize = atoi(argv[1]);
        input = argv[2];
        cout << "=====================" << endl;
        cout << "Framesize: " << framesize << endl;
        cout << "Input: " << input << endl;
        cout << "=====================" << endl;

        cout << YELLOW_TEXT << "FIFO: " << DEFAULT_TEXT
             << runFIFO(framesize, input).first << " hits e "
             << runFIFO(framesize, input).second << " faults" << endl;

        cout << YELLOW_TEXT << "LRU: " << DEFAULT_TEXT
             << runLRU(framesize, input).first << " hits e "
             << runLRU(framesize, input).second << " faults" << endl;

        cout << YELLOW_TEXT << "OPTIMAL: " << DEFAULT_TEXT
             << runOptimal(framesize, input).first << " hits e "
             << runOptimal(framesize, input).second << " faults" << endl;

    } else {
        cout << "Uso: ./main.exe <framesize> <input>" << endl;
    }

    return 0;
}
