/*
	persistent deque by babin74
*/

template<typename T>
struct persique {
	
	size_t sz;
	size_t size() { return sz; }
	
	persique *p0, *p1;
	T value;
	
	persique() {
		sz = 0;
	}
	
	persique(const T &x) {
		p1 = p0 = nullptr;
		value = x;
		sz = 1;
	}
	
	persique(persique *q0, persique *q1) {
		p0 = q0;
		p1 = q1;
		sz = p0->sz + p1->sz;
	}
	
	const T operator[](size_t i) {
		assert(sz!=0);
		assert(i<sz);
		if(sz == 1) return value;
		if(i%2==0) return p0->operator[](i/2);
		return p1->operator[](i/2);
	}
	
	persique *set_value(size_t i, const T &x){
		assert(sz!=0);
		assert(i<sz);
		if(sz == 1) return new persique(x);
		if(i%2==0) return new persique(p0->set_value(i/2, x), p1);
		return new persique(p0, p1->set_value(i/2, x));
	}
	
	persique* push_back(const T &x) {
		if(sz == 0) return new persique(x);
		if(sz == 1) return new persique(this, new persique(x));
		if(sz%2==0) return new persique(p0->push_back(x), p1);
		return new persique(p0, p1->push_back(x));
	}
	
	persique* push_front(const T &x){
		if(sz == 0) return new persique(x);
		if(sz == 1) return new persique(new persique(x), this);
		return new persique(p1->push_front(x), p0);
	}
	
	persique* pop_back(){
		assert(sz!=0);
		if(sz == 1) return new persique();
		if(sz == 2) return p0;
		if(sz%2 == 0) return new persique(p0, p1->pop_back());
		return new persique(p0->pop_back(), p1);
	}
	
	persique* pop_front(){
		assert(sz!=0);
		if(sz == 1) return new persique();
		if(sz == 2) return p1;
		return new persique(p1, p0->pop_front());
	}
};