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

  TZL[BUDDY_MAX_INDEX] = zone_init;

  for(int i = 0; i < BUDDY_MAX_INDEX; i++){
	  TZL[i] = NULL;
  }
  
  return 0;
}

void *
mem_alloc(unsigned long size)
{
    int indice = ceil(log(size)/log(2));
    if(TZL[indice] == NULL) {
		unsigned long taille_sup = pow(2, indice + 1);
		ZL* zl_taille_sup = mem_alloc(taille_sup);
		TZL[indice] = zl_taille_sup;
		TZL[indice]->suivant = zl_taille_sup + taille_sup/2;
    }
		ZL* courant = TZL[indice];
		TZL[indice] = courant->suivant;
		return courant;
	return (void*)0;
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

