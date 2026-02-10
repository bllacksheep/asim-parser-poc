d:
	mkdir -p bin/ build/

b:
	go build -o bin/asim-parser-poc main.go

g:
	go run main.go stubs/example.xml

dbg:
	go build -work -gcflags="all=-N -l" -ldflags="-w=false" -o bin/cgo_sandbox main.go 

c:
	xxd -i stubs/test.xsd > api/xsd.h && \
	gcc xsd.c -g -O0 `pkg-config --libs --cflags libxml-2.0` -o bin/validator

v:
	bin/validator

clean:
	rm -rf bin/* build/*

