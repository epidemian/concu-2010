/*
 * exceptions_test.cpp
 *
 *  Created on: Apr 13, 2010
 *      Author: demian
 */

#include "exception.h"
#include "utils.h"

#include <iostream>
#include <vector>

using std::cout;

void test1();
void test2();
void test3();

int exceptionsTest  (int argc, char* argv[])
{
	cout << "Exceptions Test\n\n";

	void ( *tests[] )() = { test1, test2, test3 };
	size_t nTests = ARR_SIZE(tests);

	for (size_t i = 0; i < nTests; ++i)
	{
		try {
			tests[i]();
		}
		catch (std::exception& e)
		{
			cout << e.what() << "\n\n";
		}
	}

	return 0;
}

int divide(int n, int d)
{
	if (d == 0)
		throw Exception("Division by zero");
	else
		return n / d;
}

void recursiveFunc(int n)
{
	if (n > 0)
		recursiveFunc(n -1);
	else
		divide(5, n);
}

string thirdFunction(const string& foobar)
{
	recursiveFunc(3);
	return foobar;
}

float secondFunc(int foo, double bar)
{
	thirdFunction("lala");
	return foo / bar;
}

void firstFunc()
{
	secondFunc(45, -7.3);
}


void test1()
{
	cout << "Test 1: Long stack\n";
	firstFunc();
}


template <typename First, typename Second>
class SuperClass
{
public:
	SuperClass(First f, Second s) { }

	void explode() { method1(); }

private:
	void method1() { method2(3); }
	float method2(int a) { templateMethod<std::vector<int> >(); return 3; }
	template <typename T>
	T templateMethod() { doExplode();  return T(); }
	void doExplode()
	{
		throw Exception("Something went very wrong");
	}

};

void test2()
{
	cout << "Test 2: Parameterized class\n";
	SuperClass<double, std::string> superObject(4, "lalala");
	superObject.explode();
}

static void hiddenExplosion()
{
	throw Exception("ERROR: This will be hard to debug");
}

static void hiddenRecursiveFunc(int n)
{
	if (n) hiddenRecursiveFunc(n - 1);
	else hiddenExplosion();
}

static void hiddenFunc()
{
	hiddenRecursiveFunc(3);
}

void test3()
{
	cout << "Test 3: Static functions\n";
	hiddenFunc();
}



