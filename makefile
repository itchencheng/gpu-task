
crun: 
	./c.out kernel/kernel.cl

cpprun:
	./cpp.out kernel/kernel.cl

cppbuild:
	g++ -I /usr/local/cuda-9.0/include \
        -L /usr/local/cuda-9.0/lib64 \
        src/cpp/main.cpp \
        src/cpp/OpenCLTask.h \
        src/cpp/OpenCLTask.cpp \
        src/cpp/Task1.h \
        src/cpp/Task1.cpp \
        -lOpenCL \
        -o cpp.out 

cbuild:
	g++ -I /usr/local/cuda-9.0/include \
        -L /usr/local/cuda-9.0/lib64 \
        src/c/main.cpp \
        src/c/setup_opencl.h \
        src/c/setup_opencl.cpp \
        src/c/helper.h \
        src/c/helper.cpp \
        -lOpenCL \
        -o c.out

clean:
	rm -rf *.so *.out src/c/*.h.gch src/cpp/*.h.gch

# very funny, the lOpenCL must write at the last
