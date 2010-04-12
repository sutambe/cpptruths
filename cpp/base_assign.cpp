class Shape { int p; };
class Circle : public Shape { int r; };
class Square : public Shape { int b,w; };

void func (Shape &s1, Shape &s2)
{
	s1 = s2;
}

int main (void)
{
	Circle c;
	Square s;
	func (c, s);
	return 0;
}
