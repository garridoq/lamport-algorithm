/* T. Grandpierre - Application distribue'e pour TP IF4-DIST 2004-2005

But : 

fournir un squelette d'application capable de recevoir des messages en 
mode non bloquant provenant de sites connus. L'objectif est de fournir
une base pour implementer les horloges logique/vectorielle/scalaire, ou
bien pour implementer l'algorithme d'exclusion mutuelle distribue'

Syntaxe :
arg 1 : Numero du 1er port
arg 2 et suivant : nom de chaque machine

--------------------------------
Exemple pour 3 site :

Dans 3 shells lances sur 3 machines executer la meme application:

pc5201a>./dist 5000 pc5201a.esiee.fr pc5201b.esiee.fr pc5201c.esiee.fr
pc5201b>./dist 5000 pc5201a.esiee.fr pc5201b.esiee.fr pc5201c.esiee.fr
pc5201c>./dist 5000 pc5201a.esiee.fr pc5201b.esiee.fr pc5201c.esiee.fr

pc5201a commence par attendre que les autres applications (sur autres
sites) soient lance's

Chaque autre site (pc5201b, pc5201c) attend que le 1er site de la
liste (pc5201a) envoi un message indiquant que tous les sites sont lance's


Chaque Site passe ensuite en attente de connexion non bloquante (connect)
sur son port d'ecoute (respectivement 5000, 5001, 5002).
On fournit ensuite un exemple permettant 
1) d'accepter la connexion 
2) lire le message envoye' sur cette socket
3) il est alors possible de renvoyer un message a l'envoyeur ou autre si
necessaire 

*/

#include <time.h>
#include <stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>
#include<string.h>

#include "file.h"
#include "split.h"
int GetSitePos(int Nbsites, char *argv[]) ;
void WaitSync(int socket);
void SendSync(char *site, int Port);


/*Construction d'une requete*/
char* buildRequete(char* type, int sitePos, int estampille){
	char* requete = (char*)malloc(strlen(type)+12+12+2+1);
	char sitePos_str[12];
	char estampille_str[12];
	sprintf(sitePos_str, " %d", sitePos);
	sprintf(estampille_str, " %d", estampille);
	strcat(strcat(strcat(requete,type),sitePos_str),estampille_str);

	return requete;
}

/*Check si tout le tableau est à 1*/
int verifierNombreReponses(int* reponses, int nsites){
	for(int j = 0; j<nsites; ++j){
		if(reponses[j]!=1){
			return 0;
		}
	}
	return 1;
}

/*Ident	ification de ma position dans la liste */
int GetSitePos(int NbSites, char *argv[]) {
	char MySiteName[20]; 
	int MySitePos=-1;
	int i;
	gethostname(MySiteName, 20);
	for (i=0;i<NbSites;i++) 
		if (strcmp(MySiteName,argv[i+2])==0) {
			MySitePos=i;
			//printf("L'indice de %s est %d\n",MySiteName,MySitePos);
			return MySitePos;
		}
	if (MySitePos == -1) {
		printf("Indice du Site courant non trouve' dans la liste\n");
		exit(-1);
	}
	return (-1);
}


/*Attente bloquante d'un msg de synchro sur la socket donne'e*/
void WaitSync(int s_ecoute) {
	char texte[40];
	int l;
	int s_service;
	struct sockaddr_in sock_add_dist;
	int size_sock;
	size_sock=sizeof(struct sockaddr_in);
	printf("WaitSync : ");fflush(0);
	s_service=accept(s_ecoute,(struct sockaddr*) &sock_add_dist,&size_sock);
	l=read(s_service,texte,39);
	texte[l] ='\0';
	printf("%s\n",texte); fflush(0);
	close (s_service);
} 

/*Envoie d'un msg de synchro a la machine Site/Port*/
void SendSync(char *Site, int Port) {
	struct hostent* hp;
	int s_emis;
	char chaine[15];
	int longtxt;
	struct sockaddr_in sock_add_emis;
	int size_sock;
	int l;

	if ( (s_emis=socket(AF_INET, SOCK_STREAM,0))==-1) {
		perror("SendSync : Creation socket");
		exit(-1);
	}

	hp = gethostbyname(Site);
	if (hp == NULL) {
		perror("SendSync: Gethostbyname");
		exit(-1);
	}

	size_sock=sizeof(struct sockaddr_in);
	sock_add_emis.sin_family = AF_INET;
	sock_add_emis.sin_port = htons(Port);
	memcpy(&sock_add_emis.sin_addr.s_addr, hp->h_addr, hp->h_length);

	if (connect(s_emis, (struct sockaddr*) &sock_add_emis,size_sock)==-1) {
		perror("SendSync : Connect");
		exit(-1);
	}

	sprintf(chaine,"**SYNCHRO**");
	longtxt =strlen(chaine);
	/*Emission d'un message de synchro*/
	l=write(s_emis,chaine,longtxt);
	close (s_emis); 
}

