#include <bits/stdc++.h>

using namespace std;

int main()
{
    int n, item, Dl = 0, Dr = 0;
    cin >> n;
    for(int row = 0; row < n; row ++) {
        for(int col = 0; col < n; col ++) {
            cin >> item;
            if(row == col) Dl += item;
            if(row + col == n - 1) Dr += item;
        }
    }
    cout << abs(Dl - Dr);
    return 0;
}
