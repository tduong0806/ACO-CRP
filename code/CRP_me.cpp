#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>

using namespace std;

int HEIGHT;
int WIDTH;
int numC;
const int CHROM_LEN = 1000;
vector<vector<int>> base;
/* ================= BAY INIT ================= */
void Init() {
    cin >> HEIGHT >> WIDTH >> numC;
    vector<vector<int>> bay(HEIGHT, vector<int>(WIDTH, 0));
    for(int i = 0; i < HEIGHT; ++i)
        for(int j = 0; j < WIDTH; ++j) cin >> bay[i][j];
    base = bay;
}
vector<vector<int>> bayInit() {
    /*vector<vector<int>> bay(HEIGHT, vector<int>(WIDTH, 0));

    bay[0][3] = 9;
    bay[0][5] = 8;
    bay[1][0] = 6;
    bay[1][2] = 5;
    bay[1][3] = 7;
    bay[1][5] = 11;
    bay[2][0] = 10;
    bay[2][1] = 2;
    bay[2][2] = 1;
    bay[2][3] = 3;
    bay[2][5] = 4;

    return bay;*/
    return base;
}

/* ================= BASIC ================= */

bool isBayEmpty(const vector<vector<int>>& bay) {
    for (auto& r : bay)
        for (int v : r)
            if (v != 0) return false;
    return true;
}

pair<int,int> findUrgentContainer(const vector<vector<int>>& bay) {
    int mn = 1e9, y = -1, x = -1;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (bay[i][j] > 0 && bay[i][j] < mn) {
                mn = bay[i][j];
                y = i; x = j;
            }
    return {y, x};
}

bool isMoveAble(const vector<vector<int>>& bay, int y, int x) {
    if (y == 0) return true;
    return bay[y-1][x] == 0;
}

void retrieveContainer(vector<vector<int>>& bay, int y, int x) {
    bay[y][x] = 0;
}

/* ================= MOVE ================= */

int moveContainer(vector<vector<int>>& bay, int fromX, int toX) {
    if (toX < 0 || toX >= WIDTH) return -1;

    int fromY = -1;
    for (int i = 0; i < HEIGHT; ++i)
        if (bay[i][fromX] != 0) {
            fromY = i;
            break;
        }
    if (fromY == -1) return -1;

    int temp = bay[fromY][fromX];
    bay[fromY][fromX] = 0;

    int toY = HEIGHT-1;
    for (int i = 0; i < HEIGHT; ++i)
        if (bay[i][toX] != 0) {
            toY = i - 1;
            break;
        }
    if (toY < 0) return -1;

    bay[toY][toX] = temp;
    return toY;
}

/* ================= RULES ================= */

int R1(const vector<vector<int>>& bay, int x) {
    int top = -1;
    for (int i = 0; i < HEIGHT; i++)
        if (bay[i][x] != 0) { top = i; break; }
    if (top == -1) return -1;

    int best = -1, dist = WIDTH;
    for (int i = 0; i < WIDTH; i++) {
        bool ok = true;
        for (int j = 0; j < HEIGHT; j++)
            if (bay[j][i] != 0 && bay[j][i] <= bay[top][x])
                ok = false;
        if (bay[HEIGHT-1][i] == 0) ok = false;
        if (bay[0][i] != 0) ok = false;
        if (ok && abs(x-i) < dist) {
            dist = abs(x-i);
            best = i;
        }
    }
    return best;
}

int R2(const vector<vector<int>>& bay, int x) {
    int best = -1, dist = WIDTH;
    for (int i = 0; i < WIDTH; i++)
        if (bay[HEIGHT-1][i] == 0 && abs(x-i) < dist) {
            dist = abs(x-i);
            best = i;
        }
    return best;
}

int R3(const vector<vector<int>>& bay, int x) {
    vector<int> skyline(WIDTH);
    for (int i = 0; i < WIDTH; i++) {
        int h = HEIGHT;
        for (int j = 0; j < HEIGHT; j++)
            if (bay[j][i] != 0) { h = j; break; }
        skyline[i] = HEIGHT - h;
    }
    int minH = *min_element(skyline.begin(), skyline.end());

    int best = -1, dist = WIDTH;
    for (int i = 0; i < WIDTH; i++)
        if (skyline[i] == minH && i != x && abs(x-i) < dist) {
            dist = abs(x-i);
            best = i;
        }
    return best;
}

int R4(const vector<vector<int>>& bay, int x) {
    int best = -1, dist = WIDTH;
    for (int i = 0; i < WIDTH; i++)
        if (bay[0][i] == 0 && i != x && abs(x-i) < dist) {
            dist = abs(x-i);
            best = i;
        }
    return best;
}