/*Envoi d'une requete vers une machine*/
void SendReq(char* req, char* Site,int Port){
	struct hostent* hp;
	int s_emis;
	struct sockaddr_in sock_add_emis;
	int size_sock;
	int l;

	if ( (s_emis=socket(AF_INET, SOCK_STREAM,0))==-1) {
		perror("SendReq : Creation socket");
		exit(-1);
	}

	hp = gethostbyname(Site);
	if (hp == NULL) {
		perror("SendReq: Gethostbyname");
		exit(-1);
	}

	size_sock=sizeof(struct sockaddr_in);
	sock_add_emis.sin_family = AF_INET;
	sock_add_emis.sin_port = htons(Port);
	memcpy(&sock_add_emis.sin_addr.s_addr, hp->h_addr, hp->h_length);

	if (connect(s_emis, (struct sockaddr*) &sock_add_emis,size_sock)==-1) {
		perror("SendReq : Connect");
		exit(-1);
	}

	/*Emission de la requete*/
	l=write(s_emis,req,strlen(req));
	close (s_emis); 

}

/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

int main (int argc, char* argv[]) {
	struct sockaddr_in sock_add, sock_add_dist;
	int size_sock;
	int s_ecoute, s_service;
	char texte[40];
	int i,l,r;
	float t;

	file* filei = NULL;

	int H=0; //Horloge du site
	int Hreq=0; //Estampillage lors d'envoi de la requete
	int Sc=0; //Est on en section critique ?
	int demandeSc = 0;//Venons nous de demander une SC ?

	int PortBase=-1; /*Numero du port de la socket a` creer*/
	int NSites=-1; /*Nb total de sites*/

	if (argc<3) {
		printf("Erreur: il faut donner au moins 2 sites pour faire fonctionner l'application: NumeroPortBase et liste_des_sites");
		exit(-1);
	}

	/*----Nombre de sites (adresses de machines)---- */
	NSites=argc-2;

	// Tableau pour gérer les réponses
	int Reponses[NSites];
	for(int j=0;j<NSites;++j){
		Reponses[NSites] = 0;
	}

	/*CREATION&BINDING DE LA SOCKET DE CE SITE*/
	PortBase=atoi(argv[1])+GetSitePos(NSites, argv);
	printf("Numero de port de ce site %d\n",PortBase);
	
	srand(PortBase);

	sock_add.sin_family = AF_INET;
	sock_add.sin_addr.s_addr= htons(INADDR_ANY);  
	sock_add.sin_port = htons(PortBase);

	if ( (s_ecoute=socket(AF_INET, SOCK_STREAM,0))==-1) {
		perror("Creation socket");
		exit(-1);
	}

	if ( bind(s_ecoute,(struct sockaddr*) &sock_add, \
				sizeof(struct sockaddr_in))==-1) {
		perror("Bind socket");
		exit(-1);
	}

	listen(s_ecoute,30);
	/*----La socket est maintenant cre'e'e, binde'e et listen----*/

	if (GetSitePos(NSites, argv) ==0) { 
		/*Le site 0 attend une connexion de chaque site : */
		for(i=0;i<NSites-1;i++) 
			WaitSync(s_ecoute);
		printf("Toutes les synchros recues \n");fflush(0);
		/*et envoie un msg a chaque autre site pour les synchroniser */
		for(i=0;i<NSites-1;i++) 
			SendSync(argv[3+i], atoi(argv[1])+i+1);
	} else {
		/* Chaque autre site envoie un message au site0 
		   (1er  dans la liste) pour dire qu'il est lance'*/
		SendSync(argv[2], atoi(argv[1]));
		/*et attend un message du Site 0 envoye' quand tous seront lance's*/
		printf("Wait Synchro du Site 0\n");fflush(0);
		WaitSync(s_ecoute);
		printf("Synchro recue de  Site 0\n");fflush(0);
	}


	/* Passage en mode non bloquant du accept pour tous*/
	/*---------------------------------------*/
	fcntl(s_ecoute,F_SETFL,O_NONBLOCK);
	size_sock=sizeof(struct sockaddr_in);

	/* Boucle infini*/
	while(1) {

		/* On commence par tester l'arrivée d'un message */
		s_service=accept(s_ecoute,(struct sockaddr*) &sock_add_dist,&size_sock);
		if (s_service>0) {
			/*Extraction et affichage du message */
			l=read(s_service,texte,39);
			texte[l] ='\0';
			printf("Message recu : %s\n",texte); fflush(0);
			close (s_service);

			//Format du message recu (envoyé par site j) "TYPE SitePos Estampille"
			//TYPE = char * = "Requete", "Reponse", "Liberation"
			//SitePos = int (Pos du site j)
			//Estampille = int (estampille de site j)
			int n1 = count_occurences(texte,' ');
			if(n1 <2){
				printf("Requete invalide\n");
				continue;	
			}
			char** liste1 = split(texte,n1," ");			

			char* type = strdup(liste1[0]);
			int sitePos = atoi(liste1[1]);
			int estampille = atoi(liste1[2]);
			
			printf("type: |%s|\n",type);
			printf("Pos: |%d|\n",sitePos);
			printf("Estampille: |%d|\n",estampille);
			//Mise à jour de H = max(H,estampille recue)
			H = H > estampille ? H : estampille; 

			//SI on recoit Reponse
			//Verifier si tout le monde à répondu
			// On entre en Sc si requête en tête de file
			if(strcmp(type,"Reponse") == 0){
				printf("On a recu une Reponse\n");
				if(verifierNombreReponses(Reponses,NSites)==0){
					printf("Reponses pas toutes recues\n");
					continue;
				}
				if(verifierSiPremier(filei, Hreq,GetSitePos(NSites,argv))){
					printf("On rentre en section critique\n");
					Sc=1;
					demandeSc=0;
				}
			}
			//SI on recoit Requete
			//Ajouter Requete dans file
			else if(strcmp(type,"Requete") == 0){
				printf("On a recu une Requete\n");
				inserer(&filei, estampille, sitePos);
							
				char* req = buildRequete("Reponse",GetSitePos(NSites, argv),H);
				printf("Sending back  \'%s\' to : %s, %d\n",req,argv[sitePos+2],atoi(argv[1])+sitePos); 	
				SendReq(req,argv[sitePos+2],PortBase+sitePos);	
				

				free(req);
			}
			//SI on recoit Liberation
			//Enlever Requete (il va falloir retrouver Requete, peut être passée avec Libé ?) ed file
			else if(strcmp(type,"Liberation") == 0){
				printf("On a recu une Liberation\n");
				retirer(&filei,estampille,sitePos);
			}


			for(int i=0;i<=n1;++i){
				free(liste1[i]);
			}
			free(liste1);
			free(type);
		}


		nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);

		/* Petite boucle d'attente : c'est ici que l'on peut faire des choses*/
		r = rand()%100;
		if(r < 50){
			//On mange des pruneaux
			H++;
			printf("|");fflush(0);

		}
		else if(r<80 && Sc==1){
			//On veut sortir de section critique
			printf("\nSortie de section critique\n");

			//TODO Envoyer Libération aux autres machines
			char* req = buildRequete("Liberation",GetSitePos(NSites, argv),Hreq);

			for(i=0;i<NSites-1;i++){
				if(i != GetSitePos(NSites,argv))
					SendReq(req,argv[3+i], atoi(argv[1])+i+1);
			}
			// retirer Requete de file
			retirer(&filei, Hreq, GetSitePos(NSites, argv));
			Sc=0;
			demandeSc=0;

			free(req);
		}
		else if(demandeSc==0 && r>95){
			// On veut ici faire la demande pour entrer en section critique
			// Placer sa requete dans File i
			//TODO envoyer Requete aux autres machines
			char* req = buildRequete("Requete",GetSitePos(NSites, argv),H);
			Hreq = H;
			demandeSc=1;	
			printf("\nDemande d'entree en section critique\n");
			inserer(&filei,Hreq,GetSitePos(NSites,argv));
			printFile(filei);
			//Envoi requete
			for(i=0;i<=NSites-1;i++){
				printf("%d sur %d\n",i,NSites);
				if(i != GetSitePos(NSites,argv)){
					SendReq(req,argv[3+i], atoi(argv[1])+i+1);
					printf("Sending\n");
				}
			}

			free(req);
		}


		printf(".");fflush(0); /* pour montrer que le serveur est actif*/
	}


	close (s_ecoute);  
	return 0;
}


