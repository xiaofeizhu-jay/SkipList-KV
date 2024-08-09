#include<bits/stdc++.h>
using namespace std; 

int n;
const int N = 1e5+10;
vector<vector<int>> tree(N); 
vector<int> values(N); 

int longestChain(vector<vector<int>>& tree, vector<int>& values) { 
    int n = tree.size(); vector<int> dp(n, 0); 
    function<int(int)> dfs = [&](int node) { 
        int max_length = 0; 
        for (int child : tree[node]) { 
            if (values[node] <= values[child]) { 
                max_length = max(max_length, dfs(child) + 1); 
                } 
            } 
            dp[node] = max_length; return max_length; 
        }; 
        for (int i = 0; i < n; i++) { 
            dfs(i); 
        } 
        int result = *max_element(dp.begin(), dp.end()) + 1; 
        return result; 
} 

int main() { 
    int n; 
    cin >> n; 
    for (int i = 0; i < n; i++) { 
        cin >> values[i]; 
    } 
    for (int i = 0; i < n - 1; i++) { 
        int u, v; 
        cin >> u >> v; // 将节点编号从1到n转换为从0到n-1 
        u--; 
        v--; 
        tree[u].push_back(v); 
        tree[v].push_back(u); 
    } 
    int result = longestChain(tree, values); 
    // int result = 0;
    cout << "最长链的结点数: " << result << endl; 
    return 0; 
}