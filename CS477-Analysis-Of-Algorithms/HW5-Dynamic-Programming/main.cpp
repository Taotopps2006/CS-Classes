#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Payoff
{
    Payoff(int _l, int _h)
    {
        l = _l;
        h = _h;
    }

    int l; // low payoff
    int h; // high payoff
};

struct Value
{

    Value(int _l, int _h, int _n)
    {
        l = _l;
        h = _h;
        n = _n;
    }

    int l; // low value
    int h; // high value
    int n; // none value
};

struct Path
{
    Path(char _l, char _h, char _n)
    {
        l = _l;
        h = _h;
        n = _n;
    }

    char l; // path at low
    char h; // path at high
    char n; // path at none
};

int findMaxValue(vector<Payoff> payoffMatrix, vector<Value> *valueMatrix);
int findMaxValue(vector<Payoff> payoffMatrix, vector<Value> *valueMatrix, vector<Path> *pathMatrix, char &lastChoice);
vector<char> findPathOfMaxValue(char lastDecision, vector<Path> pathMatrix);
int max3(int x, int y, int z);
int max3(int x, int y, int z, char &retChar);
void printVector(vector<char> v);
void printVector(vector<Path> v);
void printVector(vector<Payoff> v);
void printVector(vector<Value> v);


int main() 
{
    char maxChoice = '0';
    int maxValue = 0;
    vector<char> pathVector;
    vector<Path> *pathMatrix = new vector<Path>;
    vector<Payoff> payoffMatrix;
    vector<Value> *valueMatrix = new vector<Value>;

    payoffMatrix.push_back(Payoff(10, 5));
    payoffMatrix.push_back(Payoff(1, 50));
    payoffMatrix.push_back(Payoff(10, 5));
    payoffMatrix.push_back(Payoff(10, 1));
    
    cout << "Payoff Matrix:" << endl;
    printVector(payoffMatrix);

    cout << "Part B" << endl;
    maxValue = findMaxValue(payoffMatrix, valueMatrix);
    cout << "Value Matrix:" << endl;
    printVector(*valueMatrix);
    cout << "Max Value:" << maxValue << endl;
    
    valueMatrix = new vector<Value>;
    
    cout << "Part C" << endl;
    maxValue = findMaxValue(payoffMatrix, valueMatrix, pathMatrix, maxChoice);
    cout << "Value Matrix" << endl;
    printVector(*valueMatrix);
    cout << "Max Value found at: " << maxChoice << endl;
    cout << "Max Value:" << maxValue << endl;
    cout << "Path Matrix:" << endl;
    printVector(*pathMatrix);

    cout << "Part D" << endl;
    pathVector = findPathOfMaxValue(maxChoice, *pathMatrix);
    cout << "Path to max value: ";
    printVector(pathVector);


    return 0;
}

/**
 * Part B
 * Value matrix holds the max value possible at each week for a given choice (low, high, none)
 * Returns the overall highest possible value given payoffMatrix
 */ 
int findMaxValue(vector<Payoff> payoffMatrix, vector<Value> *valueMatrix)
{
    // Highest value at week 1 is equal to the low/high/0 of the payoff matrix
    valueMatrix->push_back(Value(payoffMatrix[0].l, payoffMatrix[0].h, 0));
    int n = payoffMatrix.size();
    for(int i = 1; i < n; i++)
    {
        // New Low value: max of last week (any choice can choose low in the following week) + low at this week
        // New High value: value of none at last week (must choose none in the week preceding a high decision) + high at this week
        // New None value: max of last week (Choosing none gives no additional value, so best possible none is highest of previous week)
        int prevMax = max3((*valueMatrix)[i-1].l, (*valueMatrix)[i-1].h, (*valueMatrix)[i-1].n);
        valueMatrix->push_back(Value(payoffMatrix[i].l + prevMax, payoffMatrix[i].h + (*valueMatrix)[i-1].n, prevMax));
    }

    return max3((*valueMatrix)[n-1].l,(*valueMatrix)[n-1]. h, (*valueMatrix)[n-1].n);
}

