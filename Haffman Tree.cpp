#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <wchar.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

using namespace std;

wifstream fin("input.txt", ios::binary);
wofstream fout("output.txt", ios::binary);



struct vertex{
    int f;
    int left;
    int right;
    int z;
};

map<wchar_t, wstring> code_table;
vector <wchar_t> symb;
vector <vertex> tree;

void code(int ind, wstring bin){
    if (tree[ind].f){
        code_table[symb[ind]] = bin;
        return;
    }
    code(tree[ind].left, bin+L"1");
    code(tree[ind].right, bin+L"0");
    return;

}

int main()
{
    setlocale(LC_ALL, "");

    fin.imbue(locale(fin.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));
    fout.imbue(locale(fout.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));
    
    string command = "";
    cin >> command;
    if (command == "code"){
        map <wchar_t, int> data;
        wstring start = L"";
        
        wchar_t a;
        while (fin.get(a)){
            if (a == 13){
                continue;
            }
            start += a;
            if (data[a] == 0){
                symb.push_back(a);
            }
            data[a]+=1;
        }
        
        
        set <pair<int, int>> julia;
        tree.resize(2*symb.size() - 1);
        
        for (int i = 0; i < symb.size(); i++){
            tree[i] = vertex{1, -1, -1, data[symb[i]]};
            julia.insert(make_pair(tree[i].z, i));
        }
        
        int ind = symb.size(), cnt = symb.size();
        
        while (cnt > 1){
            
            set<pair<int, int>>::iterator it = julia.begin();
            
            pair <int, int> a = *it;
            julia.erase(it);
            
            it = julia.begin();
            pair<int, int> b = *it;
            julia.erase(it);
            
            tree[ind] = vertex{0, min(a.second, b.second), max(a.second, b.second), a.first + b.first};
            julia.insert(make_pair(a.first + b.first, ind));
            
            cnt-=1;
            ind += 1;
        }
        
        
        
        code(2*symb.size()-2, L"");
        
        for (int i = 0; i < symb.size(); i++){
            if (symb[i] == 10){
                fout << L"[new_line]" << " " << code_table[symb[i]] << endl;
            }
            else if (symb[i] == 32){
                fout << L"[space]" << " " << code_table[symb[i]] << endl;
            }
            else{
                fout << symb[i] << " " << code_table[symb[i]] << endl;
            }
            
        }
        for(int i = 0; i < start.size(); i++){
            fout << code_table[start[i]];
        }
    }
    else if (command == "decode"){
        map<wstring, wchar_t> code_table;
        wstring a = L"";
        while (getline(fin, a)){
            int ind = a.find(' ');
            if (ind == -1){
                break;
            }
            else if (a.substr(0, ind) == L"[space]"){
                code_table[a.substr(ind+1)] = ' ';
            }
            else if (a.substr(0, ind) == L"[new_line]"){
                code_table[a.substr(ind+1)] = '\n';
            }
            else{
                code_table[a.substr(ind+1)] = a[0];
            }
            
        }
        wstring start = a;
        
        
        wstring symbol = L"";
        for (int i = 0; i < start.size(); i++){
            symbol += start[i];
            
            if (code_table[symbol] != 0){
                fout << code_table[symbol];
                symbol = L"";
            }
        }
    }
    fin.close();
    fout.close();
    return 0;
}
