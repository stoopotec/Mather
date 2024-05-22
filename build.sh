
gcc -Wall -Wextra -pedantic -g ./src/main.c ./src/server.c ./src/send_files.c ./src/internet.c ./src/http.c ./src/transformer.c -o server
gcc -Wall -Wextra -pedantic -g ./src/transformer_test.c ./src/transformer.c -o transformer_test
