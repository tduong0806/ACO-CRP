#pragma GCC optimize("O3,unroll-loops,inline")
#include <bits/stdc++.h>
#include "logger.hpp"

using namespace std;

#define ld long double

const int initialNumAnts = 600;
const int numIterations = 600;
const ld rho_max = 0.4;
const ld rho_min = 0.05;
const ld tMax = 1;
const int N = 20;
const ld A = 1, B = 0.5;

int numTiers, numStacks, numContainers;
int bay[N][N], tmp[N][N], height[N], dd[N * N];
int ans;
ld tMin;
ld T[N * N][N * N], pwb[N * N];
int rr = 0;
pair<int, int> pos[N * N];

vector<int> res;
mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());
map<int, int> mp;

void place(int c, int s) {
    int x = pos[c].first;
    int y = pos[c].second;

    tmp[x][y] = 0;
    height[y]--;

    height[s]++;
    tmp[height[s] - 1][s] = c;
    pos[c] = make_pair(height[s] - 1, s);
}

int get_min(int s) {
    int mn = numContainers + 1;
    for (int i = 0; i < height[s]; i++)
        mn = min(mn, tmp[i][s]);
    return mn;
}

int get_min_(int s) {
    int mn = numContainers + 1;
    for (int i = 0; i < height[s] - 1; i++)
        mn = min(mn, tmp[i][s]);
    return mn;
}

ld weight[N];
int minContainer[N];

int numTran(vector<pair<int, pair<int, int>>> &res) {
    int ret = 0;
    for(auto &x : res) ret += (x.second.first < numContainers + 2);
    return ret;
}

void Move(int curContainer, bool trans, vector<pair<int, pair<int, int>>> &res, int i, int y, int py) {
    ld sum = 0.0;

    for (int j = 0; j < numStacks; j++) {
        if (j != y && j != py && height[j] < numTiers) {

            int mn = get_min(j);

            ld val = T[curContainer][mn] * pwb[mn];
            if (mn < curContainer)
                val /= max(height[j], numTiers - mn + i + 1);
            weight[j] = val;
            minContainer[j] = mn;
            sum += val;
        }
    }

    int cnt = 0;

    if(!trans) {
        for (int j = 0; j < numStacks; j++)
            if (j != y && height[j] < numTiers) ++cnt;
        if(cnt > 1) sum += T[curContainer][numContainers + 2];
    }

    ld randomval = (ld)(rd()) / rd.max();

    for (int j = 0; j < numStacks; j++) {
        if (j != y && j != py && height[j] < numTiers) {
            if (weight[j] / sum >= randomval) {
                res.push_back(
                    make_pair(curContainer,
                              make_pair(minContainer[j], j))
                );
                place(curContainer, j);
                return;
            }
            randomval -= weight[j] / sum;
        }
    }

    if(cnt > 1) {

        sum = 0.0;

        for (int j = 0; j < numStacks; j++) {
            if (j != y && height[j] > 0) {

                int mn = get_min_(j);

                ld val = T[curContainer][mn] * pwb[mn] / pwb[tmp[height[j] - 1][j]];
                if (mn < curContainer)
                    val /= max(height[j], numTiers - mn + i + 1);
                weight[j] = val;
                minContainer[j] = mn;
                sum += val;
            }
        }

        randomval = (ld)(rd()) / rd.max();

        for (int j = 0; j < numStacks; j++) {
            if (j != y && height[j] > 0) {
                if (weight[j] / sum >= randomval) {
                    res.push_back(
                        make_pair(curContainer,
                                  make_pair(numContainers + 2, j))
                    );
                    Move(tmp[height[j] - 1][j], 1, res, i, j, y);
                    res.push_back(
                        make_pair(curContainer,
                                  make_pair(minContainer[j], j))
                    );
                    place(curContainer, j);
                    return;
                }
                randomval -= weight[j] / sum;
            }
        }
    }
}


vector<pair<int, pair<int, int>>> generateSolution() {
    vector<pair<int, pair<int, int>>> res;

    memset(height, 0, sizeof(height));

    for (int i = 0; i < numStacks; i++)
        for (int j = 0; j < numTiers && bay[j][i]; j++) {
            height[i] = j + 1;
            pos[bay[j][i]] = make_pair(j, i);
            tmp[j][i] = bay[j][i];
        }
    for (int i = 1; i <= numContainers; ) {
        int x = pos[i].first;
        int y = pos[i].second;

        if (x == height[y] - 1) {
            height[y]--;
            i++;
            continue;
        }

        int curContainer = tmp[height[y] - 1][y];
        Move(curContainer, 0, res, i, y, -1);
    }
    return res;
}

void solve() {
    cin >> numTiers >> numStacks >> numContainers;

    for (int i = numTiers - 1; i >= 0; i--)
        for (int j = 0; j < numStacks; j++)
            cin >> bay[i][j];

    vector<pair<int, pair<int, int>>> best;

    for(int loops = 1; loops <= 3; ++loops) {

        tMin = 1.0 / numContainers;
        ld rho = rho_min;

        for (int i = 1; i <= numContainers; i++)
            for (int j = 1; j <= numContainers + 2; j++)
                T[i][j] = tMax;

        vector<pair<int, pair<int, int>>> gBest;
        vector<int> results;
        int lastImprovement = 0;

        for (int it = 0; it < numIterations; it++) {
            rho = rho_min +
                  (rho_max - rho_min) *
                  pow((ld)it / (numIterations - 1), 3.0);

            vector<pair<int, pair<int, int>>> iBest;
            int numAnts = initialNumAnts;

            for (int i = 0; i < numAnts; i++) {
                vector<pair<int, pair<int, int>>> sol = generateSolution();
                if (iBest.empty() || numTran(iBest) > numTran(sol))
                    iBest.swap(sol);
            }

            if (gBest.empty() || numTran(gBest) >= numTran(iBest)) {
                gBest = iBest;
                lastImprovement = it;
            }

            vector<pair<int, pair<int, int>>> updateToTMax =
                (rd() % 2) ? gBest : iBest;

            //cout << it << " " << numTran(iBest) << " " << numTran(gBest) << "\n";

            for (int i = 1; i <= numContainers; i++)
                for (int j = 1; j <= numContainers + 2; j++)
                    T[i][j] += rho * (tMin - T[i][j]);

            for (size_t k = 0; k < updateToTMax.size(); k++) {
                int i = updateToTMax[k].first;
                int x = updateToTMax[k].second.first;
                T[i][x] += rho * (tMax - tMin);
            }

            results.push_back(numTran(iBest));
        }

        if (best.empty() || numTran(best) >= numTran(gBest)) swap(best, gBest);
    }

    cout << numTran(best) << "\n";

    {
        for(auto &x : best) {
            if(x.second.first < numContainers + 2) cout << x.first << " " << x.second.second << "\n";
        }
    }
}

int main() {
    //freopen("test.inp", "r", stdin);
    //freopen("Aco-me.out", "w", stdout);

    pwb[0] = 1;
    for (int i = 1; i < N * N; i++)
        pwb[i] = pow(i, B);

    solve();
    return 0;
}
