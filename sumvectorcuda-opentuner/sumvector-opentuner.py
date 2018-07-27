#!/usr/bin/env python
#
# Autotune flags to g++ to optimize the performance of apps/raytracer.cpp
#
# This is an extremely simplified version meant only for tutorials
#
import adddeps  # fix sys.path
import math
import re

import opentuner
from opentuner import ConfigurationManipulator
from opentuner import EnumParameter
from opentuner import IntegerParameter
from opentuner import MeasurementInterface
from opentuner import Result

# Formato de entrada
# nvprof --metrics gld_efficiency ./sumvector-cuda 0 1024 1024 1 42 1 1 44040192 3 0

BLOCO_PARAMETROS = [
	('kernel', 0, 0), 
	('n', 65568, 65568),
	('gpuId', 0, 0)  
]

BLOCO_PARAMETROS_CONFIGS = [ 'config' ]

def read_file_configs():
  #file_sumvector = open('/home/projetocuda/Documentos/tcc_cuda_opentuner_joao/wscad/gen-configs/saida_sumvector-65568.txt','r')
  file_sumvector = open('/home/joao/tcc_cuda_opentuner_joao/wscad/gen-configs/saida_sumvector-65568.txt','r')
  list_configs = []
  for linha in file_sumvector:
    list_configs.append(linha)
  #print list_configs
  return list_configs


