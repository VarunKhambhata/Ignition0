###
 # Author:	Varun Khambhata
 # Created: 12.08.2023
###

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

build_threads = 0

#############################################################################################
import os
import sys
import math
import getopt
import os.path

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
	for i in range(0,build_threads):
		start = end
		if rem > 0:
			end += 1
			rem -= 1
		end += part
		print(start, end)

def buildIgnition0():
	oLibList = ''
	try:
		print("BUILDING ...")
		linkGCC = makeLinkGCC()

		for o, cpp in enumerate(src):
			print(cpp)
			o = str(o)
			oLibList += o + ' '
			run(makeIgnition0GCC(cpp, o, linkGCC))
		run('ar rcs libIgnition0.a ' + oLibList)
		print('BUILT libIgnition0.a')
	except:
		print("BUILD FAILED")
	finally:
		run('rm ' + oLibList)



### main ###
argumentList = sys.argv[1:]


if len(argumentList) > 0:
	if not os.path.isfile('libIgnition0.a'):
		buildIgnition0()	
	try:
		print('BUILDING APPLICATION ...')
		run('g++ ' + argumentList[0] + ' -o ' + argumentList[0].split('.cpp')[0] + r' -I./ -I./Ignition0Core/glm -L. -lIgnition0' + makeLinkGCC() + ' -no-pie')
		print('APPLICATION BUILD FINISHED')
		run('./'+argumentList[0].split('.cpp')[0])
	except:
		print('APPLICATION BUILD FAILED')

else:
	buildIgnition0()