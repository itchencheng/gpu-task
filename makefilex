
run: 
	./a.out kernel/kernel.cl

build:
	g++ -I /usr/local/cuda-9.0/include \
        -L /usr/local/cuda-9.0/lib64 \
        src/main.cpp \
        src/setup_opencl.h \
        src/setup_opencl.cpp \
        src/helper.h \
        src/helper.cpp \
        -lOpenCL 

clean:
	rm -rf *.so a.out src/*.h.gch

# very funny, the lOpenCL must write at the last
