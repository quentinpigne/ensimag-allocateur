/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "mem.h"
#include <string.h>
/**
   Structures de données
**/

typedef struct ZL {
  struct ZL* suivant; 
} ZL;

ZL* TZL[BUDDY_MAX_INDEX + 1];

void* zone_memoire = NULL;

/**
   Fonctions de l'allocateur mémoire
**/

int 
mem_init()
{
  //  printf("init : %p \n", zone_memoire);
  if (! zone_memoire)
    zone_memoire = (void *) malloc(ALLOC_MEM_SIZE);
  if (zone_memoire == 0)
    {
      perror("mem_init:");
      return -1;
    }
  // Etat initial, il n'y a qu'un bloc de taille maximale
  // Création de la zone mémoire principale
  ZL* zone_init = zone_memoire;
  zone_init->suivant = NULL;


  for(int i = 0; i < BUDDY_MAX_INDEX; i++){
    TZL[i] = NULL;
  }
  memset(zone_memoire,0,ALLOC_MEM_SIZE);
  TZL[BUDDY_MAX_INDEX] = zone_init;
  
  
  return 0;
}

void *
mem_alloc(unsigned long size)
{
  // printf("alloc : %p\n", zone_memoire);
  
  if(!zone_memoire || size <= 0 || size > ALLOC_MEM_SIZE) {
    return (void*)0;
  }
  // Faut pouvoir stocker au moins un pointeur vers la prochaine zone libre lorsqu'on libéra cette zone
  if(size < sizeof(unsigned long)){
    size = sizeof(void*);
  }

  // On veut allouer un bloc de taille 2^k
  int k = ceil(log(size)/log(2));
  // Trouver un bloc de taille convenable
  int j = k;
  /* if(j > BUDDY_MAX_INDEX){ */
  /*   printf("Pas de mémoire disponible"); */
  /*   return 0; */
  
  /*     }else{ */
    while (TZL[j] == NULL && j <= BUDDY_MAX_INDEX) {
      j++;
    }
      if(j > BUDDY_MAX_INDEX){
	printf("Pas de mémoire disponible\n");
	return (void*)0; 
      }
    
// J'enlève le bloc de la liste
    void *adress = TZL[j];
    TZL[j] = TZL[j]->suivant;
  
  // Si on a pas besoin diviser le bloc trouvé alors c'est cool !
 split: if(j == k){
    return adress;
  }
  // Split
  j--;
  void *adress_buddy =(void*) ((unsigned long)adress ^ (unsigned long)pow(2,j));
  TZL[j] = adress_buddy;
  goto split;
  
  
}
  
int 
mem_free(void *ptr, unsigned long size)
{
  /* ecrire votre code ici */
  if(size < sizeof(unsigned long)){
    size = sizeof(void*);
  }
  int k = ceil(log(size)/log(2));
  // On teste si le compagnon est disponible
  void *adress_buddy =(void*) ((unsigned long)ptr ^ (unsigned long)pow(2,k));
  int buddy_is_available = 0;
  ZL *courant = TZL[k];
  while (courant != NULL) {
    if(courant == adress_buddy){
      buddy_is_available = 1;
      break;
    }
    courant = courant->suivant;
  }
 
  if(!buddy_is_available){
    //On met le block d'adresse ptr dans la liste d'ordre k dans le cas où le body n'est pas trouvé
    ZL *zl = TZL[k];
    TZL[k] = ptr;
    TZL[k]->suivant = zl;
  }else{
    // Enlever le buddy de la liste indexé par TZL[k] puis ajouter ptr dans TZL[k+1]
    mem_free(ptr,size+1);
  }
 


  return 0;
}


int
mem_destroy()
{
  /* ecrire votre code ici */

  free(zone_memoire);
  zone_memoire = 0;
  return 0;
}

