void printVnlVector(vnl_vector<double> v) {
  for(unsigned int i = 0; i < v.size(); i++) {
    cout << v[i] << " ";
  }
}

void printVnl44Mat(vnl_matrix<double,4,4> &m) {
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
}

