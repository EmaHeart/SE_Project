#include <stdio.h>
#include <stdlib.h>

/***********************structure needed initially*******************************/
typedef struct processus {
    int *allocation_processus;
    int *demande_processus;
    struct processus *next_processus;
} Processus;

typedef struct ressource {
    int num_ressources;
    int exemplair_ressource;
    struct ressource *next_ressources;
} Ressource;

typedef struct memory_ressource {
    Ressource *head_ressource;
} Memory_Ressource;

typedef struct memory_precessus {
    Processus *head_processe;
} Memory_Precessus;

/************************function declaration**************************/
void AddRessources(Memory_Ressource*);
void DisplayRessources(Memory_Ressource*);
int *allocationProcess(int);
void ADDProcessus(Memory_Precessus*, int);
int *DemandProcess(int);
void DisplayProcessus(Memory_Precessus*, int);
int *Available(Memory_Precessus*, int*, int);
int checklogicInput(int*, int);
void InterBlockage(Memory_Precessus*, int*, int, int);
void displayInputTable(Memory_Precessus*, int);
void displaySafeSequence(int*, int*, int, int);

/*************************function definition*************************/
void AddRessources(Memory_Ressource* File_Ressources_tmp) {
    Ressource *MEMR = (Ressource*)malloc(sizeof(Ressource));
    Ressource *tmp;
    int n, tmp_exp;
    printf("\nRessources number? ");
    scanf("%d", &n);
    printf("\nExemplaire number? \n");
    scanf("%d", &tmp_exp);
    MEMR->num_ressources = n;
    MEMR->exemplair_ressource = tmp_exp;
    MEMR->next_ressources = NULL;
    
    if(File_Ressources_tmp->head_ressource == NULL) {
        File_Ressources_tmp->head_ressource = MEMR;
    } else {
        tmp = File_Ressources_tmp->head_ressource;
        while(tmp->next_ressources != NULL) {
            tmp = tmp->next_ressources;
        }
        tmp->next_ressources = MEMR;
    }
}

void DisplayRessources(Memory_Ressource* File_Ressources_tmp) {
    Ressource *tmp = File_Ressources_tmp->head_ressource;
    if(tmp == NULL) {
        printf("\nNO ressource added!!\n");
    } else {
        printf("\nRessource  :   Exemplaire\n");
        while(tmp != NULL) {
            printf("\n\t%d\t\t%d\n", tmp->num_ressources, tmp->exemplair_ressource);
            tmp = tmp->next_ressources;
        }
    }
}

int *allocationProcess(int num_of_ressources) {
    int *tmp = (int*)malloc(sizeof(int) * num_of_ressources);
    for(int i = 0; i < num_of_ressources; i++) {
        printf("Allocation for R%d: ", i + 1);
        scanf("%d", (tmp + i));
    }
    return tmp;
}

int *DemandProcess(int num_of_ressources) {
    int *tmp = (int*)malloc(sizeof(int) * num_of_ressources);
    for(int i = 0; i < num_of_ressources; i++) {
        printf("Demand for R%d: ", i + 1);
        scanf("%d", (tmp + i));
    }
    return tmp;
}

void DisplayProcessus(Memory_Precessus* File_Proc_tmp, int num_ressources) {
    Processus *tmp = File_Proc_tmp->head_processe;
    if(tmp == NULL) {
        printf("\nNO Processus added!!\n");
    } else {
        int i = 1;
        while(tmp != NULL) {
            printf("\nPour P%d:\n", i);
            printf("\tAllocation: ");
            for(int j = 0; j < num_ressources; j++) {
                printf("%d ", tmp->allocation_processus[j]);
            }
            printf("\n\tDemande: ");
            for(int j = 0; j < num_ressources; j++) {
                printf("%d ", tmp->demande_processus[j]);
            }
            printf("\n");
            tmp = tmp->next_processus;
            i++;
        }
    }
}

void ADDProcessus(Memory_Precessus* File_Process_tmp, int num_of_ressource) {
    Processus *MEMP = (Processus*)malloc(sizeof(Processus));
    Processus *tmp;
    MEMP->allocation_processus = allocationProcess(num_of_ressource);
    MEMP->demande_processus = DemandProcess(num_of_ressource);
    MEMP->next_processus = NULL;
    
    if(File_Process_tmp->head_processe == NULL) {
        File_Process_tmp->head_processe = MEMP;
    } else {
        tmp = File_Process_tmp->head_processe;
        while(tmp->next_processus != NULL) {
            tmp = tmp->next_processus;
        }
        tmp->next_processus = MEMP;
    }
}

int *Available(Memory_Precessus* File_Proc_tmp, int* work_tmp, int num_ressources) {
    Processus *tmp = File_Proc_tmp->head_processe;
    int i = 0;
    while(tmp != NULL) {
        for(int j = 0; j < num_ressources; j++) {
            *(work_tmp + j) -= tmp->allocation_processus[j];
        }
        tmp = tmp->next_processus;
        i++;
    }
    return work_tmp;
}

int checklogicInput(int *Work_tmp, int num_ressources) {
    for(int j = 0; j < num_ressources; j++) {
        if(Work_tmp[j] < 0) {
            return 0;
        }
    }
    return 1;
}

