#include <chrono>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <thread>

using namespace std;
using namespace std::filesystem;

int main(int argc, char *argv[]) {
  if(argc < 3) {
    cout << "usage: " << argv[0] << " srcDicomDir dstDicomDir [tr(ms), default 2000]" << endl;
    return 0;
  }

  const path inputDir{argv[1]};
  const path outputDir{argv[2]};
  int tr = argc >= 4 ? atoi(argv[3]) : 2000;

  set<path> sortedDirEntries;
  for(const auto &entry: directory_iterator(inputDir)) {
    sortedDirEntries.insert(entry.path());
  }

  for(const auto &filepath: sortedDirEntries) {
    stringstream cmd;
    cmd << "cp "<< filepath << " " << outputDir;
    cout << "executing: " << cmd.str() << endl;
    if(system(cmd.str().c_str())) {
      cerr << "failed to copy " << filepath << " to " << outputDir << endl;
      return 1;
    }
    this_thread::sleep_for(chrono::milliseconds(tr));
  }

  cout << "copied all files from " << inputDir << endl;

  return 0;
}
