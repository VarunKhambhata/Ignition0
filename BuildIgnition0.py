###
 # Author:	Varun Khambhata
 # Created: 12.08.2023
###

build_threads = 4

link = ['glfw', 'GL', 'GLEW']

src = [	
	'Ignition0.cpp',	
	'Ignition0Core/Camera.cpp',	
	'Ignition0Core/Cube.cpp',
	'Ignition0Core/Material0.cpp',
	'Ignition0Core/Object0.cpp',
	'Ignition0Core/Plane.cpp',
	'Ignition0Core/Scene.cpp',
	'Ignition0Core/Script0.cpp',
	'Ignition0Core/UnlitColor.cpp',
	'Ignition0Core/UnlitImage.cpp',
]

#############################################################################################
import os
import re
import sys
import math
import getopt
import os.path
import argparse
import threading

thread_exception = threading.Event()
thread_lock = threading.Lock()


def makeIgnition0GCC(cpp, o, link):
	return 'g++ -c src/' + cpp + ' -o' + o + r' -I . -I ./Ignition0Core/glm -L.' + link

def makeLinkGCC():
	linkGCC = ''
	for l in link:
		linkGCC += ' -l' + l
	return linkGCC

def run(cmd):
	if os.system(cmd) !=0:
		raise Exception()

def distributeJobs():
	size = len(src)
	part = size / build_threads
	rem  = size % build_threads

	start = 0
	end  = 0
	compile_thread = []
	linkGCC = makeLinkGCC()

	for i in range(0,build_threads):
		start = end
		if rem > 0:
			end += 1
			rem -= 1
		end += part
		t = threading.Thread(target=buildObjs, args=(start, end, linkGCC))
		t.start()
		compile_thread.append(t)

	oLibList = ' '.join(map(str, range(0, end)))
	return compile_thread, oLibList

def buildObjs(start, end, linkGCC):	
	try:
		for i in range(start, end):
			if thread_exception.is_set(): return
			with thread_lock: print('-' + src[i])
			run(makeIgnition0GCC(src[i], str(i), linkGCC))
	except:
		thread_exception.set()


def buildIgnition0():
	print("\nBUILDING ...")
	try:
		threads, oLibList = distributeJobs()
		for t in threads: t.join()
		if thread_exception.is_set(): raise Exception()
		run('ar rcs libIgnition0.a ' + oLibList)
		print('BUILT libIgnition0.a')
	except:
		print("BUILD FAILED")
	finally:
		run('rm -f ' + oLibList)	



##### main #####
if len(sys.argv) == 1:
	buildIgnition0()
else:
	parser = argparse.ArgumentParser()
	parser.add_argument('-c', action='store_true', help='clean build')
	parser.add_argument('-a', metavar='', help='cpp file for Ignition0 application')
	args = parser.parse_args()

	if args.c:
		buildIgnition0()
	if args.a:
		if not os.path.isfile('libIgnition0.a'):
			buildIgnition0()
		try:
			print('\nBUILDING APPLICATION ...')
			run('g++ ' + args.a + ' -o ' + args.a.split('.cpp')[0] + r' -I./ -I./Ignition0Core/glm -L. -lIgnition0' + makeLinkGCC() + ' -no-pie')
			print('APPLICATION BUILD FINISHED')

			pattern = r'^(.+)\/([^\/]+)$'
			match = re.match(pattern, args.a)
			os.chdir(match.group(1))
			run('./' + match.group(2).split('.cpp')[0])
		except:
			print('APPLICATION BUILD FAILED')
