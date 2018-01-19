#include<fstream>
#include<iostream>
#include<stack>
#include<bitset>
#include<vector>
#include<typeinfo>
#include <cstdlib>
using namespace std;


/* DFA'S & LANGUAGE COMPONENTS */

string  keywords[] = {"#start", "#end", "int","float","double","bool"
                        ,"eq","gr","gre","lse","ls"
                        ,"nq","and","or","if","else","endif","loop","endloop","break",};

string special[] = {"*", "/", "+", "-", "(", ")", ";", "=", ":"};

int id[4][2] = {{-1,1},{2,3},{2,3},{2,3}};

int num[5][3] = {   //  digit|  - | .
                    {  2 ,  1 , -1 },
                    {  2 , -1 , -1 },
                    {  2 , -1 ,  3 },
                    {  4 , -1 , -1 },
                    {  4 , -1 , -1 }
                };

/* PREDICTIVE PARSING COMPONENTS */

string terminals[] =     {"#start", "#end", "id", ";", "int","float","double","bool","*","/","+","-","(",")","num","eq","gr","gre","lse","ls","nq","and","or","if","else","endif","loop","endloop","break", "$"};

string non_terminals[] = {"W","S","A","D","E","X","T","Y","F","R","C","M","U","I","H","L","Q"};

