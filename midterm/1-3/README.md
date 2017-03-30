# How to Run

1. build	: $ sudo docker build -f Dockerfile -t ins .sudo docker run --rm -v $PWD:/app -w /app dock gcc insertion.c
2. complie 	: $ sudo docker run --rm -v $PWD:/app -w /app dock gcc insertion.c
3. run 		: $ sudo docker run --rm -v $PWD:/app -w /app dock ./a.out