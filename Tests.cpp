#include "Translation.h"

using namespace std;

int main() {
    cout << endl << "-----Running tests-----" << endl;
    Translation test;
    test.store_words("Text_files/English.txt", "English");
    test.store_words("Text_files/French.txt", "French");
    test.store_words("Text_files/Spanish.txt", "Spanish");
    test.store_words("Text_files/Russian.txt", "Russian");
    
    //Test 1: Check that words from each language are in corresponding array
    /*cout << test.words_ptrs[0]->language << " - "<< test.words_ptrs[0]->words_array[0] << endl; 
    cout << test.words_ptrs[1]->language << " - " << test.words_ptrs[1]->words_array[0] << endl;
    cout << test.words_ptrs[2]->language << " - "<< test.words_ptrs[2]->words_array[0] << endl;
    cout << test.words_ptrs[3]->language << " - "<< test.words_ptrs[3]->words_array[0] << endl;
    */
    if(test.words_ptrs[0]->words_array[0] == "hello" 
    && test.words_ptrs[1]->words_array[0] == "bonjour" 
    && test.words_ptrs[2]->words_array[0] == "hola" 
    && test.words_ptrs[3]->words_array[0]=="здравствуйте"){
        cout << "[PASSED]---store_words()---" << endl;
    }else{cout << "[~FAILED]---store_words()---" << endl;}

    //Test 2: Check that nodes are initialized correctly
    shared_ptr<hash_node> node(new hash_node);
    node = test.NewNode("russian", 0, 12345);
    //cout << "English: " << node->key << endl;
    //cout << "Russian: " << node->translated_values[2] << endl;
    if(node->key == "russian" && node->translated_values[3] == "русский"){
        cout << "[PASSED]---NewNode()---" << endl;
    }else{cout << "[~FAILED]---NewNode()---" << endl;}
    
    //Check: JupyterLab uses 2 bytes for UTF-8 characters an 1 for ASCII
    //string in = "иe3";
    //cout << "String " << in << " has size " << in.length() << endl;
    
    //Test 3:
   shared_ptr<hash_table> table = test.NewTable(97);
   for (auto w : test.words_ptrs[0]->words_array){
       test.MapNode(table, w, 0);
   }
    /*for (int i=0; i<97; i++){
        if(tbl->nodes_tbl->at(i) == NULL){
            cout << "<> Bucket is empty at " << i << endl;
        }
        else{
            cout << tbl->nodes_tbl->at(i)->key << "----key at bucket " << i << endl;
        }
    }
    */
    //cout << table->size << " and " << table->current_capacity << endl;
    if (table->size == 71){ //English Table
        cout << "[PASSED]---MapNode()---" << endl;
    }else{cout << "[~FAILED]---MapNode()---" << endl;}
    
    //Test 4: See if all_tables can access each languages table and translations
    test.CreateAllTables();
    //cout << "English: " << test.all_tables[0]->nodes_tbl->at(38)->key << endl;
    //cout << "Spanish: " << test.all_tables[0]->nodes_tbl->at(38)->translated_values[1] << endl;
    //cout << test.all_tables[1]->size << endl ;

    if(test.all_tables[0]->nodes_tbl->at(38)->key == "go" && 
    test.all_tables[0]->nodes_tbl->at(38)->translated_values[2] == "ir"){
        cout << "[PASSED]---CreateAllTables()---" << endl;
    }else{cout << "[~FAILED]---CreateAllTables()---" << endl;}

    //Test 5: Ensure input is being translated correctly
    string answer;
    answer = test.FindWord("soccer", ENG, SPN);
    if(answer == "fútbol"){
        cout << "[PASSED]---Translate()---" << endl;
    }else{cout << "[~FAILED]---Translate()---" << endl;}

    cout << "-----All tests are complete-----" << endl << endl;

}
