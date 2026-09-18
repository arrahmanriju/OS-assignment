//Name:Azizur Rahman
//ID : 2023000011009

#include <bits/stdc++.h>
using namespace std;

struct Process {
    string name;
    int arrival, burst;
    int start, finish, turnaround, waiting, response;
};

// One box in the Gantt chart (a process, or "Idle")
struct Block {
    string name;
    int start, end;
};

struct Averages {
    double tat, wt, rt;
};

// ---------------------------------------------------------
// ONE function for both FCFS and SJF
// The only difference is HOW we pick the next process:
//   FCFS -> smallest arrival time
//   SJF  -> smallest burst time (tie: earlier arrival)
// ---------------------------------------------------------
void schedule(vector<Process> p, bool isSJF,vector<Process>& done, vector<Block>& gantt) {

    int n = p.size();
    vector<bool> used(n, false);
    int time = 0;

    while ((int)done.size() < n) {

        // 1) Find the best process that has already arrived
        int pick = -1;
        for (int i = 0; i < n; i++) {
            if (used[i] || p[i].arrival > time) continue;

            if (pick == -1) {
                pick = i;
                continue;
            }

            // Compare using a simple "key"
            pair<int,int> keyI    = { isSJF ? p[i].burst    : p[i].arrival,    p[i].arrival };
            pair<int,int> keyPick = { isSJF ? p[pick].burst : p[pick].arrival, p[pick].arrival };

            if (keyI < keyPick) pick = i;
        }

        // 2) Nobody has arrived yet -> CPU is idle until next arrival
        if (pick == -1) {
            int next = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!used[i]) next = min(next, p[i].arrival);

            gantt.push_back({"Idle", time, next});
            time = next;
            continue;
        }

        // 3) Run the chosen process
        Process& x = p[pick];
        x.start      = time;
        x.finish     = x.start + x.burst;
        x.turnaround = x.finish - x.arrival;
        x.waiting    = x.turnaround - x.burst;
        x.response   = x.start - x.arrival;

        gantt.push_back({x.name, x.start, x.finish});

        time = x.finish;
        used[pick] = true;
        done.push_back(x);
    }
}

// ---------------------------------------------------------
// Averages
// ---------------------------------------------------------
Averages getAverages(const vector<Process>& p) {
    Averages a = {0, 0, 0};
    for (const Process& x : p) {
        a.tat += x.turnaround;
        a.wt  += x.waiting;
        a.rt  += x.response;
    }
    a.tat /= p.size();
    a.wt  /= p.size();
    a.rt  /= p.size();
    return a;
}

// ---------------------------------------------------------
// Gantt Chart
// ---------------------------------------------------------
void printGantt(const vector<Block>& gantt) {

    // Box width: at least 8, or longer if a name is long
    int W = 8;
    for (const Block& b : gantt)
        W = max(W, (int)b.name.size() + 2);

    cout << "\nGantt Chart:\n\n";

    // Top border
    for (size_t i = 0; i < gantt.size(); i++)
        cout << "+" << string(W, '-');
    cout << "+\n";

    // Process names (centered)
    for (const Block& b : gantt) {
        int left  = (W - (int)b.name.size()) / 2;
        int right = W - (int)b.name.size() - left;
        cout << "|" << string(left, ' ') << b.name << string(right, ' ');
    }
    cout << "|\n";

    // Bottom border
    for (size_t i = 0; i < gantt.size(); i++)
        cout << "+" << string(W, '-');
    cout << "+\n";

    // Time labels under each box boundary
    for (const Block& b : gantt)
        cout << left << setw(W + 1) << b.start;
    cout << gantt.back().end << "\n";
}

// ---------------------------------------------------------
// Table + Gantt for one algorithm
// ---------------------------------------------------------
void printResult(const vector<Process>& p, const vector<Block>& gantt,const string& title) {

    cout << "\n\n===================== " << title << " =====================\n\n";

    cout << left
         << setw(12) << "Process"
         << setw(12) << "Arrival"
         << setw(12) << "CPU Time"
         << setw(12) << "Start"
         << setw(12) << "Finish"
         << setw(15) << "Turnaround"
         << setw(12) << "Waiting"
         << setw(12) << "Response" << endl;
    cout << string(99, '-') << endl;

    for (const Process& x : p) {
        cout << left
             << setw(12) << x.name
             << setw(12) << x.arrival
             << setw(12) << x.burst
             << setw(12) << x.start
             << setw(12) << x.finish
             << setw(15) << x.turnaround
             << setw(12) << x.waiting
             << setw(12) << x.response << endl;
    }

    Averages a = getAverages(p);
    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time : " << a.tat << endl;
    cout << "Average Waiting Time    : " << a.wt  << endl;
    cout << "Average Response Time   : " << a.rt  << endl;

    printGantt(gantt);
}

// ---------------------------------------------------------
// Compare FCFS and SJF
// ---------------------------------------------------------
string better(double fcfs, double sjf) {
    if (fcfs < sjf) return "FCFS";
    if (sjf < fcfs) return "SJF";
    return "Same";
}

void printRow(const string& label, double fcfs, double sjf) {
    cout << left
         << setw(25) << label
         << setw(15) << fcfs
         << setw(15) << sjf
         << better(fcfs, sjf) << endl;
}

void compareAlgorithms(const vector<Process>& fcfs, const vector<Process>& sjf) {

    Averages f = getAverages(fcfs);
    Averages s = getAverages(sjf);

    cout << "\n\n================ ALGORITHM COMPARISON ================\n\n";

    cout << left
         << setw(25) << "Measurement"
         << setw(15) << "FCFS"
         << setw(15) << "SJF"
         << setw(15) << "Better" << endl;
    cout << string(70, '-') << endl;

    printRow("Avg Waiting Time",    f.wt,  s.wt);
    printRow("Avg Turnaround Time", f.tat, s.tat);
    printRow("Avg Response Time",   f.rt,  s.rt);

    cout << "\n-------------------------------------------------------\n";

    if (s.wt < f.wt)      cout << "Overall Result: SJF has lower average waiting time.\n";
    else if (f.wt < s.wt) cout << "Overall Result: FCFS has lower average waiting time.\n";
    else                  cout << "Overall Result: Both have the same average waiting time.\n";
}

// ---------------------------------------------------------
// Main
// ---------------------------------------------------------
int main() {

    int n;

    cout << "============================================\n";
    cout << "      CPU PROCESS SCHEDULING SIMULATOR      \n";
    cout << "============================================\n";

    cout << "\nEnter number of processes: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of processes!" << endl;
        return 0;
    }

    vector<Process> processes(n);

    cout << "\nEnter Process Information\n";
    cout << "---------------------------\n";

    for (int i = 0; i < n; i++) {
        cout << "\nProcess " << i + 1 << endl;
        cout << "Enter Process Name : ";
        cin >> processes[i].name;
        cout << "Enter Arrival Time : ";
        cin >> processes[i].arrival;
        cout << "Enter CPU Time     : ";
        cin >> processes[i].burst;
    }

    // Run both algorithms (false = FCFS, true = SJF)
    vector<Process> fcfsDone, sjfDone;
    vector<Block> fcfsGantt, sjfGantt;

    schedule(processes, false, fcfsDone, fcfsGantt);
    schedule(processes, true,  sjfDone,  sjfGantt);

    printResult(fcfsDone, fcfsGantt, "FIRST COME FIRST SERVE (FCFS)");
    printResult(sjfDone,  sjfGantt,  "SHORTEST JOB FIRST (SJF)");

    compareAlgorithms(fcfsDone, sjfDone);

    return 0;
}