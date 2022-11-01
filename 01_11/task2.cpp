#include <iostream>
#include <vector>
#include <algorithm>
#include <ctype.h>

using namespace std;

void print_vector(vector<float> v)
{
	copy(v.begin(), v.end(), ostream_iterator<float>(cout, " "));
	cout << endl;
}

int main()
{
	vector<int> a = { 8, 2, 3, 4, 5, 6, 7, 8 };

	vector<float> b = {};
	transform(
		a.begin(), 
		a.end(),
		back_inserter(b),
		[sum = 0, index = 0] (int item) mutable {
			sum += item;
			return (float) sum / ++index;
		}
	);
	print_vector(b);
}