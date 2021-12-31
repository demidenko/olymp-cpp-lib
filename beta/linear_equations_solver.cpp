template<class T>
struct linear_equations_solver {
	linear_equations_solver(size_t n): n(n) {}
	
	void add(const vector<T> &line, const T &value) {
		assert(size(line) == n);
		a.push_back(line);
		b.push_back(value);
	}
	
	optional<vector<T>> solve() {
		vector<T> res(n, 0);
		for(size_t r=0, c=0; c<n && r<size(a); ++c) {
			bool zero_column = true;
			for(size_t j=r; j<size(a); ++j) if(a[j][c] != 0) {
				zero_column = false;
				swap(a[j], a[r]);
				swap(b[j], b[r]);
				break;
			}
			if(zero_column) continue ;
			T u = a[r][c];
			assert(u != 0);
			for(size_t j=0; j<c; ++j) assert(a[r][j] == 0);
			for(size_t j=c; j<n; ++j) a[r][j] /= u;
			b[r] /= u;
			for(size_t i=r+1; i<size(a); ++i) if(u = a[i][c]; u != 0) {
				for(size_t j=c; j<n; ++j) a[i][j] -= a[r][j] * u;
				b[i] -= b[r] * u;
			}
			++r;
		}
		
		for(size_t i=size(a); i--; ) {
			size_t k = n;
			for(size_t j=0; j<n; ++j) if(a[i][j] != 0) {
				k = j;
				break;
			}
			if(k == n) {
				if(b[i] == 0) continue ;
				else return nullopt;
			}
			res[k] = b[i];
			for(size_t j=k+1; j<n; ++j) res[k] -= res[j] * a[i][j];
			res[k] /= a[i][k];
		}
		
		return res;
	}
	
	private:
	size_t n;
	vector<vector<T>> a;
	vector<T> b;
};
