����lua��̬̬��ʱ��Ҫ�޸�һ��Makefile�ļ�
��CFLAGS�����-fPIC -g -rdynamic 
����ִ��
g++ -g -fPIC -O2 -shared -o liblua.so lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lvm.o lzio.o lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o loslib.o ltablib.o lstrlib.o loadlib.o linit.o