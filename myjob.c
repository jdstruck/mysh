#include "myjob.h"
#include "myheader.h"

struct job *create_job() {
    struct job *j = malloc(sizeof(struct job));
    j->next = NULL;
    j->first_process = NULL;
    j->tail = NULL;
    return j;
}

struct process *create_process(char **argv) {
    struct process *p = malloc(sizeof(struct process));
    p->next = NULL;
    p->argv = argv;
    return p;
}
void add_process_to_job(job *j, process *p) {
    if(j==NULL) {
        j = create_job();
        j->first_process = j->tail = p;
    } else if (j->first_process == j->tail) {
        j->tail = j->first_process->next = p;
    } else {
        j->tail->next = p;
        j->tail = p;
    }

    // print_char_ptr_arr(p->argv);
    // print_job_queue(j);
}

void print_job_queue(job *j) {
    for (job *j_curr = j; j_curr != NULL; j_curr = j_curr->next) {
        for (process *p_curr = j_curr->first_process; p_curr != NULL; p_curr = p_curr->next) {
            print_char_ptr_arr(p_curr->argv);
        }
        printf("\n");
    }
}
