/*
  problem statement: https://iudex.io/problem/640270368216f900013beb77/statement
*/

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

#define br '\n'

typedef uint32_t uint;
typedef long long ll;

uint rngnxt;
const uint A = 1664525;
const uint C = 1013904223;

void next() { 
    rngnxt = A * rngnxt + C;
}

uint lg(uint a)
{
    uint ans = 0;
    while(a)
    {
        a >>= 1;
        if (a) ans++;
    }
    return ans;
}

class VEB{
public:

    uint u; // universo
    ll mn;
    ll mx;
    VEB* summary;
    vector<VEB*> clusters;

    // raiz (u == 2^2r, r E I)
    uint raiz(uint x)
    {
        uint p = lg(x);
        return (1<<(p/2));
    }

    VEB(uint x)
    {

        u = x;
        mn = x;
        mx = -1;
        if (x == 2){
            summary = nullptr;
            clusters = vector<VEB*> (1,nullptr);    
        }
        else {
            summary = new VEB(raiz(x));
            clusters = vector<VEB*> (raiz(x), nullptr); 
            for (int i = 0; i < raiz(x); i++) clusters[i] = new VEB(raiz(x));
        }
    }

    // hi
    uint hi(uint x)
    {
        return x/raiz(u);
    }

    // lo
    uint lo(uint x)
    {
        return x % raiz(u);
    }
    
    // index
    uint index(uint h, uint l)
    {
        return (h * raiz(u)) + l;
    }

    // insert
    uint insert(uint x) // aparentemente top
    {
        if (x == mn || x == mx) return 0;
        if (u == 2)
        {
            mn = min(mn,(ll)x);
            mx = max(mx,(ll)x);
            return 1;
        }
        if (mn == u) // valor default tem que ser u (invalido) -> implementar no construtor            
        {
            mn = x;
            mx = x;
            return 1;
        }
        if (x < mn){
            uint temp = x;
            x = mn;
            mn = temp;
        }
        mx = max(mx,(ll)x);
        uint h = hi(x);
        uint l = lo(x);
        if (clusters[h]->mn == clusters[h]->u) //  cluster vazio
            summary->insert(h);
        uint d = clusters[h]->insert(l);
        if (d) return d+1;
        else return 0;
    }

    // rem
    uint rem(uint x) // n sei
    {
        if (u == 2)
        {
            bool ismn,ismx; ismn = ismx = 0;
            uint d;
            if (x == mn){
                ismn = 1;
                if (mx != mn) mn = mx;
                else mn = u; // valor impossível
            }
            if (x == mx){
                ismx = 1;
                if (mn != u) mx = mn;
                else mx = -1; // valor impossível
            }
            if (!(ismn) && !(ismx)) d = 0;
            else d = 1;
            return d;
        }
        if (x == mn)
        {
            uint i = summary->mn;
            if (i == summary->u){
                mn = u;
                mx = -1;
                return 1;
            }
            else {
                mn = index(i,clusters[i]->mn);
                x = mn;
            }
        }
        uint h = hi(x);
        uint l = lo(x);
        uint d = clusters[h]->rem(l) + 1;
        if (d == 1) return 0;

        if (clusters[h]->mn == clusters[h]->u) // cluster ficou vazio
            summary->rem(h);
        if (x == mx)
        {
            ll i = summary->mx;
            if (i == -1) mx = mn;
            else mx = index(i,clusters[i]->mx);
        }
        return d;
    }

    // suc
    uint suc(uint x)
    {
        if (u == 2) {
            
            if (x == 0 && mx == 1) {
                return 1;
            }
            else {
                return 2; 
            }
        }
        if (mn != u && x < mn) {
            return mn;
        }
        uint h,l,s;
        h = hi(x);
        l = lo(x);
        if (l < clusters[h]->mx)
        {
            s = clusters[h]->suc(l);
            return index(h,s);
        }
        else
        {
            h = summary->suc(h);
            if (h == summary->u){
                return u;
            }
            else
            {
                s = clusters[h]->mn;
                return index(h,s);
            }
        }
    }

};




int main()
{
    uint S,M,B,N,I,F,D,P,x,U,y,ans;
    cin>>S>>M>>B>>N>>I>>F>>D>>P;

    rngnxt = S;

    U = (1<<(1<<M));
    VEB T(U);

    for (int i = 0; i < B; i++){
        T.insert(rngnxt % U);
        next();
    }

    for (int i = 0 ; i < N; i++)
    {
        x = rngnxt % (I+F+D);
        if (x < I) // ins
        {
            next();
            x = rngnxt % U;
            ans = T.insert(x);
            if (!(i%P)) cout << "I " << ans << br;
        }
        else if (x < I + F) // suc
        {
            next();
            x = rngnxt % U;
            ans = T.suc(x);
            if (!(i%P)) cout << "S " << ans << br;
        }
        else  // del
        {
            next();
            y = rngnxt % U;

            if (T.suc(y) != U) x = T.suc(y);
            else x = y;
            
            ans = T.rem(x);

            if (!(i%P)) cout << "D " << ans << br;
        }
        next();
    }

    return 0;
}
