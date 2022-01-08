# Heavy-light Decomposition
Build in O(n) time and memory.

```c++
heavy_light_decomposition hld(g); //root = 0 by default, otherwise hld(g, root)
```

Permutes vertices such vertex `v` now in position `hld.index(v)` (in range `[0, n)`)

Path Queries
```c++
hld.query_path(v, t, [&](size_t l, size_t r) {
  //range [l, r)
});
```

Strict path decompose. All ranges called in correct order such we moving from `v` to `t`.
```c++
hld.query_path_strict(v, t, [&](size_t l, size_t r, bool reversed) {
  //range [l, r)
  if(reversed) // assume range from r-1 to l
  else //usual from l to r-1
});
```

## LCA in O(logn).
Bonus `hld.lca(a, b)`.

## Decomposition by height
`heavy_light_decomposition<false>` builds Longest-path decomposition (i.e. heavy edge going to subtree with maximum heigth). 
Convinient for linear time DP on tree. 
Methods `query_path`/`lca` works correct but in O(sqrt(n)) on [worst case](https://codeforces.com/blog/entry/75410).
