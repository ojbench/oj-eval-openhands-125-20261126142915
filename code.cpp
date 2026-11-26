#include <cstdio>
#include <algorithm>
#include <cstring>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// Hash map for coordinate compression - O(1) average lookup
typedef gp_hash_table<int, int> HashTable;

// DSU with flat arrays and path compression only
static int dsu_parent[4000005];

inline void dsu_init(int n) {
    for (int i = 0; i < n; i++) dsu_parent[i] = i;
}

inline int dsu_find(int x) {
    while (dsu_parent[x] != x) {
        dsu_parent[x] = dsu_parent[dsu_parent[x]]; // path halving
        x = dsu_parent[x];
    }
    return x;
}

inline void dsu_union(int a, int b) {
    a = dsu_find(a);
    b = dsu_find(b);
    if (a != b) dsu_parent[a] = b;
}

struct Constraint {
    int u, v, eq;
};

static Constraint cons[2000005];
static int cu[2000005], cv[2000005], ceq[2000005];

void solve() {
    int n;
    scanf("%d", &n);
    
    // Use hash table for coordinate compression
    static HashTable hmap;
    hmap.clear();
    int next_id = 0;
    
    for (int k = 0; k < n; k++) {
        scanf("%d %d %d", &cons[k].u, &cons[k].v, &cons[k].eq);
        
        auto it_u = hmap.find(cons[k].u);
        if (it_u == hmap.end()) {
            hmap[cons[k].u] = next_id++;
        }
        auto it_v = hmap.find(cons[k].v);
        if (it_v == hmap.end()) {
            hmap[cons[k].v] = next_id++;
        }
    }
    
    // Build compressed constraints
    for (int k = 0; k < n; k++) {
        cu[k] = hmap[cons[k].u];
        cv[k] = hmap[cons[k].v];
        ceq[k] = cons[k].eq;
    }
    
    // Process equality constraints first
    dsu_init(next_id);
    for (int k = 0; k < n; k++) {
        if (ceq[k] == 1) {
            dsu_union(cu[k], cv[k]);
        }
    }
    
    // Check inequality constraints
    bool ok = true;
    for (int k = 0; k < n; k++) {
        if (ceq[k] == 0) {
            if (dsu_find(cu[k]) == dsu_find(cv[k])) {
                ok = false;
                break;
            }
        }
    }
    
    printf("%s\n", ok ? "YES" : "NO");
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        solve();
    }
    return 0;
}
