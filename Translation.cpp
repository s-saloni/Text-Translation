#include "Translation.h"

//constructor
Translation::Translation(){
} 

//destructor
Translation::~Translation(){
}

void Translation::store_words(string input_file, string lang){
    fstream txt_file;
    string word; //for each word/phrase in the file
    txt_file.open(input_file); //open file
    
    shared_ptr<words> words_lang(new words);
    words_lang->language =  lang; //set language for this file

    //if file could not be opened:
    if(!txt_file){
        cout << "Error. Could not open file." << endl;
    }
    //if file is open and there are lines left
    int i = 0;
    while(getline(txt_file, word)){
        words_lang->words_array[i] = word; //insert word/phrase
        i++;
    }
    if(lang == "English"){ //insert pointer into words_ptrs[]
        words_ptrs[0] = words_lang;
    }else if(lang == "French"){
        words_ptrs[1] = words_lang;
    }else if(lang == "Spanish"){
        words_ptrs[2] = words_lang;
    }else if(lang == "Russian"){
        words_ptrs[3] = words_lang;
    }
    
    txt_file.close(); //close file when finished
}

//Initialize a hash node
shared_ptr<hash_node> Translation::NewNode(string key, int lang_index, unsigned int hash_code){
    shared_ptr<hash_node> new_node(new hash_node);
    new_node->hashcode = hash_code;
    //new_node->key = key;
    
    int words_index; int i=0;
    //search through coressponding array to find idx location of key
    for(auto w : words_ptrs[lang_index]->words_array){
        if (w == key){
            words_index = i;
        }
        i++;
    }
    new_node->key = words_ptrs[lang_index]->words_array[words_index];

    //insert "addresses" for translated terms into array of "values"
    if(lang_index == 0){ //if key is English
        new_node->translated_values[1] = words_ptrs[1]->words_array[words_index];
        new_node->translated_values[2] = words_ptrs[2]->words_array[words_index];
        new_node->translated_values[3] = words_ptrs[3]->words_array[words_index];
    }
    if(lang_index == 1){ //if key is French
        new_node->translated_values[0] = words_ptrs[0]->words_array[words_index];
        new_node->translated_values[2] = words_ptrs[2]->words_array[words_index];
        new_node->translated_values[3] = words_ptrs[3]->words_array[words_index];
    }
    if(lang_index == 2){ //if key is Spanish
        new_node->translated_values[0] = words_ptrs[0]->words_array[words_index];
        new_node->translated_values[1] = words_ptrs[1]->words_array[words_index];
        new_node->translated_values[3] = words_ptrs[3]->words_array[words_index];
    }
    if(lang_index == 3){ //if key is Russian
        new_node->translated_values[0] = words_ptrs[0]->words_array[words_index];
        new_node->translated_values[1] = words_ptrs[1]->words_array[words_index];
        new_node->translated_values[2] = words_ptrs[2]->words_array[words_index];
    }
    return new_node;
}


int Translation::hash_func(string key){
    int prime = 71;
    unsigned int hash_code = 0;

    for(auto i : key){
        string char_str(1, i);
        int utf8 = hexstr_to_int(char_str);
        hash_code = prime*hash_code + utf8;
    }

    return hash_code;   
}
//hash function from: https://www.youtube.com/watch?v=jtMwp0FqEcg&t=362s


int Translation::hexstr_to_int(string char_str){
    //get hexadecimal string of char
    stringstream str_stream;
    for(char c : char_str){
    str_stream << hex << static_cast<int>(c);
    }
    string utf8_str (str_stream.str());
    
    unsigned int utf8_int = 0;
    for (int i=0; i < utf8_str.length(); i++){
        utf8_int += int(utf8_str.at(i)); //accumulate ascii values of string
    }
    return utf8_int;
}


shared_ptr<hash_table> Translation::NewTable(int capacity){
    shared_ptr<hash_table> new_tbl(new hash_table);
    new_tbl->current_capacity = capacity;
    //create h_table vector of size 'capacity' (each ptr will be null)
    new_tbl->nodes_tbl = shared_ptr<h_table> (new h_table(capacity));
    return new_tbl;
}


