#include <bits/stdc++.h>

using namespace std;

void fifoPageReplacement()
{
    int numFrames, length;

    cout << "Enter number of page frames (e.g., 3): ";
    cin >> numFrames;

    cout << "Enter the length of reference string: ";
    cin >> length;

    cout << "Enter the " << length
         << " reference page requests (space-separated or contiguous): ";

    string firstInput;
    cin >> firstInput;

    vector<string> refStr;

    // Check whether input is space-separated or contiguous
    if (cin.peek() == ' ')
    {
        // First input and remaining inputs are separate pages
        refStr.push_back(firstInput);

        for (int i = 1; i < length; i++)
        {
            string page;
            cin >> page;
            refStr.push_back(page);
        }
    }
    else
    {
        // Contiguous input such as 701203042303
        for (char c : firstInput)
        {
            refStr.push_back(string(1, c));
        }
    }

    // Truncate if more than the specified length
    if ((int)refStr.size() > length)
        refStr.resize(length);

    vector<string> frames(numFrames, " ");
    vector<string> fifoQueue;

    vector<vector<string>> frameHistory(
        numFrames, vector<string>());

    vector<string> statusList;

    int faults = 0;
    int hits = 0;

    // FIFO Page Replacement
    for (string page : refStr)
    {
        bool found = false;

        // Check if page is already in frame
        for (string frame : frames)
        {
            if (frame == page)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            // Hit
            hits++;
            statusList.push_back("H");
        }
        else
        {
            // Page Fault
            faults++;
            statusList.push_back("P");

            // Check for empty frame
            int emptyIndex = -1;

            for (int i = 0; i < numFrames; i++)
            {
                if (frames[i] == " ")
                {
                    emptyIndex = i;
                    break;
                }
            }

            if (emptyIndex != -1)
            {
                // Fill first empty frame
                frames[emptyIndex] = page;
                fifoQueue.push_back(page);
            }
            else
            {
                // Replace oldest page
                string oldestPage = fifoQueue.front();
                fifoQueue.erase(fifoQueue.begin());

                for (int i = 0; i < numFrames; i++)
                {
                    if (frames[i] == oldestPage)
                    {
                        frames[i] = page;
                        break;
                    }
                }

                fifoQueue.push_back(page);
            }
        }

        // Save current frame state
        for (int i = 0; i < numFrames; i++)
        {
            frameHistory[i].push_back(frames[i]);
        }
    }

    // Display Execution Table
    int colW = 4;
    int headerW = 12;

    cout << "\n"
         << string(headerW + refStr.size() * colW, '=')
         << endl;

    cout << "FIFO Page Replacement Execution (Requests 1-"
         << refStr.size() << ")" << endl;

    cout << string(headerW + refStr.size() * colW, '=')
         << endl;

    // Request Row
    cout << left << setw(headerW) << "Request #";

    for (int i = 0; i < (int)refStr.size(); i++)
    {
        cout << right << setw(colW) << i + 1;
    }

    cout << endl;

    // Page Reference Row
    cout << left << setw(headerW) << "Page Ref";

    for (string page : refStr)
    {
        cout << right << setw(colW) << page;
    }

    cout << endl;

    cout << string(headerW + refStr.size() * colW, '-')
         << endl;

    // Frame Rows
    for (int i = 0; i < numFrames; i++)
    {
        cout << left << setw(headerW)
             << ("Frame " + to_string(i + 1));

        for (string value : frameHistory[i])
        {
            cout << right << setw(colW) << value;
        }

        cout << endl;
    }

    cout << string(headerW + refStr.size() * colW, '-')
         << endl;

    // Status Row
    cout << left << setw(headerW) << "Status";

    for (string status : statusList)
    {
        cout << right << setw(colW) << status;
    }

    cout << endl;

    cout << string(headerW + refStr.size() * colW, '=')
         << endl;

    // Metrics
    double faultRate = 0;
    double hitRate = 0;

    if (length > 0)
    {
        faultRate = (double)faults / length * 100;
        hitRate = (double)hits / length * 100;
    }

    cout << "\nFull String Metrics" << endl;

    cout << "Total Requests: " << length << endl;

    cout << "Total Page Faults (P): "
         << faults << endl;

    cout << "Total Hits (H): "
         << hits << endl;

    cout << fixed << setprecision(2);

    cout << "Page Fault Rate: "
         << faults << "/" << length
         << " * 100 = " << faultRate << "%" << endl;

    cout << "Hit Rate: "
         << hits << "/" << length
         << " * 100 = " << hitRate << "%" << endl;
}

int main()
{
    fifoPageReplacement();

    return 0;
}
