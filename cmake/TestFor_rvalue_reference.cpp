class TestRValueRef
{
public:
	TestRValueRef() : _PH(0) {}
	TestRValueRef(TestRValueRef&& t) : _PH(1)
	{
		swap(t);
	}
	void swap(TestRValueRef& t)
	{
		int _Temp = _PH;
		_PH = t._PH;
		t._PH = _Temp;
	}
private:
	int _PH;
};

int main(int argc, const char** argv)
{
	TestRValueRef a;
	TestRValueRef b(a);
	return 0;
}