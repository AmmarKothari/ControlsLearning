#include <iostream>
#include <deque> 
using namespace std;

void printKMax(int arr[], int n, int k){
	//Write your code here.
    deque<int> dq;
    int temp = 0;
    
    for (int i=0; i<n; i++) {
        // fill deque with first part of array
        int new_val = arr[i];
        while (!dq.empty() && arr[dq.front()] < new_val) {
            // only keep values that are bigger than new value
            dq.pop_front();
        }
        dq.push_front(i);
        if (i < (k-1)) { // just fill the array to window size
            continue;
        }
        // cout << i+k << "   " << dq.back() << "   ";
        // while (dq.back() < i-1 && k <= (n-i)) {
        while (dq.back() < i-k+1) {
            // remove large values that are out of range
            dq.pop_back();
        }
        
        printf("%d ", arr[dq.back()]);
    }
    cout << endl;
}

int main(){
  
	int t;
	cin >> t;
	while(t>0) {
		int n,k;
    	cin >> n >> k;
    	int i;
    	int arr[n];
    	for(i=0;i<n;i++)
      		cin >> arr[i];
    	printKMax(arr, n, k);
    	t--;
  	}
  	return 0;
}