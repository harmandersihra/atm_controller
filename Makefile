
output: *.cpp *.hpp output.exe
	g++ *.cpp -o output.exe

clean:
	rm -rf output.exe