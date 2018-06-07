// #include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    // using namespace boost::lambda;
    typedef std::istream_iterator<int> itr_input_int;

    std::for_each(
        itr_input_int(std::cin), 
		itr_input_int(), 
		[](auto x){ 
			std::cout << (x * 3) << "\n\n" ;
		}
	);
}

// std::istream_iterator is a single-pass input iterator that reads successive objects of type T 
// from the std::basic_istream object for which it was constructed, by calling the appropriate operator>>. 

// The actual read operation is performed when the iterator is incremented, not when it is dereferenced. 
// The first object may be read when the iterator is constructed or when the first dereferencing is done. 
// Otherwise, dereferencing only returns a copy of the most recently read object.


// The default-constructed std::istream_iterator is known as the end-of-stream iterator. 
// When a valid std::istream_iterator reaches the end of the underlying stream, 
// it becomes equal to the end-of-stream iterator. 
// Dereferencing or incrementing it further invokes undefined behavior.