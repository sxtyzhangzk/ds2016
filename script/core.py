import os
import sys
import subprocess
#stuList = open("students", "r");

def Prepare(stuName):
	print("Preparing for the test of ", stuName)
	dirname = "./testspace/"
	fileList = [
		"algorithm.hpp",      
		"list.hpp",           
		"queue.hpp",          
		"vector.hpp",
		"deque.hpp",          
		"map.hpp",            
		"stack.hpp",
		"exceptions.hpp",     
		"priority_queue.hpp", 
		"unordered_map.hpp",
	]
	for filename in fileList:
		targetFile = open(dirname + filename, "w")
		targetFile.write("#include \"submit/%s/%s\"" % (stuName, filename))
		targetFile.close()

def BasicTest():
	basicTestCompile = subprocess.Popen(["g++", "-std=c++11", "-O2", "./testspace/vector_basic_test.cc", "-o", "./testspace/vector_basic_test"], stdout = None);
	basicTestCompile.wait()
	if basicTestCompile.returncode != 0:
		return -1
	try:
		result = open("./testres/vector-basic", "w")
	except:
		os.mkdir("testres")
		result = open("./testres/vector-basic", "w")
	basicTest = subprocess.Popen("./testspace/vector_basic_test", stdout = result)
	basicTest.wait()
	if basicTest.returncode != 0:
		return -2
	print("basic test finished")

def AdvanceTest():
	pass

def Benchmark():
	pass

Prepare("Songyu")
if BasicTest() == 0:
	pass
