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

/**
   Structures de données
**/

typedef struct ZL {
  struct ZL* suivant; 
} ZL;

ZL* TZL[BUDDY_MAX_INDEX + 1];

void* zone_memoire = 0;

/**
   Fonctions de l'allocateur mémoire
**/

int 
mem_init()
{
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
  TZL[BUDDY_MAX_INDEX] = zone_init;

  
  return 0;
}

void *
mem_alloc(unsigned long size)
{
  
  if(size ==0) {
    return (void*)0;
  }
  if(size == 1){
    return (void*)0;
  }

  // On veut allouer un bloc de taille 2^k
  int k = ceil(log(size)/log(2));
  // Trouver un bloc de taille convenable
  int j = k;
  if(j > BUDDY_MAX_INDEX){
    printf("Pas de mémoire disponible");
    return 0;
  
      }else{
    while (TZL[j] == NULL ) {
      if(j > BUDDY_MAX_INDEX){
	printf("Pas de mémoire disponible");
	return 0; 
      }
      j++;
    }
  }
  
  void *adress = TZL[j];
  TZL[j] = TZL[j]->suivant;
  
  // Si on a pas besoin diviser le bloc trouvé alors c'est cool !
 split: if(j == k){
    return adress;
  }
  // Split
  j--;
  ZL *adress_buddy = (int)adress ^ (int)pow(2,j);
  TZL[j] = adress_buddy;
  goto split;
  
  
}
  
int 
mem_free(void *ptr, unsigned long size)
{
  /* ecrire votre code ici */

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

