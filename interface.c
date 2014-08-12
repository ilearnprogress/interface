#include "vlan_interface.h"

struct vid_contain_pids_t *vlan_pids_arr[VLAN_NUM];

void init_var(struct vlan_t *vlan_head,struct port_t *port_head) {
    INIT_LIST_HEAD(&vlan_head.list_vlan);
    INIT_LIST_HEAD(&port_head.list_port);
}

int create_vlan(int vid, char *name) {
    if (vid <= 0 || vid >= 4095 || name = NULL)
        return -1;
        
    struct vlan_t *vlantmp;
    struct list_head *postmp;
    int len_name;
    
    list_for_each(pos,&vlan_head.list_vlan) {
        vlantmp = list_entry(pos, vlan_t, list_vlan);
        if(vlantmp->vid == vid)
            return;
    }
    vlantmp = (struct vlan_t *) malloc (sizeof (struct vlan_t));
    len_name = strlen(name) + 1;
    vlantmp.name = (char *) malloc (len_name);
    memcpy(vlantmp.name, name, len_name);
    vlantmp.vid = vid;  
    /* XXX:vlan_head is not clear , head of a list that a vlan contain many pids */
    list_add(&(vlantmp->list_vlan), &(vlan_head.list_vlan));
    
    return 0;
}

vlan_t *vid_to_vlan(int vid) {
    if (vid <= 0 || vid >= 4095 || list_empty(&vlan_head))
        return -1;

    struct list_head *pos;
    struct vlan_t *vlantmp;
    
    list_for_each(pos, &vlan_head.list_vlan) {
        vlantmp = list_entry(pos, vlan_t, list_vlan);
        if (vlantmp->vid == vid) 
            return vlantmp;
    }
    
    return NULL;
}

void destory_vlan(int vid) {
    if (vid <= 0 || vid >= 4095 || list_empty(&vlan_head))
        return;
        
    struct list_head *pos, *postmp;
    struct vlan_t *vlandeltmp;
    
    list_for_each_safe(pos, postmp, &vlan_head.list_vlan) {
        vlandeltmp = list_entry(pos, vlan_t, list_vlan);
        if (vlandeltmp->vid == vid) {
            list_del(pos);
            free(vlandeltmp->name);
            free(vlandeltmp);
            vlandeltmp->name = NULL;
            vlandeltmp = NULL;
        }       
    }
    
    return;
}

void add_port(int pid, int speed, int duplex) {
    if (pid < 0 || 512 < pid < 2048 || pid > 2176
    || speed < 0 || duplex < 0)
        return;
    
    struct list_head *postmp;
    struct port_t *porttmp;
    
    /* XXX:port_head is the head of post list */
    list_for_each(postmp,&port_head.list_port) {
        porttmp = list_entry(postmp, port_t, list_port);
        if (porttmp->pid == pid)
            return;
    }
    
    porttmp = (struct port_t *) malloc (sizeof (struct port_t));
    porttmp->pid = pid;
    porttmp->speed = speed;
    porttmp->duplex = duplex;
    
    list_add(&(porttmp->list_port),&(port_head.list_port));
    
    return;
}

void delete_port(int pid) {
    if (pid < 0 || 512 < pid < 2048 || pid > 2176 || list_empty(&port_head))
        return;
        
    struct list_head *pos, *porttmp;
    struct port_t *portdeltmp;
    
    list_for_each_safe(pos, porttmp, &port_head.list_port) {
        portdeltmp = list_entry(pos, port_t, list_port);
        if (portdeltmp->pid == pid) {
            list_del(pos);
            free(portdeltmp);
            portdeltmp = NULL;
        }
    }
    
    return;
}

port_t *pid_to_port(int pid) {
    if (pid < 0 || 512 < pid < 2048 || pid > 2176)
        return;
        
    struct list_head *postmp;
    struct port_t *port_look;
    
    list_for_each(pos, &(port_head->list_port)) {
        port_look = list_entry(pos, port_t, list_port);
        if (port_look->pid == pid)
            return port_look;
    }
    
    return NULL;
}

