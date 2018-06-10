
#include <algorithm>
#include <vector>
#include <ios>
#include <iomanip>
#include <iostream>

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::sort;

int solution(vector<int> &A) {
    // given an array A of N integers, returns the smallest positive integer (greater than 0) that does not occur in A
    int min_value = 1; // smallest acceptable value
    int max_value = 1000000; // largest acceptable value
    // check if array is empty
    if ((int)A.size() == 0){
        return min_value;
    };
    sort(A.begin(), A.end());
    int sol = min_value;

    // search through list from smallest to largets
    for (int i=0; i!=(int)A.size(); i++){
        // cout << i << ":" << A[i] << " ";

        if (A[i] < min_value || A[i] > max_value) {
            // skip values smaller than smallest possible value or greater than largest possible value
            continue;
        } else if (A[i] == sol-1) {
            // skip if the same value as last iteration
            continue;
        }
        else if (A[i] > sol) {
            // if current value is greater than current solution, then solution found
            break;
        }
        else if (A[i] == sol) {
            // if current value is equal to current solution, increment solution
            sol = sol + 1;
        } else {
            cout << "Shouldn't be here: " << A[i] << ", Current Solution: " << sol << endl;
        }
    };
    // if has all positive numbers then choose one larger than largest
    if (sol < min_value) {
        sol = A.back() + 1;
    };
    // if only negative numbers, make solution minimum value
    if (sol < min_value) {
        sol = min_value;
    } else if (sol > max_value) {
        // if only huge numbers, make solution maximum value
        sol = max_value;
    }
    return sol;
}
