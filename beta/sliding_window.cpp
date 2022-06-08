template<typename T, const T& f(const T&, const T&) = std::min<T> >
struct sliding_window {
	constexpr static T neutral = [](T m,T M){return f(m,M)==m?M:m;}(numeric_limits<T>::min(),numeric_limits<T>::max());
	
	sliding_window(): rall(neutral) {}

	void push_back(const T &x) {
		r.push_back(x);
		rall = f(rall, x);
	}

	void pop_front() {
		if(l.empty()){
			for(T m = neutral; !r.empty();){
				m = f(m, r.back());
				l.push_back(m);
				r.pop_back();
			}
			rall = neutral;
		}
		l.pop_back();
	}

	T current() const {
		if(!l.empty()) return f(rall, l.back());
		return rall;
	}
	
	size_t size() const { return l.size() + r.size(); }
	bool empty() const { return l.empty() && r.empty(); }
	
	private:
	vector<T> l, r;
	T rall;
};
