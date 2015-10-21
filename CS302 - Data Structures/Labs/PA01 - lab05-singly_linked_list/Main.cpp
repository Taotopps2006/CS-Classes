#include "ListLinked.cpp"

int main()
{
	List<int> list1;
	List<int> list2;

	list1.insert(5);
	list1.insert(10);
	list1.insert(15);

	list2 = list1;

	list1.insert(10);
	list1.insert(5);

}