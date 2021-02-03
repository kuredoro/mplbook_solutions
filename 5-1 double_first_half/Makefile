run: build
	./task.out

build: with_views.cpp no_views.cpp
	clang++ with_views.cpp -Wall -o with_views.out
	clang++ no_views.cpp -Wall -o no_views.out -ftemplate-depth=20