int add_port_to_vlan(int vid, int pid) {
    if (vid <= 0 || vid >= 4095
        || pid < 0 || 512 < pid < 2048 || pid > 2176)
        return -1;
    
    struct vid_contain_pids_t *pidinvid;
    
    pidinvid = (struct vid_contain_pids_t *) malloc (sizeof (struct vid_contain_pids_t));
    pidinvid.pid = pid;
    pidinvid.vid = vid;
    
    /* XXX: tmpname is not sure, need to define */
    if (vlan_pids_arr[vid%VLAN_NUM] == NULL) {
        char tmpname[10];
        
        sprintf(tmpname, "%d", vid);
        create_vlan(vid, tmpname);
        vlan_pids_arr[vid%VLAN_NUM]  = (struct vid_contain_pids_t *) malloc (sizeof (struct vid_contain_pids_t));
        INIT_LIST_HEAD(&(vlan_pids_arr[vid%VLAN_NUM]->list_pidinvid));
    }
        
    /* XXX:vlan_pids_head is not clear , head of a list that a vlan contain many pids ???????????????*/
    list_add(&(pidinvid->list_pidinvid), &(vlan_pids_arr[vid%VLAN_NUM]->list_pidinvid));
    
    return 0;
}

void del_port_from_vlan(int vid, int pid) {
    if (vid <= 0 || vid >= 4095 
        || pid < 0 || 512 < pid < 2048 || pid > 2176
        || vlan_pids_arr[vid%VLAN_NUM] == NULL)
        return -1;
    
    struct list_head *pos, *postmp;
    struct vid_contain_pids_t *pidinvid_tmp;
    
    list_for_each_safe(pos, postmp, &(vlan_pids_arr[vid%VLAN_NUM]->list_pidinvid)) {
        pidinvid_tmp = list_entry(pos, vid_contain_pids_t, list_pidinvid);  // have a problem
        if (pidinvid_tmp->pid == pid && pidinvid_tmp->vid == vid){
            list_del(pos);
            free(pidinvid_tmp);
            pidinvid_tmp = NULL;
        }
    }
    
    return;
}

int is_contain(int vid, int pid) {
    if (vid <= 0 || vid >= 4095 
        || pid < 0 || 512 < pid < 2048 || pid > 2176
        || vlan_pids_arr[vid%VLAN_NUM] == NULL)
        return -1;
     
    list_head *pos, *postmp;
    vid_contain_pids_t* pidinvid_tmp;
    
    list_for_each(pos, &(vlan_pids_arr[vid%VLAN_NUM]->list_pidinvid)) {
        pidinvid_tmp = list_entry(pos, vid_contain_pids_t, list_pidinvid);
        if (pidinvid_tmp->pid == pid && pidinvid_tmp->vid == vid)
            return 1
    }
    
    return -1;
}

void show_port(int pid, int vid) {
    struct port_t *port_status;
    
    port_status = pid_to_port(pid);
    if (port_status == NULL)
        return NULL;
       
    printf("vid = %d, pid = %d, speed = %d, duplex = %d\n", 
            vid, port_status->pid, port_status->speed, port_status->duplex);
    
    return;
}

void show_vlan(int pid, int vid) {
    struct vlan_t *vlan_status;
    
    vlan_status = vid_to_vlan(vid);
    if (vlan_status == NULL)
        return;
      
    printf("pid = %d, vid = %d, vid_name = %s\n",
            pid, vlan_status->vid, vlan_status->name);
            
    return;
}

void for_each_port(int vid, void (*operation)(int pid, int vid)) {
    if (vid <= 0 || vid >= 4095 || vlan_pids_arr[vid%VLAN_NUM] == NULL)
        return NULL;
        
    struct list_head *pos;
    struct vid_contain_pids_t *pidinvid_look;
    
    operation = show_port;
    list_for_each(pos, &(vlan_pids_arr[vid%VLAN_NUM]->list_pidinvid)) {
        pidinvid_look = list_entry(pos, vid_contain_pids_t, list_pidinvid);
        if (pidinvid_look->vid == vid)
            operation(pidinvid_look->pid,vid);
    }
    
    return;
}

void for_each_vlan(int pid, void (*operation)(int pid, int vid)) {
    if (pid < 0 || 512 < pid < 2048 || pid > 2176)
        return NULL;
        
    struct list_head *pos;
    struct vid_contain_pids_t *pidinvid_look;
    int i;
    
    operation = show_vlan;
    for (i = 0; i < VLAN_NUM; i++) {
        if (vlan_pids_arr[i] == NULL)
            continue;
            
        list_for_each(pos, &(vlan_pids_arr[i]->list_pidinvid)) {
            pidinvid_look = list_entry(pos, vid_contain_pids_t, list_pidinvid);
            if (pidinvid_look->pid == pid){
                operation(pid, i);
                break;
            }                
        }
    }
 
    return;
}
