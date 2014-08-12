#ifndef _VLAN_INTERFACE_H
#define _VLAN_INTERFACE_H

#include <stdio.h>
#include <string.h>

#define VLAN_NUM 256

typedef struct {
    int  pid;                         /* port id */
    int  speed;                       /* port speed */
    int  duplex;                      /* port duplex */
    struct list_head list_port;       /* linux's list */
} port_t;

typedef struct {
    int vid;
    char *name;
    struct list_head list_vlan;
} vlan_t;

typedef struct {
    int pid;
    int vid;
    struct list_head list_pidinvid;
}vid_contain_pids_t;

int create_vlan(int vid, char *name);
vlan_t *vid_to_vlan(int vid);
void destory_vlan(int vid);
void add_port(int pid, int speed, int duplex) ;
void delete_port(int pid);
port_t *pid_to_port(int pid);
int add_port_to_vlan(int vid, int pid);
void del_port_from_vlan(int vid, int pid);
int is_contain(int vid, int pid);
void show_port(int pid, int vid);
void show_vlan(int pid, int vid);
void for_each_port(int vid, void (*operation)(int pid, int vid));
void for_each_vlan(int pid, void (*operation)(int pid, int vid));

#endif