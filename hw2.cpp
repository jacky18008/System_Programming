#include<iostream>
#include<cstring>
#include<string>
#include<sstream>
#include<cmath>
#include<vector>
#include<cstdio>
#include<cstdlib>

using namespace std;

string Hex[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
char Hex1[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
string opTable[] =
{"ADD","ADDF", "ADDR", "AND", "CLEAR" ,"COMP","COMPF","COMPR", "DIV","DIVF","DIVR", "FIX","FLOAT",
    "HIO",
    "J",
    "JEQ",
    "JGT",
    "JLT",
    "JSUB",
    "LDA",
    "LDB",
    "LDCH",
    "LDF",
    "LDL",
    "LDS",
    "LDT",
    "LDX",
    "LPS",
    "MUL",
    "MULF",
    "MULR",
    "NORM",
    "OR",
    "RD",
    "RMO",
    "RSUB",
    "SHIFTR",
    "SIO",
    "SSK",
    "STA",
    "STB",
    "STCH",
    "STF",
    "STI",
    "STL",
    "STS",
    "STSW",
    "STT",
    "STX",
    "SUB",
    "SUBF",
    "SUBR",
    "SVC",
    "TD",
    "TIO",
    "TIX",
    "TIXR",
    "WD"};

vector<string> optable(opTable, opTable+59);



class SymTab{
    public:
    SymTab(string loc, string symbol);
    bool InList(string check);
    void insert(SymTab* s);
    void print();
    private:
    string location;
    string label;
    SymTab *nextPtr;
};

SymTab::SymTab(string loc, string symbol){
    location = loc;
    label = symbol;
    nextPtr = NULL;
}

bool SymTab::InList(string check){
    SymTab *arrow = this;
    while(arrow != NULL){
        if(check == arrow->label){
            return true;
        }
        arrow = arrow->nextPtr;
    }
    return false;
}

void SymTab::insert(SymTab* s){
    SymTab *arrow = this;
    while(arrow->nextPtr != NULL){
        arrow = arrow->nextPtr;
    }
    arrow->nextPtr = s;
}

void SymTab::print(){
    SymTab *arrow = this;
    while(arrow != NULL){
        cout<<arrow->label<<" "<<arrow->location<<endl;
        arrow = arrow->nextPtr;
    }
}

void OutPut(string a, string b, string c, int space, int Now);
string DeciToHex(int a);
bool InOpTable(string s);
int HexToDeci(string s);

int main(){
    int i;
    string s;
    int NowAddress = 0;
    SymTab *firstPtr = NULL;
    
    while(getline(cin, s)){
        
        int num_of_space = 0;
        for(int i = 0; i < s.size(); i++) {
            if(s[i] == 32){
                num_of_space++;
            }
        }
        string symbol = "", op = "", operand = "";
        
        
        stringstream stream(s);
        if(num_of_space == 0){
            stream>>op;
        }
        
        else if(num_of_space == 1){
            getline(stream, op, ' ');
            getline(stream, operand, ' ');
        }
        
        else if(num_of_space >= 2){
            getline(stream, symbol, ' ');
            getline(stream, op, ' ');
            getline(stream, operand, ' ');
        }
        /* start to do pass1 */
        int StartAddress;
        int Loct;
        if(op == "START"){
            StartAddress = HexToDeci(operand);
            NowAddress = StartAddress;
            OutPut(symbol, op, operand, num_of_space, NowAddress);
            firstPtr = new SymTab(DeciToHex(NowAddress), symbol);
        }
        else if(op == "END"){
            OutPut(symbol, op, operand, num_of_space, NowAddress);
            break;
        }
        else{
            if(symbol[0] != '.' && op[0] != '.' && operand[0] != '.'){/* not comment */
                if(symbol != ""){
                    if(firstPtr->InList(symbol)){
                        cout<<"ERROR DUPSYM: ";
                        if(num_of_space >= 2){
                            cout<<symbol<<" ";
                        }
                        if(num_of_space >= 0){
                            cout<<op<<" ";
                        }
                        if(num_of_space >= 1){
                            cout<<operand<<" ";
                        }
                        cout<<endl;
                        return 0;
                        
                    }
                    else{
                        SymTab *s = new SymTab(DeciToHex(NowAddress), symbol);
                        firstPtr->insert(s);
                    }
                }
                if(InOpTable(op)){
                    if(op[0] == '+'){
                        Loct = 4;
                    }
                    else if(op == "CLEAR" || op == "COMPR" || op == "TIXR"){
                        Loct = 2;
                    }
                    
                    else{
                        Loct=3;
                    }
                }
                else if(op == "WORD"){
                    Loct=3;
                }
                else if(op == "RESW"){
                    Loct=3*atoi(operand.c_str());
                }
                else if(op == "RESB"){
                    Loct=atoi(operand.c_str());
                }
                else if(op == "BYTE"){//Do Byte
                    int k, mark = 0;
                    string ref = "";
                    for(k=0;k<operand.size();k++){
                        if(operand[k] == 39){
                            ++mark;
                        }
                        if(mark == 1 && operand[k] != 39){
                            ref=ref+operand[k];
                        }
                    }
                    int s = HexToDeci(ref);
                    if(s != -1){
                        Loct = (s/(pow(16, 8))) + 1;
                    }
                    else{
                        Loct = 3;
                    }
                    
                }
                else if(op == "BASE"){
                    Loct = 0;
                }
                else{
                    cout<<"ERROR INVALIDOP: ";
                    if(num_of_space >= 2){
                        cout<<symbol<<" ";
                    }
                    if(num_of_space >= 0){
                        cout<<op<<" ";
                    }
                    if(num_of_space >= 1){
                        cout<<operand<<" ";
                    }
                    cout<<endl;
                    return 0;
                }
                OutPut(symbol, op, operand, num_of_space, NowAddress);
                
                NowAddress+=Loct;
            }
            else{
                cout<<DeciToHex(NowAddress)<<" "<<s<<endl;
            }
        }
    }
    cout<<"=========="<<endl;
    firstPtr->print();
    return 0;
}

void OutPut(string a, string b, string c, int space, int Now){
    cout<<DeciToHex(Now)<<" ";
    
    if(space >= 2){
        cout<<a<<" ";
    }
    if(space >= 0){
        cout<<b<<" ";
    }
    if(space >= 1){
        cout<<c<<" ";
    }
    cout<<endl;
}

string DeciToHex(int a){
    string sum, hold;
    int i;
    for(i=3; i>=0; i--){
        hold = Hex[(int)(a/pow(16, i))];
        a -= (int)(a/pow(16, i))*pow(16, i);
        sum = sum+hold;
    }
    return sum;
}

bool InOpTable(string s){
    int i;
    if(s[0] == '+'){
        string k = "";
        for(i=0;i<s.size();i++){
            if(i>0){
                k = k+s[i];
            }
        }
        s = k;
    }
    for(i = 0; i<optable.size(); i++){
        if(s == optable[i]){
            return true;
        }
    }
    return false;
}

int HexToDeci(string s){
    int sum = 0, i, j;
    for(i=0;i<s.size();i++){
        if(s[i]>70){
            return -1;
        }
    }
    for(i=0;i<s.size();i++){
        int t;
        for(j=0;j<16;j++){
            if(Hex1[j] == s[i]){
                t = j;
            }
        }
        sum+=pow(16,s.size()-1-i)*t;
    }
    return sum;
}
