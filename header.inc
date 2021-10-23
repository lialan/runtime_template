#include <iostream>
#include <typeinfo>
template <typename T1, typename T2>
void foo(T1 *a, T2 *b) {
	std::cout<< "First type name:" << typeid(a).name() << std::endl;
	std::cout<< "Second type name:" << typeid(b).name() << std::endl;
}

