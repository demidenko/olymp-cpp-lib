/*
	vector<int> a;
	auto next_less = 
		nearest(a, 1, less<int>());
	auto prev_greater_or_equal = 
		nearest(a, -1, greater_equal<int>());
*/

template<typename T, typename C>
vector<int> nearest(const vector<T> &v, int dir, const C &cmp){
	assert(abs(dir) == 1);
	int n = size(v), fin = dir>0 ? n : -1;
	vector<int> res(n, fin), s;
	for(int i = dir>0 ? 0 : n-1; i!=fin; i+=dir){
		while(!empty(s) && cmp(v[i],v[s.back()])){
			res[s.back()] = i;
			s.pop_back();
		}
		s.push_back(i);
	}
	return res;
}
