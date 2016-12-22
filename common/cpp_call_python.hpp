#ifndef CPP_CALL_PYTHON_HPP
#define CPP_CALL_PYTHON_HPP

#include "Python.h"
#include <string>

using namespace std;

class CppCallPy
{
private:
public:
	CppCallPy()
        {
                Py_Initialize();
		string path = "/home/lishuo/Desktop/quant_studio/python_component";
        	string chdir_cmd = string("sys.path.append(\"") + path + "\")";
	        const char* cstr_cmd = chdir_cmd.c_str();
        	PyRun_SimpleString("import sys");
	        PyRun_SimpleString(cstr_cmd);
        }

	PyObject* call(string module_name, string function_name, PyObject* args)
	{
		PyObject* moduleName = PyString_FromString(module_name.c_str());
	        PyObject* pModule = PyImport_Import(moduleName);
        	if (!pModule)
        	{
                	cout << "[ERROR] Python get module failed." << endl;
                	return 0;
	        }
		
	        PyObject* pv = PyObject_GetAttrString(pModule, function_name.c_str());
        	if (!pv || !PyCallable_Check(pv))
	        {
        	        cout << "[ERROR] Can't find funftion (test_add)" << endl;
                	return 0;
	        }
		
        	PyObject* pRet = PyObject_CallObject(pv, args);
		
	        if (!pRet)
		{
                	cout <<  "falied to call function " + function_name << endl;
			return 0;
		}
		
		return pRet;
	}

	~CppCallPy()
        {
                Py_Finalize();
        }

};

// assume that only numeric calculation need passed to python component
// need to figure out a way to pass any kind of data later
template<class T>
PyObject* vector_to_py_list(vector<T> double_vector)
{
        PyObject* l = PyList_New(double_vector.size());
        for(int i = 0; i < double_vector.size(); ++i)
        {
		PyList_SET_ITEM(l, i, PyFloat_FromDouble(double(double_vector[i])));
        }

	return Py_BuildValue(("O"), l);
}



#endif
