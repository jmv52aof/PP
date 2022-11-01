#include <iostream>
#include <vector>
#include <algorithm>
#include <ctype.h>

using namespace std;

void print_vector(vector<int> v)
{
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}

int main()
{
	vector<int> a = { 8, 2, 3, 4, 5, 6, 7, 8 };

	vector<int> b = {};
	transform(
		a.begin(), 
		a.end(),
		back_inserter(b),
		[max_elem = INT_MIN, min_elem = INT_MAX] (int item) mutable {
			max_elem = max(max_elem, item);
			min_elem = min(min_elem, item);
			return max_elem - min_elem;
		}
	);
	print_vector(b);
}
