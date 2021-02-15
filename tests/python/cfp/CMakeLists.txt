cmake_minimum_required (VERSION 3.13)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME print_param 
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_simulate 
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_filter 
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_smoother 
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_emax 
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_emax_debug
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_predict 
	CLASS testCfp 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)
