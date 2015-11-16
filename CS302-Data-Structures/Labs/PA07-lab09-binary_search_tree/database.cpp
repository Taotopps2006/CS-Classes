/** @file database.cpp
@author Tim Kwist
@version 1.0
@brief This program will implement Exercise 1 for BSTree
@details The specifications of this program are defined by C++ Data Structures: A Laboratory Course (3rd edition) by Brandle, JGeisler, Roberge, Whittington, lab 9 exercise 1.
@date Wednesday, October 8, 2014
*/

//--------------------------------------------------------------------
//
//  Laboratory 11, In-lab Exercise 1                     database.cs
//
//  (Shell) Indexed accounts database program
//
//--------------------------------------------------------------------

// Builds a binary search tree index for the account records in the
// text file accounts.dat.

#include <iostream>
#include <fstream>
#include "BSTree.cpp"
#include <cstdlib>

using namespace std;

//--------------------------------------------------------------------
//
// Declarations specifying the accounts database
//

const int nameLength      = 11;   // Maximum number of characters in
                                  //   a name
const long bytesPerRecord = 37;   // Number of bytes used to store
                                  //   each record in the accounts
                                  //   database file

struct AccountRecord
{
    int acctID;                   // Account identifier
    char firstName[nameLength],   // Name of account holder
         lastName[nameLength];
    double balance;               // Account balance
};

//--------------------------------------------------------------------
//
// Declaration specifying the database index
//

struct IndexEntry
{
    int acctID;              // (Key) Account identifier
    long recNum;             // Record number

    int getKey () const
        { return acctID; }   // Return key field
};

//--------------------------------------------------------------------
/**
* Get the account ids and store them into a binary search tree with appropriate record number, then output the account ids from least to greatest.
* Next user will input an account id; if it exists in the list, find it and output all of the account details of the account id.
*/
int main ()
{
    ifstream acctFile("accounts.dat");   // Accounts database file
    AccountRecord acctRec;                // Account record
    BSTree<IndexEntry,int> index;         // Database index
    IndexEntry entry;                     // Index entry
    int searchID = 0;                         // User input account ID
    long recNum = 0;                          // Record number
    // Iterate through the database records. For each record, read the
    // account ID and add the (account ID, record number) pair to the
    // index.
	acctFile >> entry.acctID;
	for(entry.recNum = 0; !acctFile.eof(); entry.recNum++)
	{
		index.insert(entry);
		string temp;
		acctFile >> temp >> temp >> temp;
		acctFile >> entry.acctID;
	}
    // Output the account IDs in ascending order.
	cout << endl;
	cout << "Account IDs : " << endl;
	index.writeKeys();
	cout << endl;

    // Clear the status flags for the database file.
    acctFile.clear();

    // Read an account ID from the keyboard and output the
    // corresponding record.
	cout << "Enter account ID : ";
	while(cin >> searchID)
	{
		if(index.retrieve(searchID, entry))
		{
			cout << entry.recNum << " : ";
			acctFile.seekg(bytesPerRecord*entry.recNum, ios::beg);
			acctFile >> acctRec.acctID;
			acctFile >> acctRec.firstName;
			acctFile >> acctRec.lastName;
			acctFile >> acctRec.balance;
			cout << acctRec.acctID << " " << acctRec.firstName << " " << acctRec.lastName << " " << acctRec.balance << endl;
		}
		else
		{
			cout << "No record with that account ID" << endl;
		}
		cout << "Enter account ID (EOF to quit): ";
	}
	return 0;
}