class SumVectorTuner(MeasurementInterface):

	def manipulator(self):
		"""Define the search space by creating a
		ConfigurationManipulator
		"""
		list_configs = []
		list_configs = read_file_configs()
		manipulator = ConfigurationManipulator()
		print "Executing manipulator"
		for param, mini, maxi in BLOCO_PARAMETROS:
			#print "param: ", param, " mini: ", mini, " maxi: ", maxi
			manipulator.add_parameter(IntegerParameter(param, mini, maxi))

		# E preciso gerar as configuracoes validas com o run.c.
		# manipulator.add_parameter(EnumParameter(param, [ 'gx:1024, gy:1, gz:1, bx:1, by:1, bz:1, ', 'gx:32, gy:32, gz:1, bx:1, by:1, bz:1, ' ]))
		for param in BLOCO_PARAMETROS_CONFIGS:
			manipulator.add_parameter(EnumParameter(param,list_configs))
		return manipulator

	def run(self, desired_result, input, limit):
		"""
		Compile and run a given configuration then
		return performance
		"""
		# cfg = desired_result.configuration.data

		while True:
			# Configuration:  {'kernel': 0, 'gpuId': 0, 'config': 'gx:1024, gy:1, gz:1, bx:1, by:1, bz:1, ', 'funcId': 7}
			configuration = desired_result.configuration.data
			print "Configuration: ", configuration
			cfg = { match.group(1):match.group(2) for match in re.finditer(r"([^:]+):(\S+)\s*,[ ']", configuration['config'])}
			print "CFG: ", cfg
			confBlock = int(cfg['bx']) * int(cfg['by']) * int(cfg['bz'])
			confGrid =  int(cfg["'gx"]) * int(cfg['gy']) * int(cfg['gz'])
			config = confBlock * confGrid
			print "ConfBlock "+ str(confBlock)
			print "ConfGrid " + str(confGrid)
			if((confBlock <= 1024) and (confBlock % 32 == 0) and (config == n)):
				break
			else:
				return Result(time=FAIL_PENALTY)

		# print "desired: " + str(desired_result.configuration.data)
		# print "CFG: ", cfg
		print "compiled: ", 'true' if compiled else 'false'
		if not compiled:
			print "Compiling the program..."
			gcc_cmd = 'nvcc -I /usr/local/cuda/include -L /usr/local/cuda/lib64 -ccbin=g++-4.9 src/sumvector.cu -lcuda -lm -o sumvector-cuda'
			compile_result = self.call_program(gcc_cmd)
			assert compile_result['returncode'] == 0
			print " OK.\n"
			global compiled
			compiled = not compiled
		run_cmd = 'nvprof --metrics inst_executed ./sumvector-cuda'

		#print "TESTE:" + " " + str(cfg['gx']) + " " + str(cfg['gy']) + " " + str(cfg['gz']) + str(cfg['bx']) + " " + str(cfg['by']) + " " + str(cfg['bz'])
		# confBlock = cfg['bx'] * cfg['by'] * cfg['bz']
		# confGrid =  cfg['gx'] * cfg['gy'] * cfg['gz']
		# config = confBlock * confGrid
		# print "confBlock: ", confBlock
		# print "confGrid: ", confGrid
		#print "config: ", config

		# Evict kernel divergence, blocks with multiply warp size.
		#print "Test: ", "True" if((confBlock <= 1024) and (confBlock % 32 == 0)) else "False"
		print "Antes do IF"
		if((confBlock <= 1024) and (confBlock % 32 == 0) and (config == n)):
			dimBlock = 0
			dimGrid = 0
			# Test of quantity of block dimensions are used.
			# a if test else b
			dimBlock += 1 if(int(cfg['bx']) > 1) else 0
			dimBlock += 1 if(int(cfg['by']) > 1) else 0
			dimBlock += 1 if(int(cfg['bz']) > 1) else 0
			if(dimBlock == 0):
				dimBlock = 1

			# Test of quantity of grid dimensions are used.
			dimGrid += 1 if(int(cfg["'gx"]) > 1) else 0
			dimGrid += 1 if(int(cfg['gy']) > 1) else 0
			dimGrid += 1 if(int(cfg['gz']) > 1) else 0
			if(dimGrid == 0):
				dimGrid = 1
			
			if(dimGrid == 1):
				cfg['funcId'] =  dimGrid + dimBlock - 2
			if(dimGrid == 2):
				cfg['funcId'] =  dimGrid + dimBlock + 0
			if(dimGrid == 3):
				cfg['funcId'] =  dimGrid + dimBlock + 2
			
			run_cmd += ' {0}'.format(configuration['kernel'])
			run_cmd += ' {0}'.format(cfg["'gx"])
			run_cmd += ' {0}'.format(cfg['gy'])
			run_cmd += ' {0}'.format(cfg['gz'])
			run_cmd += ' {0}'.format(cfg['bx'])
			run_cmd += ' {0}'.format(cfg['by'])
			run_cmd += ' {0}'.format(cfg['bz'])
			run_cmd += ' {0}'.format(configuration['n'])
			run_cmd += ' {0}'.format(configuration['gpuId'])

			print "Running command line: ", run_cmd
			#print "CFG->funcId: " +  str(cfg['funcId'])

			run_result = self.call_program(run_cmd)
			if run_result['returncode'] != 0:
				return Result(time=FAIL_PENALTY)
			else:
				val = self.get_metric_from_app_output(run_result['stderr'])
				return Result(time=val)
		else:
			print "Invalid configuration, return penalty."
			# FAIL_PENALTY = FAIL_PENALTY - 1
			return Result(time=FAIL_PENALTY)

	def get_metric_from_app_output(self, app_output):
		"""Returns the metric value from output benchmark"""
		metric_value = 0.0
		lines = app_output.split("\n")
		for current_line in lines:
			strg = "" + current_line
			if strg.find("Instructions Executed") > -1:
				idx = strg.index("Instructions Executed")
				subsrtg = strg[idx:].split("   ")
				print "substrg: ", subsrtg
				metric_value = float(subsrtg[3])
				print "inst_executed: ", metric_value
		return (1.0 - metric_value)

	def save_final_config(self, configuration):
		"""called at the end of tuning"""
		print "Optimal block size written to sumvectorcuda_final_config.json:", configuration.data
		self.manipulator().save_to_file(configuration.data, 'sumvectorcuda_final_config.json')
		


if __name__ == '__main__':
	FAIL_PENALTY = 9999999999
	compiled = False
	n = 65568
	argparser = opentuner.default_argparser()
	SumVectorTuner.main(argparser.parse_args())
