/** @file login.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement the Exercise 1 of Lab 10 Hash Table
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 10.
@date Wednesday, October 29, 2014
*/

#include "HashTable.cpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

class TestData {
  public:
    TestData();
    void setKey(const string& newKey);
	void setValue(const string& newValue);
    string getKey() const;
    string getValue() const;
    static unsigned int hash(const string& str);

  private:
    string key; // Username
    string value; // Password
    static int count;
};

int TestData::count = 0;

TestData::TestData(){
}

void TestData::setKey(const string& newKey) {
    key = newKey;
}

void TestData::setValue(const string& newValue) {
	value = newValue;
}

string TestData::getKey() const {
    return key;
}

string TestData::getValue() const {
    return value;
}

unsigned int TestData::hash(const string& str) {
    unsigned int val = 0;

    for (int i = 0; i < str.length(); ++i) {
	val += str[i];
    }

    return val;
}

/**
 * This method will simulate a login authenticator.
 * The list of login/password combos will be read from a file, password.dat, and stored in a hash table.
 * Then the user will be allowed to input usernames and passwords which will be ran against the hash table.
 * If valid login/password, 'Authentication successful' will be output. Otherwise, 'Authentication failure' will be output.
 *
 * @param None
 * @return None
 * @pre password.dat is a valid file to open
 * @post None
*/
int main()
{
	
	HashTable<TestData, string> table(8);
	ifstream passwordFile("password.dat");
	TestData data;
	string tmp;
	while(passwordFile >> tmp)
	{
		data.setKey(tmp);
		passwordFile >> tmp;
		data.setValue(tmp);
		table.insert(data);
	}
	table.showStructure();
	cout << "Login: ";
	while(cin >> tmp)
	{
		table.retrieve(tmp, data);
		cout << "Password: ";
		cin >> tmp;
		if(data.getValue() == tmp)
		{
			cout << "Authentication successful";
		}
		else
		{
			cout << "Authentication failure";
		}
		
		cout << endl << "Login: ";
		
	}
}