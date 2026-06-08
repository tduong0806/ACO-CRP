#pragma GCC optimize("O3,unroll-loops,inline")
#include <bits/stdc++.h>
#include "logger.hpp"

using namespace std;

#define ld long double

const int initialNumAnts = 500;
const int finalNumAnts   = 500;
const int numIterations = 1200;
const ld rho_max = 0.4;
const ld rho_min = 0.05;
const ld tMax = 1;
const int N = 20;
const ld A = 1, B = 0.5;

int numTiers, numStacks, numContainers;
int bay[N][N], tmp[N][N], height[N];
int ans;
ld tMin;
ld T[N*N][N*N], pwb[N*N];
pair<int,int> pos[N*N];

vector<int> res;
mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());
map<int,int> mp;

void place(int c, int s){
    int x = pos[c].first;
    int y = pos[c].second;

    tmp[x][y] = 0;
    height[y]--;

    height[s]++;
    tmp[height[s]-1][s] = c;
    pos[c] = make_pair(height[s]-1, s);
}

int get_min(int s){
    int mn = numContainers + 1;
    for (int i = 0; i < height[s]; i++)
        mn = min(mn, tmp[i][s]);
    return mn;
}

ld weight[N];
int minContainer[N];

vector<pair<int, pair<int,int>>> __attribute__((target("avx2")))
generateSolution(){
    vector<pair<int, pair<int,int>>> res;

    memset(height, 0, sizeof(height));

    for (int i = 0; i < numStacks; i++)
        for (int j = 0; j < numTiers && bay[j][i]; j++){
            height[i] = j + 1;
            pos[bay[j][i]] = make_pair(j, i);
            tmp[j][i] = bay[j][i];
        }

    for (int i = 1; i <= numContainers; ){
        int x = pos[i].first;
        int y = pos[i].second;

        if (x == height[y] - 1){
            height[y]--;
            i++;
            continue;
        }

        int curContainer = tmp[height[y]-1][y];
        ld sum = 0.0, mx = 0.0;
        int mxs = -1;

        for (int j = 0; j < numStacks; j++){
            if (j != y && height[j] < numTiers){
                int mn = j;

                ld val = T[curContainer][mn] * pwb[mn];

                weight[j] = val;
                minContainer[j] = mn;
                sum += val;
            }
        }

        ld randomval = (ld)(rd()) / rd.max();
        bool ok=0;

        for (int j = 0; j < numStacks; j++){
            if (j != y && height[j] < numTiers){
                if (weight[j] / sum >= randomval){
                    res.push_back(
                        make_pair(curContainer,
                                  make_pair(minContainer[j], j))
                    );
                    place(curContainer, j);
                    ok=1;
                    break;
                }
                randomval -= weight[j] / sum;
            }
        }
    }
    return res;
}

void solve(){
    cin >> numTiers >> numStacks >> numContainers;

    for (int i = numTiers - 1; i >= 0; i--)
        for (int j = 0; j < numStacks; j++)
            cin >> bay[i][j];

    tMin = 1.0 / numContainers;
    ld rho = rho_min;

    for (int i = 1; i <= numContainers; i++)
        for (int j = 0; j <= numContainers + 2; j++)
            T[i][j] = tMax;

    vector<pair<int, pair<int,int>>> gBest;
    vector<int> results;
    int lastImprovement = 0;

    for (int it = 0; it < numIterations; it++){
        rho = 0.05;

        vector<pair<int, pair<int,int>>> iBest;
        int numAnts = max((int)(initialNumAnts - .5 * it), finalNumAnts);

        for (int i = 0; i < numAnts; i++){
            vector<pair<int, pair<int,int>>> sol = generateSolution();
            if (iBest.empty() || iBest.size() > sol.size())
                iBest.swap(sol);
        }

        if (gBest.empty() || gBest.size() >= iBest.size()){
            gBest = iBest;
            lastImprovement = it;
        }

        vector<pair<int, pair<int,int>>> updateToTMax =
            (rd() & 1) ? gBest : iBest;

        for (int i = 1; i <= numContainers; i++)
            for (int j = 0; j <= numContainers + 2; j++)
                {
                    T[i][j] += rho * (tMin - T[i][j]);
                    if(T[i][j]>=1) cout<<"????\n";
                }


        for (size_t k = 0; k < updateToTMax.size(); k++){
            int i = updateToTMax[k].first;
            int x = updateToTMax[k].second.first;
            ld _old = T[i][x];
            T[i][x] += rho * (tMax - tMin);
            if(T[i][x]>1) {cout<<"?\n"<<" "<<i<<" "<<x<<" "<<_old<<" "<<rho*(tMax-tMin)<<" "<<T[i][x]<<"\n";exit(0);}
        }

        results.push_back((int)iBest.size());
    }

    ans = (int)gBest.size();

    stringstream conf;
    conf << "{";
    for (size_t i = 0; i < gBest.size(); i++){
        conf << gBest[i].second.second;
        if (i + 1 < gBest.size()) conf << ",";
    }
    conf << "}";

    cout<< ans <<"\n";

    {
        for(auto &x: gBest) {
            cout<<x.first<<" "<<x.second.second<<"\n";
        }
    }
}

int main(){
    freopen("test.inp","r",stdin);
    //freopen("Aco-Nam.out","w",stdout);

    pwb[0] = 1;
    for (int i = 1; i < N * N; i++)
        pwb[i] = pow(i, B);


    solve();
    return 0;
}
