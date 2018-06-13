#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;

#ifdef FUSY
#define deb(args...) {string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); err(_it, args); cerr << endl;}
#define d(arg) {cerr << #arg << " = " << arg << endl << endl;}
#else
#define deb(args...)
#define d(arg)
#endif

template<class T, class R> ostream& operator<<(ostream& os, pair<T, R>& v) {os << '(' << v.first << ' ' << v.second << ')' << ' '; return os;}
template<class T> ostream& operator<<(ostream& os, vector<T>& v) {for (auto i : v) os << i + 1 << " "; return os;}
template<class T> ostream& operator<<(ostream& os, set<T>& v) {for (auto i : v) os << i << " "; return os;}

void err(istream_iterator<string> it) {}
template<typename T, typename... Args>
void err(istream_iterator<string> it, T a, Args... args) {
	cerr << *it << " = " << a << endl;
	err(++it, args...);
}

void fastIO() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
}

struct Edge {
    int to;
    int cost;
    int cap;
    int flow;
    int number;
    Edge(int to, int cost, int cap, int flow, int number):
        to(to), cost(cost), cap(cap), flow(flow), number(number) {}
};

int main() {
    fastIO();
    int n, m, k;
    cin >> n >> m >> k;
    int s = 0, t = n - 1;
    vector<Edge> edges;
    vector<vector<int>> gr(n);
    vector<vector<int>> potential(n, vector<int>(n, 2e9));
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        a--, b--;
        potential[a][b] = potential[b][a] = c;
        potential[a][a] = potential[b][b] = 0;
        gr[a].push_back(edges.size());
        edges.emplace_back(b, c, 1, 0, i + 1);
        gr[b].push_back(edges.size());
        edges.emplace_back(a, -c, 0, 0, i + 1);
        gr[b].push_back(edges.size());
        edges.emplace_back(a, c, 1, 0, i + 1);
        gr[a].push_back(edges.size());
        edges.emplace_back(b, -c, 0, 0, i + 1);
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                potential[i][j] = min(potential[i][j], potential[i][k] + potential[k][j]);
            }
        }
    }
    function<vector<int>(int, int)> dijkstra = [&](int from, int to) -> vector<int> {
        vector<int> dist(n, 2e9);
        vector<pair<int, int>> parent(n, {-1, -1});
        dist[from] = 0;
        set<pair<int, int>> queue = {{dist[from], from}};
        while (!queue.empty()) {
            int v = queue.begin()->second;
            queue.erase(queue.begin());
            for (int u : gr[v]) {
                if (edges[u].cap > edges[u].flow) {
                    if (dist[v] + edges[u].cost + potential[from][edges[u].to] - potential[from][v] < dist[edges[u].to]) {
                        queue.erase({dist[edges[u].to], edges[u].to});
                        dist[edges[u].to] = dist[v] + edges[u].cost + potential[from][edges[u].to] - potential[from][v];
                        parent[edges[u].to] = {v, u};
                        queue.insert({dist[edges[u].to], edges[u].to});
                    }
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            potential[from][i] = dist[i];
        }
        vector<int> ans = {to};
        while (parent[ans.back()].first != -1) {
            ans.push_back(parent[ans.back()].first);
        }
        if (ans.back() != from) return vector<int>();
        reverse(ans.begin(), ans.end());
        vector<int> rightAnswer(ans.size() - 1);
        for (int i = 1; i < ans.size(); ++i) {
            rightAnswer[i - 1] = parent[ans[i]].second;
        }
        return rightAnswer;
    };
    double dist = 0;
    for (int i = 0; i < k; ++i) {
        auto way = dijkstra(s, t);
        if (way.empty()) {
            cout << -1;
            return 0;
        }
        deb(way);
        for (int v : way) {
            dist += edges[v].cost;
            edges[v].flow++;
            edges[v^1].flow--;
        }
    }
    cout << fixed << setprecision(5) << dist / k << endl;
    vector<int> used(n, 0);
    function<vector<int>(int)> getAns = [&](int v) {
        if (v == s) used.assign(n, 0);
        if (v == t) return vector<int>(1, t);
        used[v] = 1;
        for (int u : gr[v]) {
            if (used[edges[u].to]) continue;
            if (edges[u].flow > 0) {
                vector<int> nowAns = getAns(edges[u].to);
                if (nowAns.empty()) continue;
                edges[u].flow--;
                edges[u^1].flow++;
                nowAns.push_back(edges[u].number);
                return nowAns;
            }
        }
        return vector<int>();
    };
    for (int i = 0; i < k; ++i) {
        vector<int> ans = getAns(s);
        reverse(ans.begin(), ans.end());
        ans.pop_back();
        cout << ans.size() << ' ';
        for (int j : ans) {
            cout << j << ' ';
        }
        cout << endl;
    }
}
