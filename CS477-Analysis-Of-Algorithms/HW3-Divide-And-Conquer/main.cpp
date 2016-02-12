#include <vector>
#include <iostream>

using namespace std;

char maxHelper(vector<char> *);
char max(vector<char> *, int, int);
void mergesortBU(vector<char> *);
void merge(vector<char> *, int, int, int);

int main()
{
	string input = "TINYEXAMPLE";
	vector<char> * data;
	char maxElement;

	/**
	 * Divide and Conquer Max Example
	 */
	data = new vector<char>(input.c_str(), input.c_str() + input.length());
	maxElement = maxHelper(data);
	cout << "Overall max element of  \"" << input << "\": " << maxElement << "\n";

	/**
	 * Mergesort Bottom Up Example
	 */
	input = "ASORTINGEXAMPLE";
	data = new vector<char>(input.c_str(), input.c_str() + input.length());
	mergesortBU(data);
	cout << "Sorted version of \"" << input << "\" is: \"" << string(data->begin(), data->end()) << "\"";
}

char maxHelper(vector<char> * data)
{
	if(data == 0)
	{
		cout << "Bad data given to max function\n";
		return '~';
	}

	return max(data, 0, data->size() - 1);
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

void mergesortBU(vector<char> * data)
{
	if(data == 0)
	{
		cout << "Bad data given to mergesortBU function\n";
		return;
	}
	// m is size of merge
	for(int m = 1; m < data->size(); m = m+m)
	{
		cout << "Current width: " << m << "\n";
		for(int i = 0; i < data->size() - m; i += m+m)
		{
			merge(data, i, i + m-1, min(i+m+m-1, (int)data->size() - 1));
		}
	}
}

void merge(vector<char> * data, int start, int mid, int end)
{
	cout << "Input being processed: \"" << string(data->begin() + start, data->begin() + mid+1) << "|" << string(data->begin() + mid + 1, data->begin() + end + 1) << "\"\n";
	vector<char> aux;
	aux.reserve(end-start);
	for(int i = start; i <= end; i++)
	{
		aux.push_back(data->at(i));
	}

	for(int i = start, curLeft = 0, curRight = mid+1-start; i <= end; i++)
	{
		if(curLeft > mid-start) // Nothing remaining to add on the left side, add the remaining items on the right
		{
			data->at(i) = aux.at(curRight++);
		}
		else if(curRight > end-start) // Nothing remaining to add on the right side, add the remaining items on the left
		{
			data->at(i) = aux.at(curLeft++);
		}
		else if(aux.at(curLeft) < aux.at(curRight)) // If the item on the left is lesser, we want to grab that first
		{
			data->at(i) = aux.at(curLeft++);
		}
		else // Otherwise, the item on the right is lesser so we want that item
		{
			data->at(i) = aux.at(curRight++);
		}
	}
}