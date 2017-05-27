#include "retracement_level.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "cpp_call_python.hpp"
#include <tuple>

using namespace std;

int main()
{
	CppCallPy cpp_call_py;

	vector<int> x = {1, 2, 3, 4, 5};
	vector<int> y = {1, 2, 3, 4, 5};
	
	PyObject* arg1 = Py_BuildValue(("O"), vector_to_py_list(x));
	PyObject* arg2 = Py_BuildValue(("O"), vector_to_py_list(y));	
	PyObject* args = PyTuple_New(2);
	PyTuple_SetItem(args, 0, arg1);
        PyTuple_SetItem(args, 1, arg2);

	double slop, shift;
	PyObject* result = PyList_AsTuple(cpp_call_py.call("linear_regression", "linear_regression", args));
	//slop = PyFloat_AsDouble(PyList_GetItem(result, 0));
	//shift = PyFloat_AsDouble(PyList_GetItem(result, 1));
	//cout << "Slop:" << slop << endl << "Shift:" << shift << endl;
	/*RetracementLevel rl;
	rl.init();
	unordered_map<string, vector<pair<string, double>>> symbol_closes = rl.test_data();

	double max_close = -1;
	for(auto it = symbol_closes["GOOG"].begin(); it != symbol_closes["GOOG"].end(); ++it)
	{
		if(max_close < it->second)
			max_close = it->second;
		cout << it->first << ": " << it->second << endl;
	}
	cout << "finish getting data" << endl;

	cout << "max:" << max_close << endl;
	cout << symbol_closes["GOOG"][rl.test_find_peak("GOOG")].second << endl;*/
	return 0;
}
