#include <bits/stdc++.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
typedef long long ll;
typedef unsigned long long ull;
#define task "digits"
#define forinc(a,b,c) for(int a=b,_c=c;a<=_c;++a)
#define fordec(a,b,c) for(int a=b,_c=c;a>=_c;--a)
#define forv(a,b) for(auto&a:b)
#define pii pair<int,int>
#define fi first
#define se second
#define nd second.first
#define rd second.second
#define vi vector<int>
#define vii vector<ii>
#define pb push_back
#define all(a) a.begin(),a.end()
#define reset(a,x) memset(a,x,sizeof(a))
#define bit(x,i) ((x>>(i-1))&1)
#define on(x,i) (x|(1<<(i-1)))
#define off(x,i) (x&~(1<<(i-1)))
#define bug cerr<<__LINE__<<"\n"
#define int long long
using namespace std;
ll Rand(ll l,ll r)
{
    return l + ((ll)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (ll)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
                (ll)rand() * (RAND_MAX + 1) +
                rand())%(r-l+1);
}
struct oo {int i,u,v,c;};
int a[100][100];
main()
{
    srand(time(NULL));
    forinc(Test,1,1000)
    {
        ofstream inp("test.inp");
        int HEIGHT=3, WIDTH = 3, numC = HEIGHT*WIDTH - WIDTH;
        inp << HEIGHT << " " << WIDTH << " "<<numC<< "\n";
        vector<int> e;
        forinc(i,1,numC) e.pb(i);
        while(e.size() < HEIGHT*WIDTH) e.pb(0);
        random_shuffle(e.begin(),e.end());
        forinc(i,0,HEIGHT-1)
            forinc(j,0,WIDTH-1) {
                a[i][j]=e.back();
                e.pop_back();
            }
        while(1) {
            bool ok=1;
            forinc(j,0,WIDTH-1) {
                forinc(i,0,HEIGHT-2) if(a[i][j]&&!a[i+1][j]) {
                    swap(a[i][j],a[i+1][j]);
                    ok=0;
                }
            }
            if(ok) break;
        }
        forinc(i,0,HEIGHT-1) {
            forinc(j,0,WIDTH-1) {
                inp<<a[i][j]<<" ";
            }
            inp<<"\n";
        }
        inp.close();return 0;
        system("Aco-me.exe");return 0;
        system("Aco-Nam.exe");


        std::ifstream file1("Aco-Nam.out");
        std::ifstream file2("Aco-me.out");

        if (!file1.is_open() || !file2.is_open()) {
            std::cerr << "Error: Could not open one or both of the files." << std::endl;
            return 1;
        }

        std::istreambuf_iterator<char> begin1(file1);
        std::istreambuf_iterator<char> begin2(file2);
        std::istreambuf_iterator<char> end;

        if (std::equal(begin1, end, begin2)) {
            std::cout << "The contents of the two files are equal." << std::endl;
        }
        else {
            std::cout << "The contents of the two files are not equal." << std::endl;
            return 0;
        }

        file1.close();
        file2.close();


    /*return 0;
        if(system("fc class.out class.ans")!=0)
        {
            cout<<"WRONG!";
            return 0;
        }*/
        /*ofstream inp("IJOB.inp");
        int oo=19,n=Rand(oo,oo);
        inp<<n<<"\n";
        forinc(i,1,n) inp<<Rand(min(1ll,i-1),i-1)-1<<" ";inp<<"\n";
        forinc(i,1,n) inp<<Rand(1,1e5)<<" ";inp<<"\n";
        forinc(i,1,n) inp<<Rand(1,1e5)<<" ";inp<<"\n";
        inp.close();
        system("IJOB.exe");
        system("IJOB_qhd.exe");

        if(system("fc IJOB.out IJOB.ans")!=0)
        {
            cout<<"WRONG!";
            return 0;
        }
        /*ofstream inp("PIKACHU2.inp");
        int m=Rand(1,2),n=Rand(1,200000);
        while(m*n%2) m=Rand(1,2),n=Rand(1,200000);
        inp<<m<<" "<<n<<"\n";
        forinc(i,1,m*n) a[i]=(i+1)/2;
        random_shuffle(a+1,a+m*n+1);
        int t=1;
        forinc(i,1,m)
        {
            forinc(j,1,n) inp<<a[t++]<<" ";
            inp<<"\n";
        }
        inp.close();return 0;
        system("PIKACHU2.exe");
        system("PIKACHU2_t.exe");

        if(system("fc PIKACHU2.out PIKACHU2.ans")!=0)
        {
            cout<<"WRONG!";
            return 0;
        }*/
    }
}

