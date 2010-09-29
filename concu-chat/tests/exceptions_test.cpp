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
using std::cerr;

void test1();
void test2();
void test3();
void test4();

int main(int, char**)
{
	cout << "Exceptions Test\n\n";

	void ( *tests[] )() = { test1, test2, test3, test4 };
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

string thirdFunction(const string& foobar = "default parameter")
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
	cout << "Test 1: Normal functions, long stack\n";
	firstFunc();
}

class Explosion
{
public:
	Explosion(int a, float lol) { method1(lol, a); }

private:
	float method1(float, int n) { return method2(n); }
	float method2(int what, char the = 't', string fuck = "foo")
	{ return what + the + explode(fuck); }
	float explode(string lol)
	{
		throw Exception(lol + "Booooooooom!");
	}

};

void test2()
{
	cout << "Test 2: Class methods\n";
	Explosion e(1, 2);
}


template <typename First, typename Second>
class SuperClass
{
public:
	SuperClass(First, Second) { }

	void explode() { method1(); }

private:
	void method1() { method2(3); }
	float method2(int) { templateMethod<std::vector<int> >(); return 3; }
	template <typename T>
	T templateMethod() { doExplode();  return T(); }
	void doExplode()
	{
		throw Exception("Something went very wrong");
	}

};

void test3()
{
	cout << "Test 3: Parameterized class\n";
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

void test4()
{
	cout << "Test 4: Static functions\n";
	hiddenFunc();
}


