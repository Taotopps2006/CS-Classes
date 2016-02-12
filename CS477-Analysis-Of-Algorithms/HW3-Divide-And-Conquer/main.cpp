#include <vector>
#include <iostream>

using namespace std;

char max(vector<char> *, int, int);

int main()
{
	string input = "TINYEXAMPLE";
	vector<char> * data = new vector<char>(input.c_str(), input.c_str() + input.length());
	char maxElement = max(data, 0, data->size() - 1);
	cout << "Overall max element of  \"" << input << "\": " << maxElement << "\n"; 
}

char max(vector<char> * data, int start, int end)
{
	char maxElement = data->at(start);
	// If there's only one element in our range, return that element
	if(end - start != 1)
	{
		// Otherwise, split the data into two halves based on the midpoint
		// Then find the max from each side of the data
		int midPoint = (start + end) / 2;
		char left = max(data, start, midPoint);
		char right = max(data, midPoint, end);
		// Save whichever element is greater
		maxElement = left > right ? left : right;
	}

	cout << "<" << maxElement << ", \"Max(" << start << ", " << end << ")\">\n";

	return maxElement;
}