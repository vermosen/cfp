cmake_minimum_required (VERSION 3.13)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME simulate_py 
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME kalman_filter_py
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)