/* ================= CHROMOSOME ================= */
int pR1 = 55, pR2 = 25, pR3 = 15, pR4 = 5;
vector<int> generateChromosome() {
    vector<int> c(CHROM_LEN);
    for (int& v : c) {
        int p = rand() % 100 + 1;
        if(p<=pR1) v=1;
        else if(p<=pR1+pR2) v=2;
        else if(p<=pR1+pR2+pR3) v=3;
        else v=4;
    }
    return c;
}

/* ================= MOVE (SIMULATION) ================= */
void Print(vector<vector<int>> bay) {
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) cout << bay[i][j] << " ";
        cout<<"\n";
    }
    cout<<"\n";
}
int moveBay(vector<vector<int>> bay, const vector<int>& chromosome) {
    int idx = 0;
    int moves = 0;

    while (!isBayEmpty(bay)) {
        if (idx >= CHROM_LEN) idx = 0;

        auto p = findUrgentContainer(bay);
        int y = p.first, x = p.second;
        //Print(bay);

        if (isMoveAble(bay, y, x)) {
            retrieveContainer(bay, y, x);
        } else {
            int dest = -1;
            //cout<<chromosome[idx]<<"\n";
            if (chromosome[idx] == 1) dest = R1(bay, x);
            if (chromosome[idx] == 2) dest = R2(bay, x);
            if (chromosome[idx] == 3) dest = R3(bay, x);
            if (chromosome[idx] == 4) dest = R4(bay, x);

            if (dest != -1) {
                //cout<<dest<<"\n";
                moveContainer(bay, x, dest);
                //Print(bay);
                moves++;
            }
            idx++;
        }
    }
    //exit(0);
    return moves;
}

/* ================= GA HELPERS ================= */

void calChromosomePool(
    const vector<vector<int>>& chromosomePool,
    vector<int>& times
) {
    times.clear();
    for (auto& c : chromosomePool)
        times.push_back(moveBay(bayInit(), c));
}

vector<int> chromosomeSelect(const vector<int>& times) {
    vector<int> sorted = times;
    sort(sorted.begin(), sorted.end());
    int median = sorted[sorted.size()/2];

    vector<int> idx;
    for (int i = 0; i < times.size(); i++)
        if (times[i] < median)
            idx.push_back(i);

    if (idx.empty()) idx.push_back(0);
    return idx;
}

vector<vector<int>> crossover(
    const vector<int>& indexes,
    const vector<vector<int>>& pool
) {
    vector<vector<int>> children;
    for (int i = 0; i + 1 < indexes.size(); i += 2) {
        vector<int> child;
        int a = indexes[i];
        int b = indexes[i+1];

        child.insert(child.end(),
            pool[a].begin(), pool[a].begin() + CHROM_LEN/2);
        child.insert(child.end(),
            pool[b].begin() + CHROM_LEN/2, pool[b].end());

        children.push_back(child);
    }
    return children;
}

/* ================= GA ================= */

int GA() {
    vector<vector<int>> chromosomePool;
    vector<vector<int>> upperChromosome;
    vector<int> times;

    for (int i = 0; i < 1000; i++)
        chromosomePool.push_back(generateChromosome());

    for (int gen = 0; gen < 10; gen++) {
        calChromosomePool(chromosomePool, times);
        vector<int> idx = chromosomeSelect(times);

        upperChromosome.clear();
        for (int i : idx)
            upperChromosome.push_back(chromosomePool[i]);

        vector<vector<int>> children = crossover(idx, chromosomePool);
        chromosomePool = upperChromosome;
        chromosomePool.insert(
            chromosomePool.end(), children.begin(), children.end());
    }

    return *min_element(times.begin(), times.end());
}

/* ================= RANDOM ================= */

int RandomMoves() {
    vector<vector<int>> pool;
    vector<int> times;
    pool.push_back(generateChromosome());

    while (!pool.empty()) {
        calChromosomePool(pool, times);
        vector<int> idx = chromosomeSelect(times);
        pool = crossover(idx, pool);
    }
    return times.empty() ? 0 : times[0];
}

/* ================= MAIN ================= */

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("test.inp","r",stdin);
    //freopen("Maglic_ts.out","w",stdout);
    srand(time(nullptr));
    Init();
    vector<int> GAtimes;
    for (int i = 0; i < 10; i++) {
        int t = GA();
        GAtimes.push_back(t);
        cout << "Run " << i << " : " << t << endl;
    }

    double avg = accumulate(GAtimes.begin(), GAtimes.end(), 0.0) / GAtimes.size();
    cout << "Average = " << avg << endl;

    return 0;
}
