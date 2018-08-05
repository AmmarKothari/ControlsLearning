#include "bits/stdc++.h"

using namespace std;

// struct Params
// {
//     int params[3];

//     bool operator <(const Params &rhs) {
//         if (this->params[0] != rhs.params[0]) {
//             return (this->params[0] < rhs.params[0]);
//         }
//         else if (this ->params[1] != rhs.params[1]) {
//             return (this->params[1] < rhs.params[1]);
//         }
//         else if (this ->params[2] != rhs.params[2]) {
//             return (this->params[2] < rhs.params[2]);
//         }
//         else {
//             cout << "Identical" << endl;
//             return true;
//         }
//     }
// };


// int pSequences2(int n, int p, int prev_num, map<Params, int> &seq_map) {
//     int count = 0;
    
//     // check map if this challenge has already been solved
//     Params param_arr = {n, p, prev_num};
//     // cout << "Input Array: " << endl;
//     // for (auto i: input_arr) {
//     //     cout << i << " ";
//     // }
//     // cout << endl;
//     map<Params, int>::iterator it;
//     it = seq_map.find(param_arr);
//     if (it != seq_map.end()) {
//         // if it has, pull out that value
//         count += it->second;
//     }
//     else {
//         // not in map, so figure out what it should be
//         if (n == 1) {
//             // if its the last number
//             // all subsequent numbers are those that are less than p/prev
//             count += (p/prev_num);
//         }
//         else {
//             for (int i_p=1; i_p<=p; i_p++) {
//                 if (prev_num*i_p<=p) {
//                     count += pSequences2(n-1, p, i_p, seq_map);
//                 }
//                 else {
//                     break;
//                 }
//             }
//         }
//         cout << "Map Size: " << seq_map.size() << endl;
//         // seq_map[input_arr] = count;
//         seq_map.insert(make_pair(param_arr, count));
//     }
//     return count;
// }

int pSequences2(int n, int p, int prev_num, vector< vector< vector<int> > > &seq_mat) {

    return 0;
}


int pSequences(int n, int p) {
    int count = 0;
    // map<Params, int> seq_map;
    vector< vector< vector<int> > > Params; // array to hold all values; index is params and value is number in sequence;
    // generate all possible first values of sequence
    // count = pSequences2(n, p, 0, seq_map);
    count = pSequences2(n, p, 0, Params);
    return count;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));
    vector< vector<int> > seqs;

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int p;
    cin >> p;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    int result = pSequences(n, p);
    printf("Results: %d\n", result);
    
    fout << result << "\n";

    fout.close();

    return 0;
}