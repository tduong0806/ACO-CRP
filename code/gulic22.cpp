#include <bits/stdc++.h>
using namespace std;

/*
Call:
algorithm.exe inputFile outputFile
*/

static const int MAXN = 10;
static const int INF = 1e9;

int numTiers, numStacks, numContainers;
int initBay[MAXN][MAXN], tempBay[MAXN][MAXN];

/*
GA PARAMETERS
*/
const int NUM_GEN = 300;
const int POP_SIZE = 100;
const double RATE_MUT = 0.05;
const double PERCENT_TO_POOL = 0.5;
const int NUM_ELITES = 1;

#define GENE_LENGTH (10 * numContainers)

/*
UTILITIES
*/
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int random_int(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}

bool random_prob(double p) {
    uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng) < p;
}

/*
GENETIC SOLUTION
*/
class Solution {
private:
    vector<int> chromosome;
    int fitness;

    void initRandomly() {
        for (int &c : chromosome)
            c = random_int(0, numStacks - 1);
        fitness = -1;
    }

public:
    Solution() {
        chromosome.resize(GENE_LENGTH);
        initRandomly();
    }

    void assignChromosome(const vector<int> &chromo) {
        chromosome = chromo;
        fitness = -1;
    }

    int getFitness() {
        if (fitness != -1) return fitness;

        fitness = 0;

        // copy initial bay
        for (int i = 0; i < numTiers; i++)
            for (int j = 0; j < numStacks; j++)
                tempBay[i][j] = initBay[i][j];

        int curGene = 0;

        for (int k = 1; k <= numContainers; k++) {
            int curTier = -1, curStack = -1;

            // locate container k
            for (int i = 0; i < numTiers; i++)
                for (int j = 0; j < numStacks; j++)
                    if (tempBay[i][j] == k) {
                        curTier = i;
                        curStack = j;
                    }

            // remove blocking containers
            for (int i = 0; i < curTier; i++) {
                if (tempBay[i][curStack] == 0) continue;

                bool removed = false;
                while (curGene < GENE_LENGTH) {
                    int nxtStack = chromosome[curGene++];
                    if (nxtStack != curStack && tempBay[0][nxtStack] == 0) {
                        fitness++;
                        removed = true;

                        int highest = numTiers;
                        for (int j = 0; j < numTiers; j++)
                            if (tempBay[j][nxtStack] != 0) {
                                highest = j;
                                break;
                            }

                        swap(tempBay[i][curStack],
                             tempBay[highest - 1][nxtStack]);
                        break;
                    }
                }
                if (!removed) {
                    fitness = INF;
                    return fitness;
                }
            }

            // retrieve container k
            tempBay[curTier][curStack] = 0;
        }

        return fitness;
    }

    bool operator<(const Solution &other) const {
        return const_cast<Solution*>(this)->getFitness()
             < const_cast<Solution&>(other).getFitness();
    }

    void mutate() {
        for (int i = 0; i < GENE_LENGTH; i++) {
            if (random_prob(RATE_MUT))
                chromosome[i] = random_int(0, numStacks - 1);
        }
        fitness = -1;
    }

    string repr() const {
        stringstream ss;
        ss << "{ ";
        for (int x : chromosome) ss << x << ' ';
        ss << "}";
        return ss.str();
    }

    friend pair<Solution, Solution> crossover(const Solution&, const Solution&);
};

/*
CROSSOVER
*/
pair<Solution, Solution> crossover(const Solution &father,
                                   const Solution &mother) {
    vector<int> A(GENE_LENGTH), B(GENE_LENGTH);

    int point = random_int(1, GENE_LENGTH - 2);

    for (int i = 0; i < point; i++) {
        A[i] = father.chromosome[i];
        B[i] = mother.chromosome[i];
    }
    for (int i = point; i < GENE_LENGTH; i++) {
        A[i] = mother.chromosome[i];
        B[i] = father.chromosome[i];
    }

    Solution s1, s2;
    s1.assignChromosome(A);
    s2.assignChromosome(B);
    return {s1, s2};
}

/*
SOLVER
*/
void solve() {
    cin >> numTiers >> numStacks >> numContainers;

    for (int i = 0; i < numTiers; i++)
        for (int j = 0; j < numStacks; j++)
            cin >> initBay[i][j];

    vector<Solution> population(POP_SIZE);
    vector<int> bestFitness;

    for (int gen = 1; gen <= NUM_GEN; gen++) {
        sort(population.begin(), population.end());

        int poolSize = int(PERCENT_TO_POOL * POP_SIZE);
        vector<Solution> pool(population.begin(),
                              population.begin() + poolSize);

        vector<Solution> offspring;

        for (int i = 0; i < poolSize / 2; i++) {
            int a = random_int(0, poolSize - 1);
            int b = random_int(0, poolSize - 1);
            auto children = crossover(pool[a], pool[b]);
            offspring.push_back(children.first);
            offspring.push_back(children.second);
        }

        for (auto &s : offspring)
            s.mutate();

        vector<Solution> elites(population.begin(),
                                population.begin() + NUM_ELITES);

        population = offspring;
        while ((int)population.size() < POP_SIZE - NUM_ELITES)
            population.emplace_back();

        population.insert(population.begin(),
                          elites.begin(), elites.end());

        sort(population.begin(), population.end());
        bestFitness.push_back(population[0].getFitness());
    }

    // Output result
    cout << population[0].getFitness() << "\n";
    cout << population[0].repr() << "\n";
}

/*
MAIN
*/
int main() {

    freopen("test.inp", "r", stdin);
    //freopen(argv[2], "w", stdout);

    solve();
    return 0;
}
