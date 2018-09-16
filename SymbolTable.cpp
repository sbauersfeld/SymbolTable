#include "SymbolTable.h"
#include <string>
#include <vector>
#include <stack>
using namespace std;

// This class does the real work of the implementation.

class SymbolTableImpl
{
private:
	// linked list that contains the line number and scope for each declared variable
	struct list {
		string id;
		int nums;
		int scopelvl;
		list* next;
	};	
	list* table[20000];			//hash table of 20000 pointers to list structures
	vector<int> lastbin;		//vector stores the bin number of each declared variable
	int currentscope;			//keeps track of current scope level
public:
	~SymbolTableImpl();
	SymbolTableImpl();
	void enterScope();
	bool exitScope();
	bool declare(const string& id, int lineNum);
	int find(const string& id) const;
	void removeId(int bin);
	unsigned int evalId(const string& id) const;
	bool insert(int bin, const string& id, int linenum);
};

SymbolTableImpl::SymbolTableImpl() {
	lastbin.push_back(-1);			//placeholder that marks boundary between scopes
	currentscope = 0;				//start at scope 0
	for (int i = 0; i < 20000; i++) {
		table[i] = nullptr;				//initialize all hash table pointers to null
	}

}

SymbolTableImpl::~SymbolTableImpl() {
	for (int i = lastbin.size() - 1; lastbin[i] != -1; i--) {			//removes any variables declared in scope zero
		removeId(lastbin[i]);				//calls remove list node function for all bin numbers in bin vector
		lastbin.pop_back();
	}
}

void SymbolTableImpl::enterScope()
{
	lastbin.push_back(-1);				//insert placeholder to mark boundary between scope levels
	currentscope++;						//increment scope level
}

bool SymbolTableImpl::exitScope()
{
	if (currentscope == 0)			//if scope has been exited more than entered
		return false;

	for (int i = lastbin.size() - 1; lastbin[i] != -1; i--) {		//removes all list nodes declared within exited scope
		removeId(lastbin[i]);				//calls remove list node function for all bin numbers in bin vector
		lastbin.pop_back();
	}
	lastbin.pop_back();					//removes -1 placeholder
	currentscope--;						//decrement scope by one

	return true;
}

void SymbolTableImpl::removeId(int bin) {
	list* temp = table[bin];					//removes node at front of hash table bucket corresponding to bin number
	table[bin] = temp->next;
	delete temp;
}

unsigned int SymbolTableImpl::evalId(const string& id) const {			//assigns bucket number to string id
	unsigned int h = 2166135261U;
	for (int k = 0; k < id.size(); k++) {
		h += id[k];
		h *= id[k];
	}
	return h % 20000;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
	if (id.empty())
		return false;

	int bin = evalId(id);				//assign bucket number based on string of id
	bool result = insert(bin, id, lineNum);			//insert a new node into the hash table at corresponding bucket
	if (result) {					//if the variable has not already been declared in the current scope
		lastbin.push_back(bin);					//add the bin number to our current scope vector
		return true;
	}
	else return false;
}

bool SymbolTableImpl::insert(int bin, const string& id, int linenum) {

	list* ptr = table[bin];

	while (ptr != nullptr) {				//determines if the new id has already been declared in the current scope
		if (ptr->id == id && ptr->scopelvl == currentscope)
			return false;			

		ptr = ptr->next;
	}
	
	list* temp = new list;				//adds new node with id, linenumber, and scope at beginning of linked list
	temp->next = table[bin];
	temp->id = id;
	temp->nums = linenum;
	temp->scopelvl = currentscope;
	table[bin] = temp;
	return true;
	
}

int SymbolTableImpl::find(const string& id) const
{
	if (id.empty())
		return -1;

	int bin = evalId(id);
	list* ptr = table[bin];
	while (ptr != nullptr) {			//loops through linked list at target bin number until it finds the target id
		if (ptr->id == id)
			break;
		ptr = ptr->next;
	}
	if (ptr == nullptr)			//returns -1 if id is not found
		return -1;

	return ptr->nums;			//returns line number of id
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.


SymbolTable::SymbolTable()
{
	m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
	delete m_impl;
}

void SymbolTable::enterScope()
{
	m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
	return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
	return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
	return m_impl->find(id);
}
