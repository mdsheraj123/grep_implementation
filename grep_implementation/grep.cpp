/* author: Mohammad Sheraj */
/* email: mdsheraj123@gmail.com */
/* Implementation of grep */
///////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
///////////////////////////////////////////////////////////////

// Fills lps[] for given patttern pat[0..M-1] 
void computeLPSArray(string pat, int M, int* lps) 
{ 
    // length of the previous longest prefix suffix 
    int len = 0; 
  
    lps[0] = 0; // lps[0] is always 0 
  
    // the loop calculates lps[i] for i = 1 to M-1 
    int i = 1; 
    while (i < M) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else // (pat[i] != pat[len]) 
        { 
            // This is tricky. Consider the example. 
            // AAACAAAA and i = 7. The idea is similar 
            // to search step. 
            if (len != 0) { 
                len = lps[len - 1]; 
  
                // Also, note that we do not increment 
                // i here 
            } 
            else // if (len == 0) 
            { 
                lps[i] = 0; 
                i++; 
            } 
        } 
    } 
} 

  
// Prints occurrences of txt[] in pat[] 
bool KMPSearch(string pat, string txt, bool w_flag) 
{ 
    int M = pat.size(); 
    int N = txt.size(); 
  
    // create lps[] that will hold the longest prefix suffix 
    // values for pattern 
    int lps[M]; 
  
    // Preprocess the pattern (calculate lps[] array) 
    computeLPSArray(pat, M, lps); 
  
    int i = 0; // index for txt[] 
    int j = 0; // index for pat[] 
    while (i < N) { 
        if (pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
  
        if (j == M) { 
            // printf("Found pattern at index %d ", i - j); 
            if(w_flag) {
                int starts = i-j;
                int last = starts+pat.size()-1;
                // cout<<endl<<starts<<" "<<last<<endl;
                bool should_return_true = true;
                if(starts-1>=0) {
                    char temp = tolower(txt[starts-1]);
                    if(temp=='_'||(temp>='0'&&temp<='9')||(temp>='a'&&temp<='z')) {
                        should_return_true = false;
                    }
                }
                if(last+1<txt.size()) {
                    char temp = tolower(txt[last+1]);
                    if(temp=='_'||(temp>='0'&&temp<='9')||(temp>='a'&&temp<='z')) {
                        should_return_true = false;
                    }
                }
                if(should_return_true) {
                    return true;
                }
            } else {
                return true;
            }
            j = lps[j - 1]; 
        } 
  
        // mismatch after j matches 
        else if (i < N && pat[j] != txt[i]) { 
            // Do not match lps[0..lps[j-1]] characters, 
            // they will match anyway 
            if (j != 0) 
                j = lps[j - 1]; 
            else
                i = i + 1; 
        } 
    } 
    return false;
} 
  
int main() {
    cout<<"Please enter the grep command"<<endl;
    string command;
    getline(cin, command);
    
    if(command.size()<4||command.substr(0,4)!="grep") {
        cout<<"Did you mean 'grep'?"<<endl;
    } else {
        
        bool v_flag = false; //invert
        bool i_flag = false; //ignore case
        bool w_flag = false; //regexp
        bool n_flag = false; //line number
        string to_search;
        string input_file;

        int at = 4;
        while(at<command.size()&&command[at]==' ') {
            at++;
        }
        if(at==command.size()) {
            cout<<"Please enter command in the form 'grep -flags pattern filename'"<<endl;
            return 0;
        }
        if(command[at]=='-') {
            at++;
            while(at<command.size()&&command[at]!=' ') {
                if(command[at]=='v') {
                    v_flag = true;
                } else if(command[at]=='i') {
                    i_flag = true;
                } else if(command[at]=='w') {
                    w_flag = true;
                } else if(command[at]=='n') {
                    n_flag = true;
                }
                at++;
            }
            while(at<command.size()&&command[at]==' ') {
                at++;
            }
        }
        int start = at;
        while(at<command.size()&&command[at]!=' ') {
            at++;
        }
        if(start<command.size()) {
            to_search = command.substr(start,at-start);
        } else {
            cout<<"Please enter command in the form 'grep -flags pattern filename'"<<endl;
            return 0;
        }
        // cout<<to_search<<endl;
        while(at<command.size()&&command[at]==' ') {
            at++;
        }
        start = at;

        if(start<command.size()) {
            input_file = command.substr(start,command.size()-start);
        } else {
            cout<<"Please enter command in the form 'grep -flags pattern filename'"<<endl;
            return 0;
        }

        int temp_at = input_file.size()-1;
        while(temp_at<input_file.size()&&input_file[temp_at]==' ') {
            input_file.erase(input_file.begin()+temp_at);
            temp_at--;
        }

        cout<<endl<<"Result:"<<endl;

        ifstream myfile (input_file);
        if (myfile.is_open()) { //No file opening errors
            string backup_line;
            int line_number = 0;
            while ( getline (myfile,backup_line) ) { //Takes input till EOF
                line_number++;
                string line = backup_line;
                if(i_flag) {
                    for(int i=0;i<line.size();i++) {
                        line[i] = tolower(line[i]);
                    }
                }
                bool to_print = KMPSearch(to_search, line, w_flag); //This is O(n) search algo

                if(v_flag) {
                    to_print = !to_print;
                }

                if(to_print) {
                    if(n_flag) {
                        cout<<line_number<<":";
                    }
                    cout << backup_line << endl;
                }
            }
            myfile.close(); //Resources freed
        } else {
            cout << "Unable to open file, it's possible that it does not exist in this directory."<<endl;
        }
    }
    return 0; //successful execution
}