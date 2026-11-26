#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

// Union-Find with path compression and union by rank (array-based for speed)
struct DSU {
    int *parent, *rnk;
    DSU(int n) {
        parent = new int[n];
        rnk = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rnk[i] = 0;
        }
    }
    ~DSU() {
        delete[] parent;
        delete[] rnk;
    }
    int find(int x) {
        // Path compression
        int root = x;
        while (parent[root] != root) root = parent[root];
        while (parent[x] != root) {
            int tmp = parent[x];
            parent[x] = root;
            x = tmp;
        }
        return root;
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rnk[a] < rnk[b]) { int t = a; a = b; b = t; }
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
    }
};

struct Constraint {
    int u, v, eq;
};

void solve() {
    int n;
    scanf("%d", &n);
    
    // Collect all variable indices for coordinate compression
    static int coords[4000005];
    int coord_count = 0;
    
    static Constraint cons[2000005];
    for (int k = 0; k < n; k++) {
        scanf("%d %d %d", &cons[k].u, &cons[k].v, &cons[k].eq);
        coords[coord_count++] = cons[k].u;
        coords[coord_count++] = cons[k].v;
    }
    
    // Sort and unique for coordinate compression
    sort(coords, coords + coord_count);
    int uniq_count = 1;
    for (int i = 1; i < coord_count; i++) {
        if (coords[i] != coords[i-1]) {
            coords[uniq_count++] = coords[i];
        }
    }
    
    // Binary search for compressed index
    auto get_id = [&](int val) -> int {
        int lo = 0, hi = uniq_count - 1;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            if (coords[mid] == val) return mid;
            else if (coords[mid] < val) lo = mid + 1;
            else hi = mid - 1;
        }
        return -1;
    };
    
    // Build compressed constraints
    static int cu[2000005], cv[2000005], ceq[2000005];
    for (int k = 0; k < n; k++) {
        cu[k] = get_id(cons[k].u);
        cv[k] = get_id(cons[k].v);
        ceq[k] = cons[k].eq;
    }
    
    int num_vars = uniq_count;
    
    // Process equality constraints first
    DSU dsu(num_vars);
    for (int k = 0; k < n; k++) {
        if (ceq[k] == 1) {
            dsu.unite(cu[k], cv[k]);
        }
    }
    
    // Check inequality constraints
    bool ok = true;
    for (int k = 0; k < n; k++) {
        if (ceq[k] == 0) {
            if (dsu.find(cu[k]) == dsu.find(cv[k])) {
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
