run: build #test
	./scratch.out

build: main.cpp units.hpp
	g++ main.cpp -I. -o scratch.out

test: compiles/* fails/*
	go run test.go
