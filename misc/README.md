# Nearest
```c++
vector<int> a;
auto next_less = nearest<1>(a, less{});
auto prev_greater_or_equal = nearest<-1>(a, greater_equal{});
```

# Parallel Binary Search
Example of usage in solution of task [New Roads Queries](https://cses.fi/problemset/task/2101/)
```c++
size_t queries_count = size(queries), versions_count = size(edges);
auto res_bs = parallel_binary_search(queries_count, versions_count, [&](auto vc) {
	dsu g(n);
	vc.pred = [&](size_t qi) {
		auto [a, b] = queries[qi];
		return g[a] != g[b];
	};
	for(auto [i, j] : edges) {
		g.unite(i, j);
		vc.commit();
	}
});
```
As result, like in `std::partition_point`, `res_bs[i]` will be first version where `pred` returns false for i-th query, or will be `versions_count+1` if `pred` always returns true.

Versions numbered from 0 to `versions_count`, where 0-th version is state before first commit.

`vc.pred` must be set exactly once and before first commit.

There must be exactly `versions_count` commits.
