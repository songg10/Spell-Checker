
// a5.cpp

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Song Tung Nguyen>
// St.# : <301354423>
// Email: <stn5@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// You are only allowed to include these files --- don't include anything else!
// If you think it's necessary to include another file, please speak to the
// instructor.
//

#include "cmpt_error.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>   // for reading/writing text files

using namespace std;

class Hashtable{
private:
	unsigned long long sze; //size: number of values are currently in the hashtable
	unsigned long long cap; //capacity: the size of the hashtable

	struct HashNode{
		string value;
	};

	HashNode** arr;  //bucket

	//determine whether the number is prime or not
	bool IsPrime(unsigned long long number){
		if (number == 2 || number == 3)
	        return true;

	    if (number % 2 == 0 || number % 3 == 0)
	        return false;

	    int divisor = 6;
	    while (divisor * divisor - 2 * divisor + 1 <= number)
	    {

	        if (number % (divisor - 1) == 0)
	            return false;

	        if (number % (divisor + 1) == 0)
	            return false;

	        divisor += 6;

	    }

	    return true;

	}

	//find the next prime number that is >= a
	unsigned long long NextPrime(unsigned long long a){
		while (!IsPrime(++a)){ }
    	return a;
	}

	unsigned long long hashing(const string &s) const{
		unsigned long long h = 0;
		for (int i = 0; i < s.size(); i++)
		{
			h += static_cast<unsigned long long>(s[i]);
		}

		return h;
	}

	void rehashing ()
	{
		unsigned long long oldCap = cap;
		sze = 0;
		//Doubling the capacity
		cap = NextPrime(cap*2);

		HashNode** oldArr = arr;
		arr = new HashNode*[cap]();

		//moving the values to the new after rehashing
		for (unsigned long long i = 0; i < oldCap; i++){
			if (oldArr[i] != nullptr){
				for (unsigned long long j = 0; j < cap; j++){
					unsigned long long index = (hashing(oldArr[i]->value) + j*j) % cap;
					if (arr[index] == nullptr){
						arr[index] = new HashNode {oldArr[i]->value};
						sze++;
						break;
					} //if
				} //for
				delete oldArr[i];
				oldArr[i] = nullptr;
			} //if
		} //for

		delete[] oldArr;
	}

public:
	// Constructor
	Hashtable(int ini_cap = 101) : sze(0), cap(ini_cap), arr(new HashNode*[cap]){
		for (int i = 0; i < cap; i++)
		{
			//nullptr indicates the bucket is empty
			arr[i] = nullptr;
		}

	}

	//Destructor
	~Hashtable(){
		for (unsigned long long i = 0; i < cap; i++){
			if (arr[i] != nullptr){
				delete arr[i];
				arr[i] = nullptr;
			}
		}
		delete[] arr;
	}

	unsigned long long size() const {return sze;}
	unsigned long long capacity() const {return cap;}
	double load_factor() const {return double(sze)/cap;}

	bool is_empty() const {return sze == 0;}
	bool is_full() const {return sze == cap;}

	void put(const string& s){
		//Initialize a new node for the new input
		HashNode* temp = new HashNode{s};
		
		//Insert using quadratic probing
		for (unsigned long long i = 0; i < cap; i++){
			unsigned long long index = (hashing(s) + i*i) % cap;
			if (arr[index] == nullptr){
				arr[index] = temp;
				sze++;
				break;
			}
		}
		
		//rehashing if the load factor > 0.5
		if (load_factor() > 0.5){
			rehashing();
		} //if 

	} //put
	
	bool find(const string& s) const{
		if (is_empty()){
			return false;
		}
		for (unsigned long long i = 0; i < cap; i++){
			unsigned long long index = (hashing(s) + i*i) % cap;
			if (arr[index] != nullptr){
				if (arr[index]->value == s){
					return true;
				}
			}
		}

		return false;	
	} //search

	void remove (const string &s) {
		if (is_empty()){
			return;
		}
		for (unsigned long long i = 0; i < cap; i++){
			unsigned long long index = (hashing(s) + i*i) % cap;
			if (arr[index] != nullptr && arr[index]->value == s){
				HashNode* temp = arr[index];
				arr[index] = nullptr;
				delete temp;
				return;
			}
		}
	} //remove


}; //class Hashtable

