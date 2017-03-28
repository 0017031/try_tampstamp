#include "stdafx.h"

#include "gsl\gsl"

using namespace std;
using namespace gsl;

int main(const int argc, const char* argv[])
{
	const auto myArgSpan = make_span (&argv[0], argc);
	
	cout << "myArgSpan size:" << myArgSpan.length() << endl;
	
	
	return 1;
}