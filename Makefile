build:
	go build main.go

run:
	go run main.go example.xml

debug:
	go build -work -gcflags="all=-N -l" -ldflags="-w=false" -o sandbox main.go
