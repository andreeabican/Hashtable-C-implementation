build: tema1.exe 
	
hash.lib: 
	lib /out:hash.lib hash.obj
	
tema1.exe: tema1.obj hash.lib
	cl hash.lib tema1.obj
	
tema1.obj:
	cl /W3 /c tema1.c
	
clean:	
	del tema1.exe tema1.obj