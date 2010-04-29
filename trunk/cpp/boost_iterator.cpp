#include <iostream>
#include <vector>
#include <functional>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

using namespace std;

//F = transformation function, P = predicate, C = container
template<typename F, typename P, typename C>
struct Comprehension
{
private:
	typedef boost::filter_iterator<P, typename C::iterator> FilterIter;
public:
	typedef boost::transform_iterator<F, FilterIter> ComprehensionIterator;

	Comprehension(const F& function, const P& predicate, const C& container):
		mContainer(container),
		mBegin(FilterIter(predicate, mContainer.begin(), mContainer.end()), function),
		mEnd(FilterIter(predicate, mContainer.end(), mContainer.end()), function)
	{
	}
	inline ComprehensionIterator begin(void){return mBegin;}
	inline ComprehensionIterator end(void){return mEnd;}
	
private:
	C mContainer;	
	ComprehensionIterator mBegin,mEnd;

};

int main(void)
{
	vector<int> a;
	//fill up the vector with a bunch of values - count_iterator used for convenience!
	std::copy(boost::counting_iterator<int>(0),	
                  boost::counting_iterator<int>(9),
		  std::back_inserter(a));
	
	typedef boost::function<bool(int)> UnaryPred;
	typedef boost::function<int(int)> UnaryAdd;

	typedef Comprehension<UnaryAdd,UnaryPred,vector<int> > VectorComprehension;
	VectorComprehension comprehension(UnaryAdd(bind(plus<int>(),_1,5)),
                                          UnaryPred(bind(less<int>(),_1,5)),
		        		  a);

	VectorComprehension::ComprehensionIterator begin(comprehension.begin());
	while(begin!=comprehension.end())
	{
		cout << *begin++;		
	}
}
                                                                     
