#include "bits/stdc++.h"

using namespace std;

void printVector(vector<int> vec) {
    for (auto &v: vec) {
        cout << v << " ";
    }
    cout << endl;
}

void printMatrix(vector< vector<int> > mat) {
    for (auto &m: mat) {
        printVector(m);
    }
    cout << endl;
}

void appendRows(vector< vector<int> > &m1, vector< vector<int> > &m2) {
    for (int i=0; i<m1.size(); i++) {
        m2.push_back(m1[i]);
    }
}

void appendVal2RowFront(vector< vector<int> > &m, int &val) {
    // adding a value to the beginning of each row.
    for (int i=0; i<m.size(); i++) {
        m[i].insert(m[i].begin(), val);
    }
}

vector< vector<int> > pSequences2(int n, int p, int prev_num) {
    // given the previous number
    // generate all possible sequences
    vector< vector<int> > out;
    vector< vector<int> > holder;
    vector<int> temp;
    if (n == 1) {
        // if its the last number then output all values that make the limit acceptable
        for (int i_p=1; i_p<=p; i_p++) {
            if (prev_num * i_p <= p) {
                temp.clear();
                temp.push_back(i_p);
                out.push_back(temp);
            }
        }
    }
    
    else {
        // otherwise, figure out what value are compatible with previous number
        // generate sequences that are compatible
        for (int i_p=1; i_p<=p; i_p++) {
            if (prev_num * i_p <= p) {
                holder.clear();
                holder = pSequences2(n-1, p, i_p);
                if (prev_num != -1) {
                    appendVal2RowFront(holder, i_p);
                }
                appendRows(holder, out);
            }
        }
    }
    return out;
}

/*
 * Complete the pSequences function below.
 */
int pSequences(int n, int p) {
    /*
     * Write your code here.
     */
    vector< vector<int> > out;
    vector< vector<int> > holder;
    vector<int> temp;
    
    out = pSequences2(n, p, 0);
    // for (int i_n=1; i_n<n; i_n++) {
    //     holder = pSequences2(n-1, p, i_n);
    //     if (holder.size() > 0) {
    //         appendRows(holder, out);
    //     }
    //     // remove all arrays that are too short
    //     for (int i=0; i<out.size();) {
    //         if (out[i].size() < n) {
    //             out.erase(out.begin() + i);
    //         }
    //         else {
    //             i++;
    //         }
    //     }
    // }
    
    
    printMatrix(out);
    return out.size();

}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int p;
    cin >> p;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    printf("N: %d, P: %d\n", n, p);

    // n = 3;
    // p = 3;

    int result = pSequences(n, p);
    printf("Result: %d\n", result);
    fout << result << "\n";

    fout.close();

    return 0;
}
