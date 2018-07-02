#include <new>
#include <memory>
#include <stdio.h>
#include <vector>

using namespace std;
#define MAXN  9000400

int top_6;
vector<int> g_6[MAXN];
int d_6[MAXN], low_6[MAXN], scc_6[MAXN];
bool stacked_6[MAXN];
int s_6[MAXN];
int ticks_6, current_scc_6;
void tarjan_6(int u){
  d_6[u] = low_6[u] = ticks_6++;
  s_6[++top_6] = u; //	 s.push(u);
  stacked_6[u] = true;
  const vector<int> &out = g_6[u];
  for (int k=0, m=out.size(); k<m; ++k){
    const int &v = out[k];
    if (d_6[v] == -1){
      tarjan_6(v);
      low_6[u] = min(low_6[u], low_6[v]);
    }else if (stacked_6[v]){
      low_6[u] = min(low_6[u], low_6[v]);
    }
  }
  if (d_6[u] == low_6[u]){
    int v;
    do{
      v = s_6[top_6--]; //	    v = s.top();
      //s.pop();
      stacked_6[v] = false;
      scc_6[v] = current_scc_6;
    }while (u != v);
    current_scc_6++;
  }
}

