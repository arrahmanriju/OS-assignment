//Name:Azizur Rahman
//ID : 2023000011009

/*
input :

 */

#include <bits/stdc++.h>
using namespace std;

void runSimulation() {

    // ---------------- 1. Inputs ----------------

    cout << "=== Memory Allocation Simulator ===\n";

    int numBlocks;
    cout << "Enter Number of memory blocks: ";
    cin >> numBlocks;

    vector<int> initialBlocks(numBlocks);

    for (int i = 0; i < numBlocks; i++) {
        cout << "Size of memory block " << i + 1 << ": ";
        cin >> initialBlocks[i];
    }

    int numProcesses;
    cout << "\nEnter Number of processes: ";
    cin >> numProcesses;

    vector<int> processes(numProcesses);

    for (int i = 0; i < numProcesses; i++) {
        cout << "Size of process " << i + 1 << ": ";
        cin >> processes[i];
    }


    // ---------------- Helper Function ----------------

    struct SimulationResult {
        vector<vector<int>> history;
        vector<string> statuses;
        vector<int> finalBlocks;
    };


    auto simulate = [&](string algoType) {

        SimulationResult result;

        vector<int> blocks = initialBlocks;
        bool stopped = false;

        for (int p : processes) {

            if (stopped) {
                result.statuses.push_back("Deny");
                result.history.push_back(blocks);
                continue;
            }

            int chosenIdx = -1;

            // ---------- First Fit ----------
            if (algoType == "First Fit") {

                for (int i = 0; i < blocks.size(); i++) {

                    if (blocks[i] >= p) {
                        chosenIdx = i;
                        break;
                    }
                }
            }

            // ---------- Best Fit ----------
            else if (algoType == "Best Fit") {

                int bestDiff = INT_MAX;

                for (int i = 0; i < blocks.size(); i++) {

                    if (blocks[i] >= p &&
                        blocks[i] - p < bestDiff) {

                        bestDiff = blocks[i] - p;
                        chosenIdx = i;
                    }
                }
            }

            // ---------- Worst Fit ----------
            else if (algoType == "Worst Fit") {

                int worstDiff = -1;

                for (int i = 0; i < blocks.size(); i++) {

                    if (blocks[i] >= p &&
                        blocks[i] - p > worstDiff) {

                        worstDiff = blocks[i] - p;
                        chosenIdx = i;
                    }
                }
            }


            // ---------- Allocation ----------
            if (chosenIdx != -1) {

                blocks[chosenIdx] -= p;

                result.statuses.push_back("Allocated");
                result.history.push_back(blocks);
            }

            // ---------- Cannot Allocate ----------
            else {

                result.statuses.push_back("Deny");
                result.history.push_back(blocks);

                stopped = true;
            }
        }

        result.finalBlocks = blocks;

        return result;
    };


    // ---------------- Print Table ----------------

    auto printTraceTable =
        [&](string algoName,
            string desc,
            SimulationResult result) {

        cout << "\n" << algoName << ":\n";
        cout << desc << "\n\n";

        const int lblW = 8;
        const int colW = 7;

        cout << setw(lblW) << "" << " | ";

        for (int p : processes) {
            cout << setw(colW) << p << " | ";
        }

        cout << "\n";


        // Deny markers

        cout << setw(lblW) << "" << " | ";

        for (string status : result.statuses) {

            if (status == "Deny")
                cout << setw(colW) << "Deny" << " | ";
            else
                cout << setw(colW) << "" << " | ";
        }

        cout << "\n";


        cout << string(12 + numProcesses * 10, '-') << "\n";


        // Memory block rows

        for (int b = 0; b < initialBlocks.size(); b++) {

            cout << setw(lblW)
                 << initialBlocks[b]
                 << " | ";

            for (int p = 0; p < processes.size(); p++) {

                if (result.statuses[p] == "Deny") {

                    cout << setw(colW) << "" << " | ";
                }
                else {

                    cout << setw(colW)
                         << result.history[p][b]
                         << " | ";
                }
            }

            cout << "\n";
        }


        // ---------------- Fragmentation Check ----------------

        int freeMem = 0;

        for (int x : result.finalBlocks)
            freeMem += x;


        bool hasDeny = false;

        for (string status : result.statuses) {

            if (status == "Deny") {
                hasDeny = true;
                break;
            }
        }


        if (hasDeny) {

            int firstDeniedIdx = -1;

            for (int i = 0; i < result.statuses.size(); i++) {

                if (result.statuses[i] == "Deny") {
                    firstDeniedIdx = i;
                    break;
                }
            }

            int deniedProcessSize =
                processes[firstDeniedIdx];


            if (freeMem >= deniedProcessSize) {

                cout << "There is external fragmentation\n";

                cout << "Total number of Free memory / "
                        "External Fragmentation: "
                     << freeMem << " MB\n";
            }
            else {

                cout << "Not enough Space\n";

                cout << "Total number of Free memory: "
                     << freeMem << " MB\n";
            }
        }
        else {

            cout << "There is no external fragmentation\n";

            cout << "Total number of Free memory: "
                 << freeMem << " MB\n";
        }
    };


    // ---------------- 2. Run Simulations ----------------

    vector<pair<string, string>> algorithms = {

        {
            "First fit",
            "The process takes the memory block which is closer to them."
        },

        {
            "Best Fit",
            "In best fit a process takes the memory block which has the lowest difference."
        },

        {
            "Worst Fit",
            "The process takes the biggest block of them all"
        }
    };


    // Store results

    struct AlgorithmData {

        int allocatedCount;
        int totalAllocatedSize;
        int freeMem;
    };


    map<string, AlgorithmData> results;


    for (auto algorithm : algorithms) {

        string name = algorithm.first;
        string desc = algorithm.second;

        string key;

        if (name == "First fit")
            key = "First Fit";

        else if (name == "Best Fit")
            key = "Best Fit";

        else
            key = "Worst Fit";


        SimulationResult result =
            simulate(key);


        printTraceTable(
            name,
            desc,
            result
        );


        int allocatedCount = 0;
        int totalAllocatedSize = 0;


        for (int i = 0; i < result.statuses.size(); i++) {

            if (result.statuses[i] == "Allocated") {

                allocatedCount++;

                totalAllocatedSize += processes[i];
            }
        }


        int freeMem = 0;

        for (int x : result.finalBlocks)
            freeMem += x;


        results[name] = {
            allocatedCount,
            totalAllocatedSize,
            freeMem
        };
    }


    // ---------------- 3. Comparison Table ----------------

    cout << "\n"
         << string(68, '=')
         << "\n";

    cout << setw(48)
         << "COMPARISON TABLE"
         << "\n";

    cout << string(68, '=')
         << "\n";


    cout << left
         << setw(15) << "Algorithm"
         << " | "
         << setw(18) << "Allocated Requests"
         << " | "
         << setw(16) << "Total Memory Used"
         << " | "
         << setw(12) << "Free Memory"
         << "\n";


    cout << string(68, '-') << "\n";


    for (auto &item : results) {

        string name = item.first;
        AlgorithmData data = item.second;


        string allocStr =
            to_string(data.allocatedCount)
            + "/" +
            to_string(numProcesses);

        string usedStr =
            to_string(data.totalAllocatedSize)
            + " MB";

        string freeStr =
            to_string(data.freeMem)
            + " MB";


        cout << left
             << setw(15) << name
             << " | "
             << setw(18) << allocStr
             << " | "
             << setw(16) << usedStr
             << " | "
             << setw(12) << freeStr
             << "\n";
    }


    cout << string(68, '-') << "\n";


    // ---------------- 4 & 5. Best Algorithm ----------------

    string bestAlgo;
    int bestAllocated = -1;
    int bestMemoryUsed = -1;


    for (auto &item : results) {

        string name = item.first;
        AlgorithmData data = item.second;


        if (data.allocatedCount > bestAllocated ||
            (data.allocatedCount == bestAllocated &&
             data.totalAllocatedSize > bestMemoryUsed)) {

            bestAllocated = data.allocatedCount;
            bestMemoryUsed = data.totalAllocatedSize;

            bestAlgo = name;
        }
    }


    cout << "\nBest algorithm for that input: "
         << bestAlgo << "\n";
}


// ---------------- Main ----------------

int main() {

    runSimulation();

    return 0;
}
