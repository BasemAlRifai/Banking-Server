CC = gcc
CFLAGS = -fno-stack-protector -Wall -Wextra -Iutil -Iatm -Ibank -Irouter -I.
LDFLAGS=-lcrypto -lssl


all: atm bank 

atm: atm-main.o atm.o net.o
bank: bank-main.o bank.o net.o

clean:
	rm -f atm bank *.o
