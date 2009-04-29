// Example program to demonstrate the timer class

#include "timer.h"

using namespace std;

int main()
{
  timer t;

  // Start the timer and then report the time required to run a
  // big loop.  The string argument to start() is optional.
  t.start("Timer started");
  for (int i = 0; i < 1000000000; ++i);
  cout << "Check 1: " << t << endl;

  // Restart the timer and time another loop.
  t.restart("Timer restarted");
  for (int i = 0; i < 1000000000; ++i);
  cout << "Check 2: " << t << endl;

  // Stop the timer and repeat the loop.  The third timer check
  // should report the same value as the second.
  t.stop("Timer stopped");
  for (int i = 0; i < 1000000000; ++i);
  cout << "Check 3: " << t << endl;

  // Start the timer again.  Since there is no restart, the timer
  // will start from where it left off the last time it was stopped.
  t.start("Timer started, not restarted");
  for (int i = 0; i < 1000000000; ++i);
  // The 'check' member function gives a default
  // method of printing the current elapsed time.
  t.check("Check 4");

  cout << "Now sleeping for 2 hours...\n";
  //sleep(60*60*2);
  cout << "Check 5: " << t << endl;


  return 0;
}

