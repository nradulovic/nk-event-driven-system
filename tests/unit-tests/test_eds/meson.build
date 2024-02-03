project('primitive_unit_tests', ['cpp', 'c'])

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

cut_src = files('../../../source/eds.c')
test_src = files('test_eds.cpp')

e = executable(
	'unit_testing',
	cut_src + test_src,
	include_directories: [
		'../../../portable/gcc-any-any-linux',
		'../../../source'
	],
	dependencies : [
		gtest_dep,
		gmock_dep
	]
)
