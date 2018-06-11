//
// Created by 李新锐 on 11/06/2018.
//

#ifndef SRC_SYS_SEM_H
#define SRC_SYS_SEM_H

int sys_do_p(int sem_id);
int sys_do_v(int sem_id);
int sys_do_getsem(int v);
int sys_do_freesem(int sem_id);


#endif //SRC_SYS_SEM_H