void InterBlockage(Memory_Precessus* File_Process_tmp, int* work, int num_processus, int num_ressources) {
    int *finish = (int*)calloc(num_processus, sizeof(int));  // Initialize to 0
    int *SS = (int*)malloc(num_processus * sizeof(int));
    Processus *tmp = File_Process_tmp->head_processe;
    
    for(int i = 0; i < num_processus; i++) {
        finish[i] = 0;
    }

    int count = 0;
    while(count < num_processus) {
        int found = 0;
        tmp = File_Process_tmp->head_processe;
        for(int p = 0; p < num_processus; p++) {
            if(finish[p] == 0) {
                int j;
                for(j = 0; j < num_ressources; j++) {
                    if(tmp->demande_processus[j] > work[j]) {
                        break;
                    }
                }

                if(j == num_ressources) {
                    for(int k = 0; k < num_ressources; k++) {
                        work[k] += tmp->allocation_processus[k];
                    }
                    SS[count++] = p;
                    finish[p] = 1;
                    found = 1;
                }
            }
            tmp = tmp->next_processus;
        }

        if(found == 0) {
            printf("\nSystem is in deadlock.\n");
            printf("Deadlocked processes: ");
            for(int i = 0; i < num_processus; i++) {
                if(finish[i] == 0) {
                    printf("P%d ", i + 1);
                }
            }
            printf("\n");
            free(finish);
            free(SS);
            return;
        }
    }

    printf("System is not in deadlock.\n");
    displaySafeSequence(SS, work, count, num_ressources);
    free(finish);
    free(SS);
}

void displayInputTable(Memory_Precessus* File_Process_tmp, int num_ressources) {
    Processus *tmp = File_Process_tmp->head_processe;
    if (tmp == NULL) {
        printf("\nNo processes added!!\n");
    } else {
        printf("\nProcess\tAllocation\t\tDemand\n");
        printf("       \t");
        for (int i = 0; i < num_ressources; i++) {
            printf("R%d ", i + 1);
        }
        printf("\t\t");
        for (int i = 0; i < num_ressources; i++) {
            printf("R%d ", i + 1);
        }
        printf("\n");
        int process_id = 1;
        while (tmp != NULL) {
            printf("P%d\t", process_id);
            for (int j = 0; j < num_ressources; j++) {
                printf("%d  ", tmp->allocation_processus[j]);
            }
            printf("\t\t");
            for (int j = 0; j < num_ressources; j++) {
                printf("%d  ", tmp->demande_processus[j]);
            }
            printf("\n");
            tmp = tmp->next_processus;
            process_id++;
        }
    }
}

void displaySafeSequence(int* safeSequence, int* work, int count, int num_ressources) {
    printf("\nSafe Sequence:\n");
    for (int i = 0; i < count; i++) {
        printf("P%d ", safeSequence[i] + 1);
    }
    printf("\n\nWork values during the sequence:\n");
    for (int i = 0; i < count; i++) {
        printf("P%d -> ", safeSequence[i] + 1);
        for (int j = 0; j < num_ressources; j++) {
            printf("%d ", work[j]);
        }
        printf("\n");
    }
}

/**************************main function**************************************/
int main() {
    int number_of_processus;
    int number_of_ressource;
    int *Rmax;
    Memory_Precessus *File_Processus = (Memory_Precessus*)malloc(sizeof(Memory_Precessus));
    Memory_Ressource *File_Ressource = (Memory_Ressource*)malloc(sizeof(Memory_Ressource));
    File_Processus->head_processe = NULL;
    File_Ressource->head_ressource = NULL;

    /***********************RESSOURCE*************************/
    printf("Entrer le nombre des ressources: ");
    scanf("%d", &number_of_ressource);
    for(int i = 1; i <= number_of_ressource; i++) {
        AddRessources(File_Ressource);
    }
    DisplayRessources(File_Ressource);

    /***********************PROCESSUS*************************/
    printf("\nEntrer le nombre des processus: ");
    scanf("%d", &number_of_processus);
    for(int i = 1; i <= number_of_processus; i++) {
        printf("\nEntrer les donnes de processus-%d: \n", i);
        ADDProcessus(File_Processus, number_of_ressource);
    }
    DisplayProcessus(File_Processus, number_of_ressource);
    displayInputTable(File_Processus, number_of_ressource);
    printf("if all good press any key to check deadlock");
    system("pause");
    /************************Rmax******************************/
    Rmax = (int*)malloc(sizeof(int) * number_of_ressource);
    for(int i = 0; i < number_of_ressource; i++) {
        Rmax[i] = 0;
    }
    for(int i = 0; i < number_of_ressource; i++) {
        Ressource *tmp = File_Ressource->head_ressource;
        while(tmp != NULL) {
            if(tmp->num_ressources - 1 == i) {
                *(Rmax + i) = tmp->exemplair_ressource;
            }
            tmp = tmp->next_ressources;
        }
    }
    /************************work************************/
    int *work = (int*)malloc(sizeof(int) * number_of_ressource);
    for(int i = 0; i < number_of_ressource; i++) {
        work[i] = Rmax[i];
    }
    printf("\nRmax:  ");
    for(int i = 0; i < number_of_ressource; i++) {
        printf("%d ", work[i]);
    }
    work = Available(File_Processus, work, number_of_ressource);
    printf("\n Disponibilite: ");
    for(int i = 0; i < number_of_ressource; i++) {
        printf("%d ", work[i]);
    }
    /********************check INTERBLOCKAGE***************************/
    if(!checklogicInput(work, number_of_ressource)) {
        printf("\nIlogic Input\n");
    } else {
        InterBlockage(File_Processus, work, number_of_processus, number_of_ressource);
    }
    free(Rmax);
    free(work);
    free(File_Processus);
    free(File_Ressource);
    return 0;
}
