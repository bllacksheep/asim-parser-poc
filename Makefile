build:
	go build main.go -o bin/

run:
	go run main.go example.xml 

debug:
	go build -work -gcflags="all=-N -l" -ldflags="-w=false" -o bin/cgo_sandbox main.go 

c:
	gcc src/parser.c `pkg-config --libs --cflags libxml-2.0` -o bin/parser
