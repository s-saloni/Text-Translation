#ifndef TRANSLATION_H_
#define TRANSLATION_H_

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <iomanip>   

using namespace std;

#define ENG 0
#define FR 1
#define SPN 2
#define RUS 3

// array structure to hold words from each language
struct words{
    string words_array[71]; //71 words in each txt file
    string language; //holds name of language
};

struct hash_node{
     //hold addresses to translated terms of key
    string translated_values[4]; //one will always be empty
    string key;
    unsigned int hashcode;
};


typedef vector<shared_ptr<hash_node>> h_table;    

struct hash_table{
    int size = 0; //to check if all words in table (71 if no bucket collsions)
    int current_capacity;
    shared_ptr<h_table> nodes_tbl;    
};

class Translation {
public:
    Translation();
    ~Translation();

    //function to read and store words from .txt files into struct 'words'
    void store_words(string input_file, string lang);

    //array of pointers to 'words_array' of each language
    shared_ptr<words> words_ptrs[4]; 

    //initialize a new hash_node
    //takes in key, and finds the index it is located at
    //lang_index gives the language the key is in, and chooses corresponding
    // translated terms that are in other languages (0=Eng, 1=Fr, 2=Spn, 3=Rus)
    shared_ptr<hash_node> NewNode(string key, int lang_index, unsigned int hash_code);

    //function to initialize a new hash_table with given capactiy
    shared_ptr<hash_table> NewTable(int capacity);

    //maps node(key & value pair) to location in hash_table
    void MapNode(shared_ptr<hash_table>& table, string key, int lang_index);

    //fills in tables for each languages keys and translations
    void CreateAllTables();

    //array of pointers to access each hash table for each language
    shared_ptr<hash_table> all_tables[4]; 

    //finds bucket value for given key and language
    string FindWord(string key, int lang_from, int lang_to);

    //uses FindWord() to output the translation
    string Translate(string word, string lang_from, string lang_to);

private:
//function to convert each char hexadecimal (in string format) into an integer
int hexstr_to_int(string char_str);
//bucket function to map in hash table
int bucket_func(unsigned int hashcode, int cap);
//hash function that returns the hashcode with given key
int hash_func(string key);
};

#endif //TRANSLATION_H_