
#include <algorithm>
#include <vector>
#include <ios>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <valarray>

// #include "Problem2.h"

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;


vector< vector<int> > checkNeighbors(vector< vector<int> > &A, vector<int> node, int rows, int cols) {
    // checks the neighbors of a node and returns which ones are part of the country
    // node is the row and col location in A
    vector< vector<int> > neighbor_locs;
    vector<int> temp_neigh;
    cout << "Node: " << node[0] << "," << node[1] << " | " << A[node[0]][node[1]] << endl;
    for (int m = -1; m<2; m++) {
        for (int n = -1; n<2; n++) {
            // disregard staying in place and moving diagonally
            if (m == 0 && n ==0){
                continue;
            } else if (m == 1 && n == 1) {
                continue;
            } else if (m == 1 && n == -1) {
                continue;
            } else if (m == -1 && n == 1) {
                continue;
            } else if (m == -1 && n == -1) {
                continue;
            }
            // 4 possible location movements
            temp_neigh.push_back(node[0]+m);
            temp_neigh.push_back(node[1]+n);
            // if its outside the range, keep going
            
            if (temp_neigh[0] < 0 || temp_neigh[1] < 0 || temp_neigh[0] > rows || temp_neigh[1] > cols) {
                // cout << "Disregard: " << temp_neigh[0] << "," << temp_neigh[1] << " | ";
                
            } else if(A[temp_neigh[0]][temp_neigh[1]] != A[node[0]][node[1]]) {
                // if they are not equal, not part of the same country
                // cout << "Not Neighbor Value: " << A[temp_neigh[0]][temp_neigh[1]] << endl;

            } else {
                // cout << temp_neigh[0] << "," << temp_neigh[1] << " Value: " << A[temp_neigh[0]][temp_neigh[1]] << endl;
                neighbor_locs.push_back(temp_neigh);
            }
            temp_neigh.clear();
        }
        if (A[node[0]][node[1]] == 5) {
                cout << "HERE" << endl;
            }
    }
    return neighbor_locs;
};




int solution(vector< vector<int> > &A) {
    // write your code in C++14 (g++ 6.2.0)



    // for each node

        // check if it is already part of a country
            // continue

        // if not explore around that node
            // add nodes part of the same country to that country list
            // increase country count





    vector< vector<int> > explored_nodes; // all the locations that have been visited
    vector< vector<int> > country_lists; // all nodes associated with a country

    int rows = A.size();
    int cols = A[0].size();

    cout << "Rows: " << rows << " Cols: " << cols << endl;
    for (int i1=0; i1<rows; i1++) {
        for (int i2=0; i2<cols; i2++) {
            // pick a node

            vector< vector<int> > valid_neighbors; // neighbors that are in the same country
            vector<int> node;
            node.push_back(i1);
            node.push_back(i2);


            valid_neighbors = checkNeighbors(A, node, rows, cols);
            cout << "HERE" << endl;
            // cout << "R: " << i1 << ", C: " << i2 << " ";
            // cout << "Neighbors : ";
            // for (int n = 0; n < valid_neighbors.size(); n++) {
            //     cout << valid_neighbors[n][0] << "," << valid_neighbors[n][1] << "; ";
            // }
            // cout << endl;
            valid_neighbors.clear();
            node.clear();
        }
            cout << "HERE" << endl;
    }


    return -1;
}