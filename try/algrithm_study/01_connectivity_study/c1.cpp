#include "stdafx.h"

#include "union_find_class.hpp"

using namespace std;
using namespace gsl;

int main(const int argc, const char *argv[]) {
  // convert argc/argv to myArgSpan
  const auto myArgSpan = make_span(&argv[0], argc);
  cout << "myArgSpan size:" << myArgSpan.length() << endl;

  // validate the input
  if (myArgSpan.length() <= 2)
    return -1;

  cout << stol(myArgSpan[1]) << " " << stol(myArgSpan[2]) << endl;

  // make_pair from input
  // auto myPair = make_pair(stol(myArgSpan[1]), stol(myArgSpan[2]));

  return 0;
}

