
#include "split.h"

int count_occurences(char* texte, const char sep){
	int count = 0;
	for(int i = 0; i < strlen(texte);++i){
		if(texte[i] == sep)
			count++;
	}
	return count;	
}

char** split(char* texte,int count, const char* sep){
	if(count == 0)
		return NULL;

	char* texte_cop = strdup(texte);
	char** liste = (char**)malloc((count+1)*sizeof(char*));
	char* reste = NULL;
	char* token;
	int i = 0;
	for(token = strtok_r(texte_cop,sep,&reste); token !=NULL; token=strtok_r(NULL,sep,&reste))
	{
		liste[i] = strdup(token);
		i++;
	}
	return liste;
}
