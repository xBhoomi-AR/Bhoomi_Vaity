#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;


struct Task {
    string id;
    int remaining;
    vector<string> mem;
    int memIndex;

    Task(string i, int b, vector<string> m)
        : id(i), remaining(b), mem(m), memIndex(0) {}
};


class Cache {
public:
    int L1_MAX = 32, L2_MAX = 128, L3_MAX = 512;

    deque<string> L1, L2, L3;
    unordered_set<string> s1, s2, s3;

    int ramAccesses = 0;

    void erase(deque<string>& q, unordered_set<string>& s, string x) {
        s.erase(x);
        deque<string> temp;
        for (auto &i : q)
            if (i != x) temp.push_back(i);
        q = temp;
    }

    void insertL1(string x) {
        if (s1.count(x)) return;

        if (L1.size() == L1_MAX) {
            string ev = L1.front();
            L1.pop_front();
            s1.erase(ev);
            insertL2(ev);
        }

        L1.push_back(x);
        s1.insert(x);
    }

    void insertL2(string x) {
        if (s2.count(x)) return;

        if (L2.size() == L2_MAX) {
            string ev = L2.front();
            L2.pop_front();
            s2.erase(ev);
            insertL3(ev);
        }

        L2.push_back(x);
        s2.insert(x);
    }

    void insertL3(string x) {
        if (s3.count(x)) return;

        if (L3.size() == L3_MAX) {
            string ev = L3.front();
            L3.pop_front();
            s3.erase(ev);
        }

        L3.push_back(x);
        s3.insert(x);
    }

    int access(string x) {
        if (s1.count(x)) return 4;

        if (s2.count(x)) {
            erase(L2, s2, x);
            insertL1(x);
            return 12;
        }

        if (s3.count(x)) {
            erase(L3, s3, x);
            insertL1(x);
            return 40;
        }

        ramAccesses++;
        insertL1(x);
        return 200;
    }
};


int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    
    vector<Task*> allTasks;
    string line;

    while (getline(cin, line)) {

        if (line.empty()) continue;
        if (line.find("TASK") != 0) continue;

        stringstream ss(line);

        string taskWord, id, burstWord, memWord;
        int burst;

        ss >> taskWord >> id >> burstWord >> burst >> memWord;

        vector<string> mem;
        string m;

        while (ss >> m) {
            mem.push_back(m);
        }

        if (mem.empty()) mem.push_back("M0");

        allTasks.push_back(new Task(id, burst, mem));
    }


    Cache cache;

    int CORES = 3;
    int quantum = 3;

    vector<Task*> running(CORES, nullptr);
    queue<Task*> ready;

    int nextArrival = 0;
    int cycle = 0;

    
    while (nextArrival < allTasks.size() ||
           !ready.empty() ||
           any_of(running.begin(), running.end(),
                  [](Task* t){ return t != nullptr; })) {

        cycle++;

        cout << "\n====================\n";
        cout << "CYCLE " << cycle << "\n";

        
        if (nextArrival < allTasks.size()) {
            ready.push(allTasks[nextArrival]);
            cout << "TASK ARRIVED: " << allTasks[nextArrival]->id << "\n";
            nextArrival++;
        }

        
        for (int i = 0; i < CORES; i++) {
            if (!running[i] && !ready.empty()) {
                running[i] = ready.front();
                ready.pop();
            }
        }


        for (int i = 0; i < CORES; i++) {

            if (!running[i]) continue;

            Task* t = running[i];

            int slice = 0;

            while (slice < quantum && t->remaining > 0) {

                string mem;

                if (t->mem.empty()) {
                    mem = "M0";
                } else {
                    mem = t->mem[t->memIndex % t->mem.size()];
                }

                t->memIndex++;

                cout << "Core " << i
                     << " -> Task " << t->id
                     << " | MEM: " << mem << "\n";

                int latency = cache.access(mem);

                if (latency == 4) cout << "L1 HIT\n";
                else if (latency == 12) cout << "L2 HIT\n";
                else if (latency == 40) cout << "L3 HIT\n";
                else cout << "RAM MISS\n";

                t->remaining--;
                slice++;

                if (t->remaining <= 0) {
                    cout << t->id << " COMPLETED\n";
                    break;
                }
            }

            
            running[i] = nullptr;

            if (t->remaining > 0) {
                ready.push(t);   // preempted
            }
        }
    }

    cout << "Total Cycles: " << cycle << "\n";
    cout << "Total tasks loaded = " << allTasks.size() << "\n";
    cout << "Tasks Completed: " << allTasks.size() << "\n";
    cout << "Scheduler: Round Robin (quantum = " << quantum << ")\n";
    cout << "RAM Accesses: " << cache.ramAccesses << "\n";
    cout << "Cores: " << CORES << "\n";

    return 0;
}