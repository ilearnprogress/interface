OBJS = vlan_interface.o main.o
CC = gcc
CFLAGS = -Wall -O -g

vlanportrun: $(OBJS)
    $(CC) $(OBJS) -o vlanportrun
    
vlan_interface.o: vlan_interface.c vlan_interface.h
    $(CC) $(CFLAGS) -c vlan_interface.c -o vlan_interface.o
main.o: main.c
    $(CC) $(CFLAGS) -c main.c -o main.o
    
clean:
    re -rf *.o vlanportrun
