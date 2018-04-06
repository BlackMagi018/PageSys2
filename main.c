#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ftsize 8
#define pnum 5

struct frame_table {
    struct frame * ft[ftsize];
};

struct freelist{
    struct frame * top;
    struct frame * tail;
};

struct frame {
    int frame_num;
    int procID;
    char Segment[25];
    int pg_num;
    struct frame * next;
};

struct page_table {
    struct process *pt[pnum];
};

struct process {
    int procID;
    int tsize;
    int tpages;
    struct frame * trefs[ftsize];
    int dsize;
    int dpages;
    struct frame *drefs[ftsize];
};

char * output_results(struct frame_table frame,struct page_table page) {
    struct process * proc;
    struct frame * fram;
    printf("Page tables(s)\n");
    for(int i = 0; i < pnum;i++){
        proc = page.pt[i];
        if(proc->procID >= 0){
            printf("Process %d:\n",proc->procID);
            printf("       Page      Frame\n");
            for(int i = 0; i < proc->tpages;i++){
                printf("Text    %d         %d\n",i,proc->trefs[i]->frame_num);
            }
            for(int i = 0; i < proc->dpages;i++){
                printf("Data    %d         %d\n",i,proc->drefs[i]->frame_num);
            }
        }
    }
    fflush(stdout);
}

int main(int argc, char **argv) {

    FILE *tape = fopen(argv[1], "r");

    //Token pointer
    char *token;
    char parts[3][50];
    //Argument Counter
    int count = 0;

    struct frame_table frame;
    for(int i = 0; i < ftsize;i++){
        struct frame * temp = malloc(sizeof(struct frame));
        temp->frame_num = i;
        temp->procID = -1;
        temp->pg_num = -1;
        if(i > 0 ){
            struct frame * prev = frame.ft[i-1];
            prev->next = temp;
        }
        frame.ft[i] = temp;

    }
    struct page_table page;
    for(int i = 0; i < pnum; i++){
        struct process * temp = malloc(sizeof(struct process));
        temp->procID = -1;
        page.pt[i] = temp;
    }
    struct freelist free;
    free.top = frame.ft[0];
    free.tail = frame.ft[ftsize-1];

    char input[100];
    printf("Click Enter to start the Simulation");
    while (getchar() && !feof(tape)) {
        fgets(input, sizeof(input), tape);
        token = strtok(input, " ");
        while (token != NULL) {
            strncpy(parts[count],token,50);
            count++;
            token = strtok(NULL, " ");
        }
        switch(count){
            case 2:
                printf("-- Process is terminating --\n");
                int pid = (int) strtol(parts[0],NULL,10);
                printf("Reclaiming Process %d Pages\n\n",pid);
                struct process * del_entry = page.pt[pid];
                del_entry->procID = -1;
                for(int i = 0; i < del_entry->tpages;i++) {
                    struct frame *deref = del_entry->trefs[i];
                    deref->procID = -1;
                    memset(deref->Segment, 0, 25);
                    deref->pg_num = -1;
                    if(free.top == NULL && free.tail == NULL){
                        free.top = deref;
                        free.tail = deref;
                    }else if(free.top == NULL) {
                        free.top = free.tail;
                        struct frame *prev = free.tail;
                        prev->next = deref;
                        free.tail = deref;
                    } else{
                        struct frame *prev = free.tail;
                        prev->next = deref;
                        free.tail = deref;
                    }
                }
                for(int i = 0; i < del_entry->dpages;i++){
                    struct frame * deref = del_entry->drefs[i];
                    deref->procID = -1;
                    memset(deref->Segment,0,25);
                    deref->pg_num = -1;
                    if(free.top == NULL && free.tail == NULL){
                        free.top = deref;
                        free.tail = deref;
                    }else if(free.top == NULL) {
                        free.top = free.tail;
                        struct frame *prev = free.tail;
                        prev->next = deref;
                        free.tail = deref;
                    } else{
                        struct frame *prev = free.tail;
                        prev->next = deref;
                        free.tail = deref;
                    }
                }
                del_entry->tpages = 0;
                del_entry->tsize = 0;
                del_entry->dpages = 0;
                del_entry->dsize = 0;
                break;
            case 3:
                printf("-- NEW PROCESS ARRIVING --\n");
                int id = (int) strtol(parts[0],NULL,10);
                struct process * entry = page.pt[id];
                entry->procID = id;
                entry->tsize = (int) strtol(parts[1],NULL,10);
                entry->dsize = (int) strtol(parts[2],NULL,10);
                int temp = 0;
                while(temp < entry->tsize){
                    temp = temp + 512;
                    entry->tpages++;
                }
                temp = 0;
                while(temp < entry->dsize){
                    temp = temp + 512;
                    entry->dpages++;
                }
                for(int i = 0; i < entry->tpages;i++) {
                    entry->trefs[i] = free.top;
                    if(free.top == free.tail) {
                        free.top = entry->trefs[i]->next;
                        free.tail =entry->trefs[i]->next;
                    } else {
                        free.top = entry->trefs[i]->next;
                    }
                    struct frame *ref = entry->trefs[i];
                    ref->procID = entry->procID;
                    ref->pg_num = i;
                    strcpy(ref->Segment, "Text");
                }
                for(int i = 0; i < entry->dpages;i++) {
                    entry->drefs[i] = free.top;
                    if(free.top == free.tail) {
                        free.top = entry->drefs[i]->next;
                        free.tail =entry->drefs[i]->next;
                    } else {
                        free.top = entry->drefs[i]->next;
                    }
                    struct frame *ref = entry->drefs[i];
                    ref->procID = entry->procID;
                    ref->pg_num = i;
                    strcpy(ref->Segment, "Data");
                }
                printf("   Process ID:  %d\n", entry->procID);
                printf("   Text Size    %d\n", entry->tsize);
                printf("   Text Pages   %d\n", entry->tpages);
                printf("   Data Size    %d\n", entry->dsize);
                printf("   Data Pages   %d\n\n", entry->dpages);
                break;
        }
        count = 0;
        output_results(frame,page);
    }
    printf("Simulation Complete");
}