string table[17][29] = {
        {"#start S #end" , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , "0", "A S"     , " ", "D A S", "D A S", "D A S" , "D A S", " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , "I S"           , "0"             , "0"    , "L S"                , "0"       , "0"               },
        {" "             , " ", "id = E ;", " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , " ", "int"  , "float", "double", "bool" , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", "T X"     , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , "T X" , "T X"     , " ", "T X"    , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , "0", " "    , " "    , " "     , " "    , "* E" , "/ E" , " "   , " "   , " "       , "0", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", "F Y"     , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , "F Y" , "F Y"     , " ", "F Y"    , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , "0", " "    , " "    , " "     , " "    , "0"   , "0"   , "+ T" , "- T" , " "       , "0", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", "R"       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , "- F" , "R"       , " ", "R"      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", "id"      , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , "( E )"   , " ", "num"    , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", "R M R U" , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , "R M R U" , " ", "R M R U", " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , "eq"  , "gr", "gre", "lse", "ls", "nq", " "       , " "   , " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , "0", " "      , " "   , " " , " "  , " "  , " " , " " , "and C"   , "or C", " "             , " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , "if ( C ) : S H", " "             , " "    , " "                  , " "       , " "               },
        {" "             , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , "else : S endif", "endif", " "                  , " "       , " "               },
        {" "             , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , "loop ( C ) : S Q"   , " "       , " "               },
        {" "             , " ", " "       , " ", " "    , " "    , " "     , " "    , " "   , " "   , " "   , " "   , " "       , " ", " "      , " "   , " " , " "  , " "  , " " , " " , " "       , " "   , " "             , " "             , " "    , " "                  , "endloop" , "break ; endloop" }

};

string input_now;

/* PROTOTYPES */

int check(string c);
int which_char(char c);
int which_id(char c);
int get_index(string s[], string letter, int size);
void lexical(string input_string);
void print(std::vector<string> vec, string name);
void predictive_parse(std::vector<string> input, bool tree);
std::vector<string> parse (string s);

/* GLOBAL DECLARATIONS */

int i = 0;
int j = 0;
int line = 0;
int correctness_flag = 0;
int input_counter = 1;
string input_token;
string cross;
std::stack<string> stk;
std::vector<string> buffer;
std::vector<string> tokens;
std::vector<int> id_indices;
std::vector<int> num_indices;
std::vector<string> input_tokens;
std::vector<string> keywords_tokens;
std::vector<string> special_tokens;
std::vector<string> identifiers;
std::vector<string> numbers;
std::vector<string> Errors;


/* MAIN */

int main(){

    // reading input txt file
    ifstream INPUT;

    // default mode ios::in
    INPUT.open("program.txt");  

    if (INPUT.fail()) {
        cerr << "error: open file for input failed!" << endl;
        abort();
    }

    string line_;
    string data = "";
    
    while (!INPUT.eof()) {  
        INPUT >> line_;
        data += line_+" ";
    }
    INPUT.close();

    string input = data;
     
    cout << "## LEXICAL ANALYSIS ##\n\n";
    // Analyze input
    lexical(input);

    print(keywords_tokens, "KEYWORDS");
    print(special_tokens, "SPECIAL CHARACTERS");
    print(identifiers, "IDENTIFIERS");
    print(numbers, "NUMBERS");
    print(Errors, "ERRORS");


    // Replace each number with the word "num"
    for(int n = 0; n < num_indices.size(); n++)
        input_tokens[num_indices[n]] = "num";

    // Replace each identifier with the word "id"
    for(int n = 0; n < id_indices.size(); n++)
        input_tokens[id_indices[n]] = "id";

    // push for parsing
    input_tokens.push_back("$");

    // if no errors in lexical analysis
    if(Errors.empty()) {

        // check for correctness according to the grammar
        cout << "\n\n";
        cout << "## PREDICTIVE PARSING ##\n\n";
        predictive_parse(input_tokens, false);

        if(correctness_flag)
            // print the parse tree
            predictive_parse(input_tokens, true);

    }

}


/* FUNCTIONS */

// check whether the string keyword or special character
int check(string c)
{
    int size = sizeof(keywords) / sizeof(keywords[0]);
    int i = 0;

    while(i < size){

        if(c == keywords[i])
            return 1;

        else if(i < 9 && c == special[i])
            return 2;

        i++;
    }

    return 0;

}

// return index according to numbers' dfa indexing
int which_char(char c)
{
    if(isdigit(c))
        return 0;
    else if(c == '.')
        return 2;
    else
        return 1;
}

// return index according to identifiers' dfa indexing
int which_id(char c)
{
    if(isdigit(c))
        return 0;
    else if(isalpha(c))
        return 1;
}

// remove spaces and separate words
std::vector<string> parse (string s)
{
    string delimiter = " ";         //delimiting input string using spaces
    size_t pos = 0;
    string token;
    std::vector<string> tokens;      //store delimited tokens

    while((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        tokens.push_back(token);
        input_counter++;
        s.erase(0, pos + delimiter.length());

    }

    tokens.push_back(s);
    return tokens;
}

// print a vector of tokens
void print(std::vector<string> vec, string name)
{
    cout << name << endl;

    for(int i = 0; i < vec.size(); i++)
        cout << vec[i] << endl;

    cout << "--------------------\n";
}

// get index of a terminal or non terminal
int get_index(string s[], string letter, int size)
{

    for (int i = 0; i < size; i++){

        if (letter == s[i])
            return i;

    }
    return -1;
}

// perform lexical analysis
void lexical(string input_string)
{

    int state_num = 0;
    int state_id = 0;
    int flag;

    input_tokens = parse(input_string);


    for(int i = 0; i < input_tokens.size(); i++){

        flag = check(input_tokens[i]);

        if(flag == 1)
            keywords_tokens.push_back(input_tokens[i]);


        else if(flag == 2)
            special_tokens.push_back(input_tokens[i]);


        else
        {

            for(int j = 0; j < input_tokens[i].size(); j++) {
                if(state_num != -1)
                    state_num = num[state_num] [which_char(input_tokens[i][j])];
                if(state_id != -1)
                    state_id = id[state_id] [which_id(input_tokens[i][j])];
            }

            if(state_num ==  2 || state_num == 4) {
                numbers.push_back(input_tokens[i]);
                //for converting each num to its original number
                num_indices.push_back(i);
            }

            else if(state_id == 1 || state_id == 2 || state_id == 3) {
                identifiers.push_back(input_tokens[i]);
                // for converting each id to its original identifier
                id_indices.push_back(i);
            }


            else if(input_tokens[i] != "")
                Errors.push_back(input_tokens[i]);

            state_id = 0;
            state_num = 0;

        }

    }

}

// perform predictive parsing
void predictive_parse(std::vector<string> input , bool tree)
{
    int m = 0;
    int l = 0;

    if(tree){
        i = 0;
        j = 0;
        input_counter = 1;
        cout << "PARSE TREE \n\n";
    }


    tokens = input; // remove spaces

    stk.push("$");
    stk.push(non_terminals[0]);     //push root

    if(tree) {
        cout << "                         " << stk.top() << endl;
        cout << "           -----------------------------\n";
    }


    while(input_counter)
    {

        input_token = tokens[j];    // iterating through input string

        if(!tree) {
            
            for(i=j;i<tokens.size();i++){
                input_now += tokens[i]+" ";
            }
            cout << "----------input now---------- \n" << input_now << "\n-----------------------------\n" <<endl;
            input_now = "";
            cout << "input is \"" << input_token << "\"\n";
            cout << "top of stack is  \"" << stk.top() << "\"\n\n";
        }

        while(get_index(non_terminals, stk.top(), 17) != -1){       //while it's non_terminal

            cross = table [get_index(non_terminals, stk.top(), 17)][get_index(terminals, input_token, 29)];     //check the value resulted from input and top of stack

            stk.pop();

            if(cross.length() > 1){

                buffer = parse(cross);      // remove spaces

                if(tree)
                    cout << "                ";

                for(int i = buffer.size() - 1; i >= 0; i--)
                {
                    stk.push(buffer[i]);    //push one after the other

                    if(tree && buffer[i] != "0")
                        if(buffer[i] == "id") {
                            cout << identifiers[m] << "   ";
                            m++;
                        }
                        else if(buffer[i] == "num"){
                            cout << numbers[l] << "   ";;
                            l++;
                        }
                        else{
                            cout << buffer[buffer.size() - i - 1] << "   ";
                        }

                }

                if(tree)
                    cout << endl;

            } else{

                if(tree && buffer[i] == "0"){
                    if(buffer[i] == "id") {
                        cout << "       ";
                        cout << identifiers[m] << endl;
                        m++;
                        line = 1;
                    }
                    else if(buffer[i] == "num"){
                        cout << "       ";
                        cout << numbers[l] << endl;
                        l++;
                        line = 1;

                    }
                    else{
                        cout << "       ";
                        cout << cross << endl;
                        line = 1;
                    }
                }

                stk.push(cross);
            }

            if(tree && !line && cross.length() > 1)
                cout << "           -----------------------------\n";
        }

        // if match, move the input pointer
        if(input_token == stk.top() && stk.top() != "$"){

            input_counter--;
            j++;
            stk.pop();
            continue;

        }
            //else it's epsilon, pop
        else if(stk.top() == "0"){

            stk.pop();
        }
            //else it's end of stack, success
        else if(stk.top() == "$"){

            stk.pop();
            if(!tree) {
                cout << "SUCCESS: Expression is correct \n\n\n\n";
                correctness_flag = 1;
            }
            break;
        }

        else{
            if(!tree)
                cout << "SYNTAX ERROR \n";
            input_counter--;
            j++;
            break;
        }
    }

}

