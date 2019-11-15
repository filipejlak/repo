#include<iostream>
#include<string>
#include<windows.h>
#include<cmath>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<fstream>
using namespace std;

int maxi(vector<double> scor)
{
    int ret=0;
    for(int i=1; i<scor.size(); i++)
    {
        if(scor[i]>scor[ret])
            ret=i;
    }
    return ret;
}

vector<int> order(vector<double> s, int numb, double sl)
{
    vector<double> div;
    for(int i=0;i<s.size();i++)
        div.push_back(1);
    vector<double> scor = s;
    vector<int> ret;
    for(int i=0; i<numb; i++)
    {
        int k = maxi(scor);
        ret.push_back(k);
        div[k]+=(1+sl);
        scor[k]=s[k]/div[k];
    }
    return ret;
}

void rt(vector<int> taken)
{
    for(int i=0; i<taken.size(); i++)
        cout << taken[i] << endl;
    cout << endl;
}

vector<int> gen(vector<vector<int>> pref, vector<int> ord, vector<int> taken, int n)
{
    for(int i=0;i<n;i++)
    {
        int k=0;
        while(taken[pref[ord[i]][k]]!=-1)
            k++;
        taken[pref[ord[i]][k]]=ord[i];
    }
    return taken;
}

bool improv(vector<int> taken, vector<int> new_taken, vector<int> pref, int n, int p)
{
    for(int i = 0; i < n; i++)
    {
        if(taken[pref[i]]==p && new_taken[pref[i]]!=p)
            return 0;
        else if(taken[pref[i]]!=p && new_taken[pref[i]]==p)
            return 1;
    }
    return 0;
}

bool reason(vector<int> taken, vector<int> pref,int mid, int p, int n)
{
    bool first = 0;
    for(int i=mid+1;i<n;i++)
    {
        if(taken[pref[i]]==p)
        {
            first = 1;
            break;
        }
    }
    return first;
}

vector<int> randpref(int n)
{
    vector<int> ret;
    vector<int> s;
    for(int i=0;i<n;i++)
    {
        s.push_back(i);
    }
    while(s.size())
    {
        int i = rand()%s.size();
        ret.push_back(s[i]);
        s.erase(s.begin()+i);
    }
    return ret;
}

double compr(vector<int> a, vector<int> b, vector<vector<int>> orig_pref, vector<double> scor)
{
    double balance = 0;
    for(int i = 0; i < scor.size(); i++)
    {
        for(int j = 0; j < orig_pref[i].size(); j++)
        {
            if(a[orig_pref[i][j]]==i && b[orig_pref[i][j]]!=i)
            {
                balance+=scor[i];
                break;
            }
            else if(a[orig_pref[i][j]]!=i && b[orig_pref[i][j]]==i)
            {
                balance-=scor[i];
                break;
            }
        }
    }
    return balance;
}

main()
{
    srand(time(NULL));
    vector<double> scor = {2276,1098,861,469,250};
    bool penrose = 1;
    double sainte_lague = 1;
    if(penrose)
    {
        for(int i = 0; i < scor.size(); i++)
            scor[i]=sqrt(scor[i]);
    }
    int n=18;
    vector<string> part = {"A","B","C","D","E"};
    vector<string> mini = {"Administracja","Cyfryzacja","Edukacja","Energia","Finanse","Gospodarka","Infrastruktura","Kultura","Nauka","Obrona",
                            "Rolnictwo","Sport","Sprawiedliwosc","Sprawy spoleczne","Sprawy wewnetrzne","Sprawy zagraniczne","Srodowisko","Zdrowie"};
//    vector<vector<int>> pref = { {4,2,6,9,12,15,1,14,16,17,11,3,8,0,10,13,7,5},
//                                 {6,10,17,14,16,3,8,5,13,11,15,4,1,9,0,2,7,12}
//                                };
    vector<vector<int>> pref;
    for(int i=0;i<scor.size();i++)
        pref.push_back(randpref(n));
    vector<int> ord = order(scor,n,sainte_lague);
    vector<vector<int>> orig_pref = pref;
    vector<int> empt;
    for(int i=0; i<n; i++)
        empt.push_back(-1);
    vector<int> taken = gen(pref,ord,empt,n);
    vector<vector<int>> allt;
    allt.push_back(taken);
    vector<vector<vector<int>>> allp;
    allp.push_back(pref);
    bool change = 1;
    int cnt = 0;
    while(change && cnt < 72)
    {
        change = 0;
        for(int i=scor.size()-1;i>=0;i--)
        {
            int j = 0;
            while(taken[pref[i][j]]!=i)
                j++;
            while(j<n)
            {
                if(taken[pref[i][j]]!=i && reason(taken,orig_pref[i],j,i,n))
                {
                    for(int w = j-1;w>=0;w--)
                    {
                        vector<vector<int>> tmp_pref = pref;
                        int tmp = tmp_pref[i][j];
                        tmp_pref[i][j]=tmp_pref[i][w];
                        tmp_pref[i][w]=tmp;
                        vector<int> new_taken = gen(tmp_pref,ord,empt,n);
                        if(improv(taken,new_taken,orig_pref[i],n,i))
                        {
                            taken = new_taken;
                            pref = tmp_pref;
                            change = 1;
                            allt.push_back(taken);
                            allp.push_back(pref);
                            break;
                        }
                    }
                }
                j++;
            }
        }
        cnt++;
    }
    if(change)
    {
        vector<vector<int>> lastt;
        int c = allp.size()-1;
        vector<vector<int>> lastp = allp[c];
        c--;
        while(allp[c]!=lastp)
        {
            c--;
        }
        c=allp.size()-1-c;
//        for(int i=0; i<n; i++)
//        {
//            for(int j=allt.size()-c;j<allt.size(); j++)
//            {
//                cout << allt[j][i];
//            }
//            cout << endl;
//        }
        for(int i=allt.size()-c; i<allt.size(); i++)
            lastt.push_back(allt[i]);
        int tmp = 0;
        for(int i=1; i<lastt.size();i++)
        {
            if(compr(lastt[i],lastt[tmp],orig_pref,scor) >= 0)
                tmp=i;
        }
        taken=lastt[tmp];
        pref=allp[allp.size()-c+tmp];
    }
    vector<int> check = empt;
    pref = orig_pref;
    for(int i=0; i < n; i++)
    {
        int p = ord[i];
        int k = 0;
        while(taken[pref[p][k]]!=p || check[pref[p][k]]!=-1)
            k++;
        cout << mini[pref[p][k]] << " - " << part[p] << endl;
        check[pref[p][k]] = 0;
    }
}
