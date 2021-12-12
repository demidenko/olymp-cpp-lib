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
graph_t<ll, int> g(n);
g.add_edge(i, j, weight, id);
for(auto [i, w, id] : g[v]) // full edge {v, i, w, id}
for(size_t i : g[v]) // only vertex
```
