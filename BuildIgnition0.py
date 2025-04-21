###
 # Author:	Varun Khambhata
 # Created: 12.08.2023
###

build_threads = 6

link_linux = ['glfw', 'GL', 'GLEW']
link_windows = ['glew32', 'opengl32', 'glfw3', 'gdi32']

src = [
	'Ignition0.cpp',
	'Ignition0Core/Camera.cpp',
	'Ignition0Core/ColorImage2D.cpp',
	'Ignition0Core/Cube.cpp',
	'Ignition0Core/DirectionalLight.cpp',
	'Ignition0Core/IgnitionInputs.cpp',
	'Ignition0Core/Light0.cpp',
	'Ignition0Core/Line.cpp',
	'Ignition0Core/LitColor.cpp',
	'Ignition0Core/LitImage.cpp',
	'Ignition0Core/MassRender.cpp',
	'Ignition0Core/Material0.cpp',
	'Ignition0Core/Model.cpp',
	'Ignition0Core/Object0.cpp',
	'Ignition0Core/Plane.cpp',
	'Ignition0Core/PointLight.cpp',
	'Ignition0Core/RenderInfo.cpp',
	'Ignition0Core/Scene.cpp',
	'Ignition0Core/Script0.cpp',
	'Ignition0Core/Sphere.cpp',
	'Ignition0Core/UnlitColor.cpp',
	'Ignition0Core/UnlitImage.cpp',
	'Ignition0Supplement/ImageUtil0.cpp',
	'Ignition0Supplement/VoidMemory0.cpp',
]

######################################################################################################################
import os
import re
import sys
import math
import getopt
import os.path
import platform
import argparse
import threading

thread_exception = threading.Event()
thread_lock = threading.Lock()

outDir = '.out/'

def makeIgnition0GCC(cpp, o, link):
	return 'g++ -DGLEW_STATIC -c ' + cpp + ' -o ' + o + r' -I . -I ./Ignition0Core/external -L.' + link

def makeLinkGCC():
	linkGCC = ''
	for l in link:
		linkGCC += ' -l' + l
	return linkGCC

def run(cmd):
	if os.system(cmd) !=0:
		raise Exception()

def distributeJobs(checkUpdate):
	size = len(src)
	part = int(size / build_threads)
	rem  = int(size % build_threads)

	start = 0
	end   = 0
	compile_thread = []
	linkGCC = ''

	for i in range(0,build_threads):
		start = end
		if rem > 0:
			end += 1
			rem -= 1
		end += part
		t = threading.Thread(target=buildObjs, args=(start, end, linkGCC, checkUpdate))
		t.start()
		compile_thread.append(t)

	oLibList = ' '.join(map(lambda i: outDir + str(i), range(0,end)))
	return compile_thread, oLibList

def buildObjs(start, end, linkGCC, checkUpdate):
	try:
		for i in range(start, end):
			if thread_exception.is_set(): return

			srcPath = 'src/' + str(src[i])
			outPath = outDir + str(i)
			
			if checkUpdate and os.path.isfile(outPath):
				srcTime = os.path.getmtime(srcPath)
				outTime = os.path.getmtime(outPath)
				if srcTime < outTime:
					continue
			with thread_lock: print('- ' + src[i])
			run(makeIgnition0GCC(srcPath, outPath, linkGCC))
	except:
		thread_exception.set()


class QuickBuildCheck:
	def getTimeStamp(path):
		timestamp = os.path.getmtime(path)
		for dirpath, subdinames, filenames in os.walk(path):
			timestamp = max(timestamp, os.path.getmtime(dirpath))
		return timestamp

	def isUpdated(root, buildTime, ignore=[], directoryOnly=False):
		if buildTime < os.path.getmtime(root):
			return True

		ret = False
		for entry in os.scandir(root):
			if(entry.is_dir()):
				if entry.name in ignore:
					if buildTime < os.path.getmtime(entry.path):
						return True
				else:
					ret = QuickBuildCheck.isUpdated(entry.path, buildTime, ignore, directoryOnly)

			if ret:
				return True

			if(entry.is_file() and not directoryOnly):
				if buildTime < os.path.getmtime(entry.path):
					return True

		return ret

	def checkIncludesUpdated(buildTime):
		return buildTime < QuickBuildCheck.getTimeStamp('Ignition0.h') \
				or QuickBuildCheck.isUpdated('Ignition0Supplement', buildTime) \
				or QuickBuildCheck.isUpdated('Ignition0Core', buildTime, ['external'])


	def checkSourceUpdated(buildTime):
		os.chdir('src')
		ret = False
		for f in src:
			if os.path.getmtime(f) > buildTime:
				ret = True
				break
		os.chdir('..')
		return ret


def buildIgnition0(checkUpdate=True):
	if not os.path.exists(outDir):
		os.makedirs(outDir)
		buildTime = 0
	else:
		buildTime = QuickBuildCheck.getTimeStamp(outDir)

	if checkUpdate:
		if buildTime < QuickBuildCheck.getTimeStamp(__file__):
			checkUpdate = False
		elif QuickBuildCheck.checkIncludesUpdated(buildTime):
			checkUpdate = False
		elif QuickBuildCheck.checkSourceUpdated(buildTime):
			checkUpdate = True
		elif os.path.isfile('libIgnition0.a'):
			print('BUILD EXISTS: libIgnition0.a')
			return

	print("BUILDING ...")
	try:
		threads, oLibList = distributeJobs(checkUpdate)
		for t in threads: t.join()
		if thread_exception.is_set(): raise Exception()
		run('ar rcs libIgnition0.a ' + oLibList)
		print('BUILT libIgnition0.a')
	except:
		print("BUILD FAILED")



##### main #####
os_system = platform.system()
if os_system == 'Windows':
    link = link_windows
    executable_prefix = '.\\'
    executable_suffix = '.exe'
elif os_system == 'Linux':
    link = link_linux
    executable_prefix = './'
    executable_suffix = ''
else:
    print(f"{os_system} not supported")
    exit(0)

if len(sys.argv) == 1:
	buildIgnition0()
else:
	parser = argparse.ArgumentParser()
	parser.add_argument('-c', action='store_true', help='clean build')
	parser.add_argument('-a', metavar='', help='cpp file for Ignition0 application')
	args = parser.parse_args()

	buildIgnition0(not args.c)

	if args.a:
		try:
			print('\nBUILDING APPLICATION ...')
			run('g++  -DGLEW_STATIC ' + args.a + ' -o ' + args.a.split('.cpp')[0] + ' -I. -I Ignition0Core/external -L. -L Ignition0Core/external -lIgnition0' + makeLinkGCC())
			print('APPLICATION BUILD FINISHED')

			pattern = r'^(.+)[/\\]([^/\\]+)$'
			match = re.match(pattern, args.a)
			os.chdir(match.group(1))
			run(executable_prefix + match.group(2).split('.cpp')[0] + executable_suffix)
		except:
			print('APPLICATION BUILD FAILED')