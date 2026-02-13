SRC = $(CURDIR)
OBJ := build/xsd.o build/xml.o
CC := gcc
CFLAGS := -Iapi -g -O0 $(shell pkg-config --cflags libxml-2.0)
LDLIBS := $(shell pkg-config --libs libxml-2.0)

.PHONY: all clean v g d

all: bin/validator

d:
	mkdir -p bin/ build/

bin/asim-parser-poc:
	go build -o bin/asim-parser-poc main.go

g:
	go run main.go stubs/example.xml

bin/cgo_sandbox:
	go build -work -gcflags="all=-N -l" -ldflags="-w=false" -o bin/cgo_sandbox main.go 

bin/validator: $(OBJ)
	$(CC) $(OBJ) -o bin/validator $(LDLIBS)

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

build/xsd.o: api/schema.h

api/schema.h: stubs/test.xsd
	xxd -i stubs/test.xsd > api/schema.h

v:
	bin/validator

clean:
	rm -rf bin/* build/*
