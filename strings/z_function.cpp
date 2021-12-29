vector<size_t> z_function(auto first, auto last) {
	size_t n = last - first;
	vector<size_t> z(n, 0);
	for(size_t i=1, j=0; i<n; ++i) {
		if(j+z[j] >= i) z[i] = min(z[i-j], j+z[j]-i);
		while(i+z[i] < n && *(first+z[i]) == *(first+i+z[i])) ++z[i];
		if(i + z[i] > j + z[j]) j = i;
	}
	z[0] = n;
	return z;
}
