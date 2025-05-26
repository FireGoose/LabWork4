#include <iostream>
#include "TMatrix.h"

using namespace std;

int main()
{
    int n, m;
    cout << "Enter numbers of rows and columns\n";
    cin >> n >> m;
    TMatrix<double> a(n, m);
    cout << "Enter your matrix\n";
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            cin >> a[i][j];
    }

    TVector<double> c(n);
    cout << "Enter your right side of the system\n";
    for(int i = 0; i < n; ++i)
        cin >> c[i];

    TVector<double> ans(n);
    ans = a.Gauss(c);
    for(int i = 0; i < n; ++i)
        cout << ans[i] << ' ';
    return 0;
}