void SpellChecker(Hashtable& temp, string word){
	//Original idea from: http://www.programmingnotes.org/?p=3144

    int suggestion = 0;
    string remove[256];
    int numRemove=0;
            
        // alteration & insertion
        for(unsigned x = 0; x < word.length(); ++x)
        {       
            string alteration = word;
            for(char c = 'a'; c <= 'z'; ++c)
            {
                //alteration
                alteration[x] = c;
                if(temp.find(alteration) == true)
                {
                    cout<< "   " << alteration<<endl;
                    remove[numRemove++] = alteration;
                    ++suggestion;
                    // remove the entry so it isnt displayed multiple times
                    temp.remove(alteration);
                }
                //insertion
                string insertion = word.substr(0, x) + c + word.substr(x);
                if(temp.find(insertion))
                {
                    cout<<"   " << insertion<<endl;  
                    remove[numRemove++] = insertion;
                    ++suggestion;
                    // remove the entry so it isnt displayed multiple times
                    temp.remove(insertion);
                }
            }
        }
        // transposition & deletion 
        for(unsigned x = 0; x < word.length()-1;++x) 
        {
            // transposition 
            string transposition = word.substr(0,x) + word[x+1] + word[x] + word.substr(x+2);
            if(temp.find(transposition))
            {
                cout<< "   " <<transposition << endl;
                remove[numRemove++] = transposition;
                ++suggestion;
                // remove the entry so it isnt displayed multiple times
                temp.remove(transposition);

            }
            // deletion
            string deletion = word.substr(0, x)+ word.substr(x + 1);
            if(temp.find(deletion))
            {
                cout<< "   " << deletion<<endl;
                remove[numRemove++] = deletion;
                ++suggestion;
                // remove the entry so it isnt displayed multiple times
                temp.remove(deletion);
            }
        }       
        // place the removed items back inside the hash table
        while(numRemove>=0)
        {
            temp.put(remove[numRemove--]);
        }
    
}

int main() {
	ifstream dictionary;
	ifstream addedwords;
	dictionary.open("maindict.txt");
	addedwords.open("addedwords.txt");
	Hashtable table1;
	Hashtable table2;
	
	if (dictionary.is_open()){
		string line;
		while (getline(dictionary, line)){
			table1.put(line);
		}
		dictionary.close();
	}

	else{
		cout <<"Could not open the dictionary" << endl;
	}

	if (addedwords.is_open()){
		string line;
		while (getline(addedwords, line)){
			table2.put(line);
		}
		addedwords.close();
	}

	else{
		cout <<"Could not open the text file for addedwords" << endl;
	}


	cout << "Welcome to Trivial SpellChecker: " << endl << endl;

	while (1){
		string input;
		string command;
		string word;
		getline(cin, input);
		istringstream iss(input);
		iss >> command >> word;

		if (command == "check"){
			if (table1.find(word) == true || table2.find(word) == true){
				cout << "ok" << endl;
			}
			else{
				cout <<"don't know " << word << endl;
			}
			cout << endl;
		} //if check

		else if (command == "add"){
			if (table1.find(word) == false && table2.find(word) == false){
				table2.put(word);
				ofstream addnew;
				addnew.open("addedwords.txt", ios::app);

				if (addnew.is_open()){
					addnew << word << endl;
					addnew.close();
					cout << word << " added to the dictionary" << endl;
				}

				else{
					cout << "Can't open 'addedwords.txt'. Hence, can't add " << word << " to the file" << endl;
				}

				
			}

			else{
				cout << "The word is already known (either in maindict.txt or addedwords.txt)" << endl;
			}
			cout << endl;
		} //if add

		else if (command == "remove"){
			if (table1.find(word) == true){
				cout <<"can't remove "<< word << ": it's in the main dictionary." << endl;
			}

			else if (table2.find(word) == true){
				table2.remove(word);
				ifstream removeItem;
				ofstream temp;
				removeItem.open("addedwords.txt");
				temp.open("temp.txt");

				if (removeItem.is_open() && temp.is_open()){
					string line;
					while (getline(removeItem, line)){
						if (line != word){
							temp << line << endl;
						}
					}
					removeItem.close();
					temp.close();
					remove("addedwords.txt");
					rename("temp.txt", "addedwords.txt");
					cout << word << " removed" << endl;
				}
				
				else{
					cout << "Can't open 'addedwords.txt'. Hence, can't remove " << word << " from the file" << endl;
				}
			}

			else{
				cout <<"can't remove " << word <<": unknown word." << endl;
			}
			cout << endl;
		} //if remove

		else if (command == "checkfile"){
			ifstream checkfile;
			checkfile.open(word);

			if(checkfile.is_open()){
				string line;
				int count = 0;
				while (getline(checkfile, line)){
					count++;
					if (table1.find(line) == false && table2.find(line) == false){
						cout << "Line " << count << ": " << line << endl;
					}
				}
				checkfile.close();
			}

			else{
				cout << "cannot find the file" << endl;
			}
			cout << endl;
		} //if checkfile

		else if (command == "suggest"){
			cout<<"suggested words: " << endl;
			SpellChecker(table1, word);
			SpellChecker(table2, word);
			cout << endl;
		} //if suggest

		else if (command == "stop" || command == "quit" || command == "end" || command == "done"){
			break;
		} //if stop

		else{
			cout << "unknown command" << endl << endl;
		} //if unknown command
	}

	return 0;
} // main