/**
 * Part C
 * Value matrix holds the max value possible at each week for a given choice (low, high, none)
 * Path matrix holds the choice made at the previous week to get to the value made at the given choice for the current week
 * Returns the overall highest possible value given payoffMatrix
 */ 
int findMaxValue(vector<Payoff> payoffMatrix, vector<Value> *valueMatrix, vector<Path> *pathMatrix, char &lastChoice)
{
    // Highest value at week 1 is equal to the low/high/0 of the payoff matrix
    valueMatrix->push_back(Value(payoffMatrix[0].l, payoffMatrix[0].h, 0));
    // No choice made at week 1, so all choices are set to N
    pathMatrix->push_back(Path('N', 'N', 'N'));
    int n = payoffMatrix.size();
    for(int i = 1; i < n; i++)
    {
        // New Low value: max of last week (any choice can choose low in the following week) + low at this week
        // New High value: value of none at last week (must choose none in the week preceding a high decision) + high at this week
        // New None value: max of last week (Choosing none gives no additional value, so best possible none is highest of previous week)
        char maxChar = '0';
        int prevMax = max3((*valueMatrix)[i-1].l, (*valueMatrix)[i-1].h, (*valueMatrix)[i-1].n, maxChar);
        valueMatrix->push_back(Value(payoffMatrix[i].l + prevMax, payoffMatrix[i].h + (*valueMatrix)[i-1].n, prevMax));
        pathMatrix->push_back(Path(maxChar, 'N', maxChar));
    }

    return max3((*valueMatrix)[n-1].l,(*valueMatrix)[n-1]. h, (*valueMatrix)[n-1].n, lastChoice);
}

vector<char> findPathOfMaxValue(char lastDecision, vector<Path> pathMatrix)
{
    vector<char> retVector(pathMatrix.size(), '0');
    retVector.back() = lastDecision;
    for(int i = pathMatrix.size() - 1; i > 0; i--)
    {
        if(retVector[i] == 'L')
        {
            retVector[i-1] = pathMatrix[i].l;
        }
        else if(retVector[i] == 'H')
        {
            retVector[i-1] = pathMatrix[i].h;
        }
        else if(retVector[i] == 'N')
        {
            retVector[i-1] = pathMatrix[i].n;
        }
    }
    return retVector;
}

/**
 * Format:
 * x = low value
 * y = high value
 * z = none value
 * Returns the max between these 3 values
 */
int max3(int x, int y, int z)
{
    return max(max(x, y), z);
}

/**
 * Format:
 * x = low value
 * y = high value
 * z = none value
 * Returns the max between these 3 values
 * Returns (by reference) the character associated with the max of these values
 */
int max3(int x, int y, int z, char &retChar)
{
    // Compare low and high and store the max val & character between the two
    // Compare max of high/low with none; if none is higher, then it will overtake
    // Otherwise, the max of high/low will be max of all 3 and return char has already
    // been set
    int m = x; // m holds the 'current' largest value
    if(y > m)
    {
        m = y;
        retChar = 'H';
    }
    else
    {
        retChar = 'L';
    }
    
    if(z > m)
    {
        m = z;
        retChar = 'N';
    }
    
    return m;
}

void printVector(vector<char> v)
{
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;
}

void printVector(vector<Path> v)
{
    cout << "Low:  ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].l << " ";
    }
    cout << endl;
    cout << "High: ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].h << " ";
    }
    cout << endl;
    cout << "None: ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].n << " ";
    }
    cout << endl;
}

void printVector(vector<Payoff> v)
{
    cout << "Low:  ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].l << " ";
    }
    cout << endl;
    cout << "High: ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].h << " ";
    }
    cout << endl;
}

void printVector(vector<Value> v)
{
    cout << "Low:  ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].l << " ";
    }
    cout << endl;
    cout << "High: ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].h << " ";
    }
    cout << endl;
    cout << "None: ";
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i].n << " ";
    }
    cout << endl;
}
