dev:
	mkdir -p bin/ build/

build:
	go build -o bin/asim-parser-poc main.go

rungo:
	go run main.go stubs/example.xml

runc:
	bin/validator stubs/test.xml stubs/test.xsd

debug:
	go build -work -gcflags="all=-N -l" -ldflags="-w=false" -o bin/cgo_sandbox main.go 

c:
	gcc xsd.c -g -O0 `pkg-config --libs --cflags libxml-2.0` -o bin/validator

clean:
	rm -rf bin/* build/*
