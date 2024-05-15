#include <stdio.h>
#include <stdlib.h>
typedef struct processus{
    int *allocation_precessus;
    int *demande_processus;
    struct processus *next_processus;
}Processus;

typedef struct ressource{
    int num_ressources;
    int exemplair_ressource;
    struct ressource *next_ressources;
}Ressource;

typedef struct memory_ressource{
    Ressource *head_ressource;
}Memory_Ressource;

typedef struct memory_precessus
{
    Processus *head_processe;
}Memory_Precessus;

void AddRessources(Memory_Ressource*);
void AddProcessus(Memory_Precessus*, Memory_Ressource*);
void DisplayRessources(Memory_Ressource*);
int *AllocateProcessus(int);
void DisplayAll(Memory_Precessus*,Memory_Ressource *);

// void AddRessources(Memory_Ressource* Pile_Ressources_tmp){
//     Ressource *MEMR = (Ressource*)malloc(sizeof(Ressource));
//     int n, tmp_exp;
//     printf("\nRessources number? ");
//     scanf("%d", &n);
//     printf("\nexemplaire number? \n");
//     scanf("%d", &tmp_exp);
//     MEMR->num_ressources=n;
//     MEMR->exemplair_ressource = tmp_exp;
//     MEMR -> next_ressources = Pile_Ressources_tmp->head_ressource;
//     Pile_Ressources_tmp->head_ressource = MEMR;
// }
void AddRessources(Memory_Ressource* Pile_Ressources_tmp) {
    Ressource *MEMR = (Ressource*)malloc(sizeof(Ressource));
    int n, tmp_exp;
    printf("\nRessource number? ");
    scanf("%d", &n);
    printf("\nexemplaire number? \n");
    scanf("%d", &tmp_exp);
    MEMR->num_ressources = n;
    MEMR->exemplair_ressource = tmp_exp;
    MEMR->next_ressources = Pile_Ressources_tmp->head_ressource;
    Pile_Ressources_tmp->head_ressource = MEMR;
}
int *AllocateProcessus(int number_of_ressource){
    int *tmp_alloc = (int*)malloc(number_of_ressource*sizeof(int));
    int j;
    for(int i=0;i<number_of_ressource;i++)
    {
        scanf("%d ", &j);
        *(tmp_alloc+i) = j;
    }
    return tmp_alloc;
}
int *DemandeProcessus(int number_of_ressource){
    int *tmp_demand = (int*)malloc(number_of_ressource*sizeof(int));
    int j;
    for(int i=0;i<number_of_ressource;i++)
    {
        scanf("%d ", &j);
        *(tmp_demand+i) = j;
    }
    return tmp_demand;
}
void AddProcessus(Memory_Precessus* Pile_Processus_tmp, Memory_Ressource *Pile_Ressources_tmp){
    Processus *MEMP = (Processus*)malloc(sizeof(Processus));
    MEMP->allocation_precessus = AllocateProcessus(Pile_Ressources_tmp->head_ressource->num_ressources);
    MEMP->demande_processus = DemandeProcessus(Pile_Ressources_tmp->head_ressource->num_ressources);
}


void DisplayRessources(Memory_Ressource* Pile_Ressources_tmp){
    Ressource * tmp = Pile_Ressources_tmp->head_ressource;
    printf("\nRessource  :   exemplaire\n");
    while(tmp!=NULL)
    {
        printf("\n\t%d\t\t%d\n",tmp->num_ressources,tmp->exemplair_ressource);
        tmp = tmp->next_ressources;
    }
}


void DisplayAll(Memory_Precessus *Pile_Precessus_tmp,Memory_Ressource *Pile_Ressources_tmp)
{
    Processus *tmp = Pile_Precessus_tmp->head_processe;
    int i=1;
    while(tmp!=NULL)
    {
        printf("\nprocessus-%d: \n", i);
        printf("\n   Allocation   \n");
        for(int i=0;i<Pile_Ressources_tmp->head_ressource->num_ressources;i++)
        {
            printf("%d \t", tmp->allocation_precessus+i);
        }
        printf("\n   demande   \n");
        for(int i=0;i<Pile_Ressources_tmp->head_ressource->num_ressources;i++)
        {
            printf("%d \t", tmp->demande_processus);
        }
        tmp = tmp->next_processus;
        i++;
    }
}


int main()
{
    int number_of_precessus;
    int number_of_ressource;
    Memory_Precessus *Pile_Processus = NULL;
    Memory_Ressource *Pile_Ressource = NULL;
    Pile_Processus = (Memory_Precessus*)malloc(sizeof(Memory_Precessus));
    Pile_Ressource = (Memory_Ressource*)malloc(sizeof(Memory_Ressource));
    printf("entrer le nombre des resources: ");
    scanf("%d",&number_of_ressource);
    for(int i=1;i<=number_of_ressource;i++)
    {
        AddRessources(Pile_Ressource);
    }
    DisplayRessources(Pile_Ressource);
    printf("\nentrez le nombre de processus");
    scanf("%d",&number_of_precessus);
    for(int j=1;j<=number_of_precessus;j++)
    {
        AddProcessus(Pile_Processus,Pile_Ressource);
    }

    DisplayAll(Pile_Processus,Pile_Ressource);
    return 0;
}