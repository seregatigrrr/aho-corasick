#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <queue>


struct TVertex{
    int alpha = 0;
    int level = 0;
    int numeric = 0;
    bool terminate = false;
    TVertex* parent = nullptr;
    TVertex* suffLink = nullptr;
    TVertex* up = nullptr;
    std::map<int,TVertex> to;
};

class TTrie{
public:
    TVertex root;

    void Insert(std::string& s){
        static int count = 1;
        std::istringstream ss(s);
        TVertex* curr = &root;
        int alpha;
        while( ss >> alpha){
            curr->to[alpha].parent = curr;
            curr->to[alpha].level = curr->level + 1;
            curr = &curr->to[alpha];
            curr->alpha = alpha;
        }
        curr->numeric = count;
        ++count;
        curr->terminate = true;
    }
    void getSuffLink(TVertex* v){
        if(v == &root || v->parent == &root) {
            v->suffLink = &root;
        }
        else {
            TVertex *p = v->parent->suffLink;
            while (p != &root && p->to.count(v->alpha) == 0) {
                p = p->suffLink;
            }
            if (p->to.count(v->alpha) == 0) {
                v->suffLink = &root;
            } else {
                v->suffLink = &(p->to[v->alpha]);
            }
        }
    }
    void getUp(TVertex* v) {
        if (v == &root) {
            v->up = nullptr;
        } else if (v->suffLink->terminate) {
            v->up = v->suffLink;
        } else {
            v->up = v->suffLink->up;
        }
    }
    TVertex* go(TVertex* state, int alpha){
        if(state->to.count(alpha) > 0){
            return &state->to[alpha];
        }
        else {
            for (; state->to.count(alpha) == 0 && state != &root; state = state->suffLink)
                ;
            if(state->to.count(alpha) > 0){
                return &state->to[alpha];
            }
            else{
                return &root;
            }
        }
    }
};


int main() {
    TTrie trie;
    std::string s;
    std::getline(std::cin,s);
    while(!s.empty()){
        trie.Insert(s);
        std::getline(std::cin, s);
    }

    std::queue<TVertex*> q;
    q.push(&trie.root);

    while(!q.empty()){
        TVertex* temp =  q.front();
        q.pop();
        trie.getSuffLink(temp);
        trie.getUp(temp);

        for(std::pair<const int, TVertex>& elem: temp->to){
            q.push(&(elem.second));
        }
    }

    TVertex* state = &trie.root;
    int pos,currLine = 0;
    int alpha;
    std::vector<int> nSymbols;

    while(std::cin){
        std::getline(std::cin,s);
        ++currLine;
        if(s.empty()) break;
        std::istringstream ss(s);
        pos = 0;
        while(ss >> alpha){
            ++pos;
            state = trie.go(state, alpha);
            if(state->up){
                TVertex* up = state->up;
                while(up){
                    int line = currLine;
                    int nSym = up->level - pos;
                    while (nSym > 0) {
                        --line;
                        nSym -= nSymbols[line - 1];
                    }
                    std::cout << line << ", " << abs(nSym) + 1 << ", " << up->numeric << '\n';
                    up = up->up;
                }
            }
            if(state->terminate) {
                int line = currLine;
                int nSym = state->level - pos;
                while (nSym > 0) {
                    --line;
                    nSym -= nSymbols[line - 1];
                }
                std::cout << line << ", " << abs(nSym) + 1 << ", " << state->numeric << '\n';
            }
        }
        nSymbols.push_back(pos);
    }
    return 0;
}