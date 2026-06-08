#include <bits/stdc++.h>
using namespace std;

/* ================== UTILITIES ================== */

using Real = double;
const int INF = 1e9;

mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

int random_int(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}

bool random_prob(Real p) {
    bernoulli_distribution dist(p);
    return dist(rng);
}

#define possibly(p, code) if (random_prob(p)) { code; }
#define all_of(x) (x).begin(), (x).end()
#define cst(T) const T&

template<typename T>
void umin(T &a, T b) { a = min(a, b); }

/* ================== PROBLEM ================== */

const int N = 10;
int numTiers, numStacks, numContainers;
int initBay[N][N], tempBay[N][N];

/*
GA CONSTANTS
*/
const int NUM_GEN = 20;
const int POP_SIZE = 8000;
const Real RATE_MUT = 0.05;
const Real RATE_CROSS = 0.95;
const Real RATE_PRESERVE = 0.5;

/*
REPRESENTATION
*/
int minContainer[N], height[N], skyline;
bool isEmpty[N], isFull[N];

void getStacksInfo() {
    skyline = INF;
    for (int s = 0; s < numStacks; s++) {
        minContainer[s] = INF;
        height[s] = 0;
        isEmpty[s] = isFull[s] = true;
        for (int t = 0; t < numTiers; t++) {
            if (tempBay[t][s]) {
                umin(minContainer[s], tempBay[t][s]);
                height[s]++;
                isEmpty[s] = false;
            } else {
                isFull[s] = false;
            }
        }
        if (height[s]) umin(skyline, height[s]);
    }
}

int getNextStack(int rule, int t, int s) {
    int nxt = -1;
    getStacksInfo();

    if (rule == 1) {
        for (int k = 0; k < numStacks; k++)
            if (k != s && !isEmpty[k] && !isFull[k] && minContainer[k] > tempBay[t][s])
                if (nxt == -1 || abs(k - s) < abs(nxt - s)) nxt = k;
    }
    else if (rule == 2) {
        for (int k = 0; k < numStacks; k++)
            if (isEmpty[k])
                if (nxt == -1 || abs(k - s) < abs(nxt - s)) nxt = k;
    }
    else if (rule == 3) {
        for (int k = 0; k < numStacks; k++)
            if (k != s && !isFull[k] && height[k] == skyline)
                if (nxt == -1 || abs(k - s) < abs(nxt - s)) nxt = k;
    }
    else { // rule == 4
        for (int k = 0; k < numStacks; k++)
            if (k != s && !isFull[k])
                if (nxt == -1 || abs(k - s) < abs(nxt - s)) nxt = k;
    }
    return nxt;
}

/* ================== GENETIC ALGORITHM ================== */

#define GENE_LENGTH (10 * numContainers)

class Solution {
private:
    vector<int> chromosome;
    int fitness;

    void initRandomly() {
        for (auto &c : chromosome)
            c = random_int(1, 4);
        fitness = -1;
    }

public:
    Solution() {
        chromosome.resize(GENE_LENGTH);
        initRandomly();
    }

    void assignChromosome(const vector<int>& chromo) {
        chromosome = chromo;
        fitness = -1;
    }

    int getFitness() {
        if (fitness != -1) return fitness;

        fitness = 0;
        memcpy(tempBay, initBay, sizeof(initBay));
        int curGene = 0;

        for (int k = 1; k <= numContainers; k++) {
            int curTier = -1, curStack = -1;
            for (int i = 0; i < numTiers; i++)
                for (int j = 0; j < numStacks; j++)
                    if (tempBay[i][j] == k)
                        curTier = i, curStack = j;

            for (int i = 0; i < curTier; i++) {
                if (!tempBay[i][curStack]) continue;
                bool removed = false;

                while (curGene < GENE_LENGTH) {
                    int rule = chromosome[curGene++];
                    int nxt = getNextStack(rule, i, curStack);
                    if (nxt != -1 && tempBay[0][nxt] == 0) {
                        fitness++;
                        removed = true;
                        int h = numTiers;
                        for (int j = 0; j < numTiers; j++)
                            if (tempBay[j][nxt]) { h = j; break; }
                        swap(tempBay[i][curStack], tempBay[h - 1][nxt]);
                        break;
                    }
                }
                if (!removed) return fitness = INF;
            }
            tempBay[curTier][curStack] = 0;
        }
        return fitness;
    }

    bool operator<( Solution& other) {
        return getFitness() < other.getFitness();
    }

    friend pair<Solution, Solution> crossover(cst(Solution) a, cst(Solution) b);

    void mutate() {
        for (int i = 0; i < GENE_LENGTH; i++)
            possibly(RATE_MUT, chromosome[i] = random_int(1, 4));
        fitness = -1;
    }

    string repr() const {
        stringstream ss;
        for (int x : chromosome) ss << x << ' ';
        return ss.str();
    }
};

pair<Solution, Solution> crossover(cst(Solution) father, cst(Solution) mother) {
    vector<int> A(GENE_LENGTH), B(GENE_LENGTH);
    int p = random_int(0, GENE_LENGTH);

    for (int i = 0; i < p; i++)
        A[i] = father.chromosome[i], B[i] = mother.chromosome[i];
    for (int i = p; i < GENE_LENGTH; i++)
        A[i] = mother.chromosome[i], B[i] = father.chromosome[i];

    Solution x, y;
    x.assignChromosome(A);
    y.assignChromosome(B);
    return {x, y};
}

/* ================== SOLVE ================== */

void solve() {
    cin >> numTiers >> numStacks >> numContainers;
    for (int i = 0; i < numTiers; i++)
        for (int j = 0; j < numStacks; j++)
            cin >> initBay[i][j];

    vector<Solution> population(POP_SIZE);
    sort(all_of(population));

    for (int gen = 1; gen <= NUM_GEN; gen++) {
        vector<Solution> offspring;
        int need = (1 - RATE_PRESERVE) * POP_SIZE;

        while ((int)offspring.size() < need) {
            possibly(RATE_CROSS, {
                int a = random_int(0, POP_SIZE - 1);
                int b = random_int(0, POP_SIZE - 1);
                auto kids = crossover(population[a], population[b]);
                offspring.push_back(kids.first);
                offspring.push_back(kids.second);
            });
        }

        for (auto &s : offspring) s.mutate();

        population.resize(RATE_PRESERVE * POP_SIZE);
        population.insert(population.end(), all_of(offspring));
        sort(all_of(population));

        cout << "Gen " << gen
             << " best = " << population[0].getFitness() << "\n";
    }

    cout << "Best chromosome:\n" << population[0].repr() << "\n";
}

/* ================== MAIN ================== */

int main() {
    freopen("test.inp", "r", stdin);
    solve();
    return 0;
}
