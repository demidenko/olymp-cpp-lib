# Centroid Decomposition
```c++
centriod_decomposition(tree, [&](auto &g, size_t centroid, size_t sizeof_subtree) {
  //do work on subtree here
});
```
Calls lambda for each subtree created during decomposition. Graph `g` have same type as `tree`.

You can freely go to all vertices connected with `centroid` (by dfs/bfs), there are `sizeof_subtree` such vertices.

If work in lambda takes not more than O(sizeof_subtree) then total time is O(nlogn).


# Heavy-light Decomposition

```c++
heavy_light_decomposition hld(g); //root = 0 by default, otherwise hld(g, root)
```
Build takes O(n) time and memory.

Permutes vertices such vertex `v` now in position `hld.index(v)`.

### Path Queries
Decomposes path from `a` to `b` into O(logn) ranges from permutation.
```c++
hld.query_path(a, b, [&](size_t l, size_t r) {
  //range [l, r)
});
```

Strict path decomposition. All ranges called in correct order as moving from `a` to `b`.
```c++
hld.query_path_strict(a, b, [&](size_t l, size_t r, bool reversed) {
  //range [l, r)
  if(reversed) // assume range from r-1 to l
  else //usual from l to r-1
});
```

To exclude LCA(a,b) from decomposition pass `ignore_lca = true` as last parameter
```c++
hld.query_path(a, b, [&](size_t l, size_t r) { ... }, true);
```

### LCA
To find lowest common ancestor in O(logn) use `c = hld.lca(a, b)`.

But as bonus both `query_path` and `query_path_strict` returns LCA too.

### Decomposition by height
`heavy_light_decomposition<false>` builds Longest-path decomposition (i.e. heavy edge going to subtree with maximum heigth). 
Convinient for linear time DP on tree. 
Methods `query_path`/`lca` works correct but in O(sqrt(n)) on [worst case](https://codeforces.com/blog/entry/75410).
