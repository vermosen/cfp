cmake_minimum_required (VERSION 3.13)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_build_model_py
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_simulate_py
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_kalmanfilter_py
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_kalmansmoother_py
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

add_py_unittest(
	EXEC ${PYTHON_EXECUTABLE} 
	NAME test_em_py
	CLASS testPycalc 
	FILE test
	PYTHONPATH ${PYTHONPATH_TESTSUITE}
	DEPENDS cfp.python.package.import
)

