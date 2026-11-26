#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

// Fast I/O with buffer refilling
static const int BUFSZ = 1 << 20;
static char ibuf[BUFSZ];
static char *iptr = ibuf + BUFSZ;
static int ibuf_left = 0;

inline void io_init() {
    ibuf_left = (int)fread(ibuf, 1, BUFSZ, stdin);
    iptr = ibuf;
}

inline int read_int() {
    int x = 0;
    // Skip non-digit characters
    while (iptr - ibuf >= ibuf_left) {
        ibuf_left = (int)fread(ibuf, 1, BUFSZ, stdin);
        iptr = ibuf;
        if (ibuf_left == 0) return x;
    }
    while (*iptr < '0' || *iptr > '9') {
        iptr++;
        if (iptr - ibuf >= ibuf_left) {
            ibuf_left = (int)fread(ibuf, 1, BUFSZ, stdin);
            iptr = ibuf;
            if (ibuf_left == 0) return x;
        }
    }
    while (*iptr >= '0' && *iptr <= '9') {
        x = x * 10 + (*iptr++ - '0');
    }
    return x;
}

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
static int coords[4000005];
static int cu[2000005], cv[2000005], ceq[2000005];

void solve() {
    int n = read_int();
    
    int coord_count = 0;
    for (int k = 0; k < n; k++) {
        cons[k].u = read_int();
        cons[k].v = read_int();
        cons[k].eq = read_int();
        coords[coord_count++] = cons[k].u;
        coords[coord_count++] = cons[k].v;
    }
    
    // Sort and unique
    sort(coords, coords + coord_count);
    int uniq_count = 1;
    for (int i = 1; i < coord_count; i++) {
        if (coords[i] != coords[i-1]) {
            coords[uniq_count++] = coords[i];
        }
    }
    
    // Use lower_bound for coordinate compression
    for (int k = 0; k < n; k++) {
        cu[k] = (int)(lower_bound(coords, coords + uniq_count, cons[k].u) - coords);
        cv[k] = (int)(lower_bound(coords, coords + uniq_count, cons[k].v) - coords);
        ceq[k] = cons[k].eq;
    }
    
    // Process equality constraints first
    dsu_init(uniq_count);
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
    io_init();
    int t = read_int();
    while (t--) {
        solve();
    }
    return 0;
}
