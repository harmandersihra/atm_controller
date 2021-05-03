
output: *.cpp *.hpp
	g++ *.cpp -o output.exe

clean:
	rm -rf output.exe