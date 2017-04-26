# -*- coding: utf-8 -*-
"""
	@summary:
		内存插件的单元测试启动脚本
	@version:
		1.0.1
	@date:
		2017/4/26


	@usage:
		python3 TestMemoPlugin.py -f MemDatabase.dll

	@enviroment:
		需要使用 python3.x 以上的版本, 无需安装其他组件
"""


import ctypes, getopt, time, sys, os


if __name__ == '__main__':
	try:
		### 从参数取解析文件路径+参数
		opts, args = getopt.getopt( sys.argv[1:], "f:", ["file=",] )
		lstFilePath = [value for op, value in opts if op in ( "-f", "--file" )]
		sFilePath = "MemDatabase.dll" if (len(lstFilePath) == 0) else lstFilePath[0]
		print( r"--------- dll path : {path} -----------".format( path=sFilePath ) )
		objDll = ctypes.cdll.LoadLibrary( sFilePath )
		if None == objDll:
			raise Exception( "Failed 2 open data file : {path}".format( path=sFilePath )  )

		### 开始单元测试
		objDll.ExecuteUnitTest()

	except Exception as e:
		print( e )
	finally:
		print( r"--------------------- [DONE] ---------------------------" )






