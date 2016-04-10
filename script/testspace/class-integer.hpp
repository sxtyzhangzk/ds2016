class Integer {
private:
	int data;
public:
	Integer(const int &value) : data(value) {}
	Integer(const Integer &other) : data(other.data) {}
};