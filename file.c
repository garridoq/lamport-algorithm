#include "file.h"


void inserer(file** filei, int estampille, int sitePos){	

	file* node = (file*)malloc(sizeof(file*));
	node->estampille = estampille;
	node->sitePos = sitePos;

	//Si file est vide on crée la file
	if(*filei == NULL){
		*filei = node;
		return;
	}

	//Insérer au début ?
	if(estampille < (*filei)->estampille){
		node->next = *filei;
		*filei = node;
	}
	else if(estampille == (*filei)->estampille){
		if(sitePos < (*filei)->sitePos){
			node->next = *filei;
			*filei = node;
			return;
		}
	}
	
	file* previous = *filei;
	file* elt = (*filei)->next;
	while(elt != NULL){
		if(estampille < elt->estampille){
			previous->next = node;
			node->next = elt;
			return;
		}
		else if(estampille == elt->estampille){
			if(sitePos < elt->sitePos){
				previous->next = node;
				node->next = elt;
				return;
			}
		}

		previous = elt;
		elt = elt->next;	
	}
	
	previous->next = node;

	return;
}
void retirer(file** filei, int estampille, int sitePos){
	

	//Enlever le début ?
	if(estampille == (*filei)->estampille && sitePos == (*filei)->sitePos){
		*filei = (*filei)->next;
		return;
	}
	
	file* previous = *filei;
	file* elt = (*filei)->next;
	while(elt != NULL){
		if(estampille < elt->estampille){
			printf("Requete non présente dans la file\n");
			return;
		}
		else if(estampille == elt->estampille){
			if(sitePos < elt->sitePos){
				printf("Requete non présente dans la file\n");
				return;
			}
			if(sitePos == elt->sitePos){
				previous->next = elt->next;
				free(elt);
				return;
			}
		}

		previous = elt;
		elt = elt->next;	
	}
	
	printf("Requete non présente dans la file\n");
	return;
}

int verifierSiPremier(file* file, int estampille,int sitePos){
	if(file->estampille == estampille && file->sitePos == sitePos){
		return 1;
	}
	return 0;
}


void printFile(file* filei){
	file* elt = filei;
	while(elt != NULL){
		if(elt->next == NULL)
			printf("(Time: %d,Site: %d)",elt->estampille,elt->sitePos);
		else
			printf("(Time: %d,Site: %d) -> ",elt->estampille,elt->sitePos);
		elt = elt->next;
	}
	printf("\n");
}



