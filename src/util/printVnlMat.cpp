
void printVnlMat(vnl_matrix<double> &m) {
  for(int i = 0; i < m.rows(); i++) {
    for(int j = 0; j < m.cols(); j++) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

