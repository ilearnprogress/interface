#include "vlan_interface.h"

int main(int argc, char **argv) {
    struct port_t port_head;
    struct vlan_t vlan_head;
    
    INIT_LIST_HEAD(&port_head.list_port);
    INIT_LIST_HEAD(&vlan_head.list_vlan);
    
    create_vlan(20,"firstcreatvlan");
    
    printf("it is ok\n");
    return 0;
}
