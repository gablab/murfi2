#include<vnl/vnl_matrix_fixed.h>
#include <iostream>

using namespace std;

void printVnl44Mat(vnl_matrix_fixed<double,4,4> &m) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

