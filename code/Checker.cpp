#include <bits/stdc++.h>
typedef long long ll;
typedef unsigned long long ull;
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
using namespace std;
const int MAX = 100;
int HEIGHT, WIDTH, numContainers;
vector<vector<int>> root;
vector<vector<vector<int>>> q[1010];
/* ================= BAY INIT ================= */
void Init() {
    cin >> HEIGHT >> WIDTH >> numContainers;
    vector<vector<int>> bay(HEIGHT, vector<int>(WIDTH, 0));
    for(int i = 0; i < HEIGHT; ++i)
        for(int j = 0; j < WIDTH; ++j) cin >> bay[i][j];

    root = bay;
}

void printBay(vector<vector<int>> &bay) {
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) cout << bay[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}
void Print(int t, int i) {
    cout << t << "\n";
}

/*vector<int> Score(vector<vector<int>> &bay) {
    vector<int> ret(numContainers, 0);
    for(int j = 0; j < WIDTH; ++j) {
            for(int i = 0; i < HEIGHT; ++i) if(bay[i][j]) {
                int r=i,s=0;
                while(r>=0&&bay[r][j]) {
                    --r;
                    ++s;
                }
                ret[bay[i][j]-1]=s;
            }
        }

    return ret;
}*/

vector<int> Score(vector<vector<int>> &bay) {
    vector<int> ret(numContainers, 0);
    for(int j = 0; j < WIDTH; ++j) if(bay[HEIGHT - 1][j]) {
            int r = HEIGHT - 1;
            while(r >= 0) {
                if(!bay[r][j]) break;
                int l = r, s = 0;
                while(l >= 0 && bay[l][j] >= bay[r][j]) {
                    //ret[bay[l][j] - 1] = --s;
                    --l;
                }
                ret[bay[r][j] - 1] = (r - l);
                r = l;
            }
        }
        /*printBay(bay);
        forv(x,ret) cout<<x<<" ";
        exit(0);*/
    return ret;
}

bool isBayEmpty(const vector<vector<int>>& bay) {
    for (auto& r : bay)
        for (int v : r)
            if (v != 0) return false;
    return true;
}

pair<int, int> findUrgentContainer(const vector<vector<int>>& bay) {
    int mn = 1e9, y = -1, x = -1;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (bay[i][j] > 0 && bay[i][j] < mn) {
                mn = bay[i][j];
                y = i;
                x = j;
            }
    return {y, x};
}

bool isMoveAble(const vector<vector<int>>& bay, int y, int x) {
    if (y == 0) return true;
    return bay[y - 1][x] == 0;
}

void retrieveContainer(vector<vector<int>>& bay, int y, int x) {
    bay[y][x] = 0;
}

void moveContainer(vector<vector<int>> &bay, int fromX, int toX) {
    int fromY = -1;
    for (int i = 0; i < HEIGHT; ++i)
        if (bay[i][fromX] != 0) {
            fromY = i;
            break;
        }

    int temp = bay[fromY][fromX];
    bay[fromY][fromX] = 0;

    int toY = HEIGHT - 1;
    for (int i = 0; i < HEIGHT; ++i)
        if (bay[i][toX] != 0) {
            toY = i - 1;
            break;
        }

    bay[toY][toX] = temp;
}

bool canMoveContainer(vector<vector<int>> &bay, int fromX, int toX) {
    if(fromX == toX) return false;
    int fromY = -1;
    for (int i = 0; i < HEIGHT; ++i)
        if (bay[i][fromX] != 0) {
            fromY = i;
            break;
        }
    if(fromY < 0) return false;

    int toY = HEIGHT - 1;
    for (int i = 0; i < HEIGHT; ++i)
        if (bay[i][toX] != 0) {
            toY = i - 1;
            break;
        }

    if(toY < 0) return false;
    return true;
}


bool cmp(vector<vector<int>> &u, vector<vector<int>> &v) {
    return Score(u) < Score(v);
}

void Clean(vector<vector<int>> &v) {
    while(!isBayEmpty(v)) {
        //printBay(v);
        auto p = findUrgentContainer(v);
        int y = p.first, x = p.second;

        if (isMoveAble(v, y, x)) {
            retrieveContainer(v, y, x);
        } else break;
    }
}

pair<int,int> findCon(vector<vector<int>> &bay,int c) {
    pair<int,int> ret = {-1,-1};
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) if(bay[i][j]==c) {
            if(i>0&&bay[i-1][j]>0) return ret;
            return make_pair(i,j);
        }
    }
    return ret;
}

void Check() {
    int ans;
    cin>>ans;
    vector<vector<int>> bay = root;
    Clean(bay);
    printBay(bay);
    forinc(i,1,ans) {
        int c, to;
        cin>> c>>to;
        cout<<c<<" "<<to<<"\n";
        int go = findCon(bay,c).second;
        if(go==-1) {
            cout<<c<<" "<<"Sai\n";
            exit(0);
        }
        if(canMoveContainer(bay,go,to)) {
            moveContainer(bay, go, to);
            Clean(bay);
            printBay(bay);
        } else {
            cout<<"Sai1\n";
            exit(0);
        }
    }
}
main() {
    freopen("test.inp", "r", stdin);
    //freopen("Cach2.out","w",stdout);
    Init();
    Check();
}

