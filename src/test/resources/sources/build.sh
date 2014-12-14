date
time \
g++ -O0 -g3 -Wall -j4 -Wno-deprecated -rdynamic -ldl \
	$( find ../../src/ -name "*.cpp" ) \
	-o sera
