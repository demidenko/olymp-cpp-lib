# DSU
```c++
dsu g(n);
g.unite(i, j); //true iff i and j was disconnected
if(g[a] == g[b]) //check connection by compare leaders
g.size(v); //size of component
```

# Graph
creating graph of size n
```c++
graph g(n);
assert(size(g) == n);
```
adding edges
```c++
g.add_edge(i, j);
g.add_dir_edge(from, to);
```
iterating
```c++
for(size_t i : g[v])
for(int i : g[v])
```

graphs with edges with information
```c++
graph_t<int64_t, int> g(n);
g.add_edge(i, j, weight, id);
for(auto [i, w, id] : g[v]) //full edge {v, i, w, id}
for(size_t i : g[v]) //only vertex
```

# 2-SAT

```c++
sat2 a(n);
a[i] || !a[j]; //set rule
a[i] = true; //instead of a[i] || a[i]
if(auto res = a.solve()) //solution is *res
else //no solution
```
