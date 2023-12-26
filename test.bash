#!/usr/bin/env bash

cd bin || exit

#清空文件
truncate -s 0 output.txt
# 开始

for num in {1,2,3,4,5,6,7,12}
do
  capacity=1024
  for __i in {1..10}
  do
      # Print the command before executing
      echo "Executing: ./per_test "$capacity" ./traces/P"$num".lis"
      ./per_test  "$capacity"   ./traces/P"$num".lis >> output.txt
      capacity=$((capacity*2))
  done


done

#	@./$(BIN) 1024 		./traces/P$*.lis
#	@./$(BIN) 2048 		./traces/P$*.lis
#	@./$(BIN) 4096 		./traces/P$*.lis
#	@./$(BIN) 8192 		./traces/P$*.lis
#	@./$(BIN) 16384 	./traces/P$*.lis
#	@./$(BIN) 32768 	./traces/P$*.lis
#	@./$(BIN) 65536 	./traces/P$*.lis
#	@./$(BIN) 131072 	./traces/P$*.lis
#	@./$(BIN) 262144 	./traces/P$*.lis
#	@./$(BIN) 524288 	./traces/P$*.lis