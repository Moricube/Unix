server:
1. Компиляция сервера: gcc -o server server.c
2. Запуск сервера с параметрами <port>: ./server 7000

client:
1. Компиляция клиента: gcc -o client client.c
2. Запуск клиента с параметрами <message> <host> <port>: ./client signal1 127.0.0.1 7000