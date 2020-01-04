#include "Translation.h"

using namespace std;

int main() {
    Translation translate;
    translate.store_words("Text_files/English.txt", "English");
    translate.store_words("Text_files/French.txt", "French");
    translate.store_words("Text_files/Spanish.txt", "Spanish");
    translate.store_words("Text_files/Russian.txt", "Russian");
    
    translate.CreateAllTables();

    string input, language_from, language_to;

    cout<< "examples you may try: hello, computer science, fútbol, сегодня" << endl;
    cout << "Welcome to the translator" << endl;
    cout << "Language options: English, French, Spanish or Russian" << endl;
    
    cout << endl << "What word or phrase would you like to translate?" << endl <<
    "Please enter all lowercase letters and accents, if necessary." << endl;
    getline(cin, input);

    cout << endl << "Which language is this word in?" << endl;
    cin >> language_from;

    cout << endl << "Which language would you like to translate this to?" << endl;
    cin >> language_to;

    translate.Translate(input, language_from, language_to);
    cout << endl;

    return 0;
}
