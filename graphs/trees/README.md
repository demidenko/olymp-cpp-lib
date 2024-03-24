# Centroid Decomposition
```c++
centriod_decomposition(tree, [&](auto &g, size_t centroid, size_t sizeof_subtree) {
	//do work on subtree here
});
```
Calls lambda for each subtree created during decomposition. Graph `g` have same type as `tree`.

It is allowed to go to all vertices connected with `centroid` (by dfs/bfs), there will be `sizeof_subtree` such vertices.

If time complexity of lambda is not more than $O(S)$ then total time is $O(n\log n)$. ($O(S\log S)$ → $O(n\log ^2n)$, $O(S^2)$ → $O(n^2)$).

### Offline version / Centroid tree

<details>
<summary>To construct offline data structure use following snippet</summary>

It returns pair of vectors `level` and `centroid_parent`. Levels numbered from 0 and `level[v] = level[cpar[v]]+1`.
```c++
auto centriod_decomposition_offline(const auto &g) {
	vector<size_t> cpar(size(g), -1), w;
	centriod_decomposition(g, [&](auto &g, size_t centroid, size_t sizeof_subtree) {
		w.push_back(centroid);
		for(size_t i : g[centroid]) {
			while(cpar[i] != -1) i = cpar[i];
			cpar[i] = centroid;
		}
	});
	vector<uint16_t> level(size(g));
	if(size_t i=size(g)) for(--i; i--; ) level[w[i]] = level[cpar[w[i]]] + 1;
	return pair{level, cpar};
}
```
</details>

</br>

# Heavy-light Decomposition
```c++
heavy_light_decomposition hld(g); //root = 0 by default, otherwise hld(g, root)
```
Build takes $O(n)$ time and memory.

Permutes vertices such vertex `v` now in position `hld.index(v)`.

### Path Queries
Decomposes path from `a` to `b` into $O(\log n)$ ranges from permutation.
```c++
hld.decompose(a, b, [&](size_t l, size_t r) {
	//range [l, r)
});
```

Ordered path decomposition. All ranges called in correct order as moving from `a` to `b`.
```c++
hld.decompose_ordered(a, b, [&](size_t l, size_t r, bool reversed) {
	//range [l, r)
	if(reversed) // assume range from r-1 to l
	else //usual from l to r-1
});
```

To exclude LCA(a,b) from decomposition pass `exclude_lca = true` as last parameter
```c++
hld.decompose(a, b, [&](size_t l, size_t r) { ... }, true);
```

### LCA
To find lowest common ancestor in $O(\log n)$ use `c = hld.lca(a, b)`.

But as bonus both `decompose` and `decompose_ordered` returns LCA too.

### Decomposition by height
`heavy_light_decomposition(g, root, true)` builds Longest-path decomposition (i.e. heavy edge going to subtree with maximum height). 
Convinient for linear time DP on tree. 
Methods `decompose`/`lca` works correct but in $O(\sqrt n)$ on [worst case](https://codeforces.com/blog/entry/75410).

</br>

# Tree DP Root Each
Calculates DP for each vertex as root of given forest (not only connected tree).
```c++
vector<T> res = tree_dp_root_each<T>(g,
	[](size_t v) { ... }, //single vertex
	[](T dv, T di, auto &edge) { ... } //link function
);
```
`link` means subtree rooted at `i` linked to subtree rooted at `v` (by edge `[v, i]`).

`edge` exists in `g` and can be used as `auto [v, i, ...] = edge`.

`dv` is DP of some subtree with root `v` (same for `di` and `i`).

`link` will be called $O(n\log n)$ times in worst case (more precisely $O(\log(deg_v))$ for each edge [v, i]).
`single` will be called exaclty 2 times for each vertex.

<details>
<summary>Code examples for problems</summary>

Diameter of forest
```c++
auto res = tree_dp_root_each<int>(g,
	[](size_t v) { return 1; },
	[](int dv, int di, ...) { return max(dv, di+1); }
);
int diam = *max_element(begin(res), end(res));
```

[codeforces 1324F](https://codeforces.com/contest/1324/problem/F): Best subtree by balance
```c++
auto res = tree_dp_root_each<int>(g,
	[&](size_t v){ return a[v] ? 1 : -1; },
	[](int dv, int di, ...) { return dv + max(di, 0); }
);
```

[atcoder](https://atcoder.jp/contests/dp/tasks/dp_v): Count of black connected subtrees
```c++
auto res = tree_dp_root_each<mint>(g,
	[](size_t v) { return 1; },
	[](mint dv, mint di, ...) { return dv * (di+1); }
);
```

[codeforces 960E](https://codeforces.com/contest/960/problem/E): +- sum of all paths
```c++
struct S {
	mint sum, cnt; //sum/count of paths started from root
};

auto res = tree_dp_root_each<S>(g,
	[&](size_t v) { return S{a[v], 1}; },
	[&](S dv, S di, auto &edge) {
		auto [v, i] = edge;
		return S{
			dv.sum + a[v] * di.cnt - di.sum,
			dv.cnt + di.cnt
		};
	}
);
```
</details>
