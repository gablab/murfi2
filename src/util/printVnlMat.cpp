#include<vnl/vnl_matrix.h>
#include <iostream>

using namespace std;

void printVnlMat(vnl_matrix<double> &m) {
  for(int i = 0; i < m.rows(); i++) {
    for(int j = 0; j < m.cols(); j++) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}


void printVnlMat(vnl_matrix<double> &m, int numCols) {
  for(int i = 0; i < m.rows(); i++) {
    for(int j = 0; j < numCols; j++) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