int Translation::bucket_func(unsigned int hashcode, int cap){
    int bucket = hashcode % cap;
    return bucket;
}


void Translation::MapNode(shared_ptr<hash_table>& table, string key, int lang_index){
    unsigned int hashcode = hash_func(key);
    shared_ptr<hash_node> node = NewNode(key, lang_index, hashcode);
    
    int bucket = 0;
    bucket = bucket_func(hashcode, table->current_capacity);
    int probed = 0; 
    
    while(probed < table->current_capacity){
        if(table->nodes_tbl->at(bucket) == NULL){ //if bucket is empty, insert node
            table->nodes_tbl->at(bucket) = node;
            table->size++;
            //cout << bucket << " is bucket for key " << node->key << endl << endl;
            return;
        }

        //I have included a few words that have the same translated terms,
        //so there will be bucket collisions
        else if(table->nodes_tbl->at(bucket) != NULL && table->nodes_tbl->at(bucket)->hashcode == hashcode){ //if not null
            //will *not insert already used term in new location,
            //unless new translation pair is found
            if(table->nodes_tbl->at(bucket)->translated_values[0] != node->translated_values[0]){
                //cout << "Different translation found for word." << endl;
                table->nodes_tbl->at(bucket)->translated_values[0] = node->translated_values[0];
                table->nodes_tbl->at(bucket)->translated_values[1] = node->translated_values[1];
                table->nodes_tbl->at(bucket)->translated_values[2] = node->translated_values[2];
                table->size++;
                return;
            }
            else{
                return;
            }
        }
    bucket++;
    bucket %= table->current_capacity;
    probed++;
    }
    cout << "Table full. Could not add key to table." << endl;
    
    return;
}


void Translation::CreateAllTables(){
    for(int i=0; i<4; i++){ //iterate for each language
    shared_ptr<hash_table> Table = NewTable(97); // 0.73 = 71/97 load factor
    Table->size == 0;
    for (auto w : words_ptrs[i]->words_array){
        MapNode(Table, w, i);
    }
    all_tables[i] = Table;
    //cout << "Table #" << i+1 << " is complete." << endl;
    }
}

string Translation::FindWord(string key, int lang_from, int lang_to){
    unsigned int hashcode = hash_func(key);
    int probed = 0;
    shared_ptr<hash_table> table = all_tables[lang_from];
    int bucket = bucket_func(hashcode, table->current_capacity);

    while(table->nodes_tbl->at(bucket) != NULL){
        if(probed++ > table->current_capacity){
            return "Error. Word not found.";
        }

        if (table->nodes_tbl->at(bucket)->key == key){
            return table->nodes_tbl->at(bucket)->translated_values[lang_to];
        }
        bucket++;
        bucket %= table->current_capacity;
    }
    return "Error. Word not found.";
}


string Translation::Translate(string word, string lang_from, string lang_to){
    if(lang_from == lang_to){
        cout << "Error. Cannot translate between the same language." <<endl;
    }

    //lang_from_ integer
    int lang_from_ = 0;
    if(lang_from == "English" || lang_from == "english"){lang_from_ = ENG;}
    if(lang_from == "French" || lang_from == "french"){lang_from_ = FR;}
    if(lang_from == "Spanish" || lang_from == "spanish"){lang_from_ = SPN;}
    if(lang_from == "Russian" || lang_from == "russian"){lang_from_ = RUS;}
    
    //lang_to_ integer
    int lang_to_ = 0;
    if(lang_to == "English" || lang_to == "english"){lang_to_ = ENG;}
    if(lang_to == "French" || lang_to == "french"){lang_to_ = FR;}
    if(lang_to == "Spanish" || lang_to == "spanish"){lang_to_ = SPN;}
    if(lang_to == "Russian" || lang_to == "russian"){lang_to_ = RUS;}

    string translated = FindWord(word, lang_from_, lang_to_);
    if(translated != "Error. Word not found."){
        cout << endl << "Here is your translation!" << endl;
        cout << lang_from << ": " << word << endl;
        cout << lang_to << ": " << translated << endl;
    }else{
        cout << endl << "Error. Please try a different word." << endl;
    }
    return translated;
}

