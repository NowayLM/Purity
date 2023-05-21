#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define M_PI 3.14
#include <math.h>
#include "libxml/parser.h"
#include "libxml/tree.h"

#define MAX_LINE_LENGTH 1000

typedef struct Node {

    double lat;
    double lon;
    xmlChar* nodeid;
    size_t nombreDeWays;
    size_t nombreDeFreres;
    struct Frere* listeFreres;
    struct Node* suivant;

} Nodes;

typedef struct Way {

   xmlNodePtr way;
   xmlChar** nodes;
   size_t nodecount;
   struct Way* suivant;
} Ways;

typedef struct Frere {
    struct Node* frere;
    struct Frere* suivant;
} Freres;

typedef struct {
    double x;
    double y;
} Coordinate;


void updateMinMaxCoordinates(Coordinate* minCoord, double x, double y) {
    if (x < minCoord->x)
        minCoord->x = x;
    if (y < minCoord->y)
        minCoord->y = y;
}

Coordinate findMinCoordinates(xmlNodePtr root) {
    Coordinate minCoord = { 999, 999 };  // Valeurs initiales élevées pour les coordonnées minimales

    // Parcourir les fils du nœud racine
    xmlNodePtr child = xmlFirstElementChild(root);
    while (child != NULL) {
        // Vérifier si le fils est un élément "node"
        if (xmlStrcmp(child->name, (const xmlChar*)"node") == 0) {
            const xmlChar* latAttr = xmlGetProp(child, (const xmlChar*)"lat");
            const xmlChar* lonAttr = xmlGetProp(child, (const xmlChar*)"lon");

            double lat = atof((const char*)latAttr);
            double lon = atof((const char*)lonAttr);

            // Mettre à jour les coordonnées minimales
            updateMinMaxCoordinates(&minCoord, lon, lat);

            xmlFree((void*)latAttr);
            xmlFree((void*)lonAttr);
        }

        // Passer au fils suivant
        child = xmlNextElementSibling(child);
    }

    return minCoord;
}

int getRandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

int getRandomValue() {
    int values[] = {30, 50, 80, 110, 130};
    int numValues = sizeof(values) / sizeof(values[0]);
    int randomIndex = rand() % numValues;
    return values[randomIndex];
}

void fillWayList(xmlNodePtr node, Ways** wayList) {
    xmlNodePtr wayNode = node->children;
    while (wayNode != NULL) {
        if (xmlStrcmp(wayNode->name, (const xmlChar*)"way") == 0) {
            Ways* newWay = malloc(sizeof(Ways));
            newWay->way = wayNode;
            newWay->nodes = NULL;
            newWay->nodecount = 0;
            newWay->suivant = NULL;

            // Parcourir les enfants du nœud way
            xmlNodePtr childNode = wayNode->children;
            int count = 0;
            while (childNode != NULL) {
                // Vérifier si le nœud est de type "nd"
                if (xmlStrcmp(childNode->name, (const xmlChar*)"nd") == 0) {
                    xmlChar* ref = xmlGetProp(childNode, (const xmlChar*)"ref");
                    if (ref != NULL) {
                        newWay->nodes = realloc(newWay->nodes, (count + 1) * sizeof(xmlChar*));
                        if (newWay->nodes == NULL) {
                            printf("Erreur lors de l'allocation mémoire pour les nœuds.\n");
                            return;
                        }
                        newWay->nodes[count] = ref;
                        count++;
                    }
                }

                // Passer au prochain nœud enfant
                childNode = childNode->next;
            }
            newWay->nodecount = count;

            // Ajouter la nouvelle way à la liste
            if (*wayList == NULL) {
                *wayList = newWay;
            } else {
                Ways* currentWay = *wayList;
                while (currentWay->suivant != NULL) {
                    currentWay = currentWay->suivant;
                }
                currentWay->suivant = newWay;
            }
        }

        // Passer au prochain nœud
        wayNode = wayNode->next;
    }
}

void printWayList(Ways* wayList) {
    Ways* currentWay = wayList;
    while (currentWay != NULL) {
        printf("Way ID: %s\n", xmlGetProp(currentWay->way, (const xmlChar*)"id"));
        printf("Number of Nodes: %zu\n", currentWay->nodecount);
        for (size_t i = 0; i < currentWay->nodecount; i++) {
            printf("Node ID: %s\n", currentWay->nodes[i]);
        }
        printf("\n");

        currentWay = currentWay->suivant;
    }
}

Nodes* ajouterNoeud(Nodes* liste, double lat, double lon, xmlChar* nodeid)
{
        Nodes* elementCourant = liste;
    while (elementCourant != NULL) {
        if (xmlStrcmp(elementCourant->nodeid, nodeid) == 0) {
            // Le nœud existe déjà dans la liste, mettez à jour les coordonnées lat/lon
            elementCourant->lat = lat;
            elementCourant->lon = lon;
            return liste;
        }
        elementCourant = elementCourant->suivant;
    }



    // Le nœud n'existe pas encore dans la liste, ajoutez-le
    Nodes* nouvelElement = malloc(sizeof(Nodes));
    nouvelElement->lat = lat;
    nouvelElement->lon = lon;
    nouvelElement->nodeid = xmlStrdup(nodeid);
    nouvelElement->nombreDeWays = 0; // Initialisez le nombre de ways à 0
    nouvelElement->listeFreres = NULL; // Initialisez la liste de frères à NULL
    nouvelElement->suivant = NULL;

    if (liste == NULL)
    {
        liste = nouvelElement;
    }
    else
    {
        Nodes* current = liste;
        while (current->suivant != NULL)
        {
            current = current->suivant;
        }
        current->suivant = nouvelElement;
    }
    return liste;

}

void incrementerNombreDeWays(Nodes* liste, xmlChar* nodeid) {
    Nodes* elementCourant = liste;
    while (elementCourant != NULL) {
        if (xmlStrcmp(elementCourant->nodeid, nodeid) == 0) {
            elementCourant->nombreDeWays++;
        }
        elementCourant = elementCourant->suivant;
    }
}

Nodes* chercherNoeud(Nodes* liste, xmlChar* nodeid) {
    Nodes* elementCourant = liste;
    while (elementCourant != NULL) {
        if (xmlStrcmp(elementCourant->nodeid, nodeid) == 0) {
            return elementCourant;
        }
        elementCourant = elementCourant->suivant;
    }
    return NULL;
}


void ajouterFrere(Nodes* noeud, Nodes* frere) {
    // Vérifier si le frère est déjà présent dans la liste
    Freres* frereCourant = noeud->listeFreres;
    while (frereCourant != NULL) {
        if (frereCourant->frere == frere) {
            // Le frère est déjà présent, pas besoin de l'ajouter
            return;
        }
        frereCourant = frereCourant->suivant;
    }

    // Ajouter le frère à la liste
    Freres* nouveauFrere = (Freres*)malloc(sizeof(Freres));
    nouveauFrere->frere = frere;
    nouveauFrere->suivant = NULL;

    if (noeud->listeFreres == NULL) {
        noeud->listeFreres = nouveauFrere;
    } else {
        frereCourant = noeud->listeFreres;
        while (frereCourant->suivant != NULL) {
            frereCourant = frereCourant->suivant;
        }
        frereCourant->suivant = nouveauFrere;
    }

    noeud->nombreDeFreres++;
}

void parcourirNoeuds(xmlNodePtr noeud, Nodes** liste) {
    xmlNodePtr noeudCourant = NULL;
    for (noeudCourant = noeud; noeudCourant != NULL; noeudCourant = noeudCourant->next) {
        if (noeudCourant->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(noeudCourant->name, (const xmlChar *)"node") == 0) {
                xmlChar* lat = xmlGetProp(noeudCourant, (const xmlChar *)"lat");
                xmlChar* lon = xmlGetProp(noeudCourant, (const xmlChar *)"lon");
                xmlChar* nodeid = xmlGetProp(noeudCourant, (const xmlChar *)"id");

                double latitude = atof((char *)lat);
                double longitude = atof((char *)lon);

                *liste = ajouterNoeud(*liste, latitude, longitude, nodeid);

                xmlFree(lat);
                xmlFree(lon);
            } else if (xmlStrcmp(noeudCourant->name, (const xmlChar *)"way") == 0) {
                xmlNodePtr nd = NULL;
                for (nd = noeudCourant->children; nd != NULL; nd = nd->next) {
                    if (nd->type == XML_ELEMENT_NODE && xmlStrcmp(nd->name, (const xmlChar *)"nd") == 0) {
                        xmlChar* ref = xmlGetProp(nd, (const xmlChar *)"ref");
                        xmlChar* nodeid = xmlStrdup(ref);
                        incrementerNombreDeWays(*liste, nodeid);

                        xmlFree(ref);
                        xmlFree(nodeid);
                    }
                }
            }
        }

        parcourirNoeuds(noeudCourant->children, liste);
    }
}

int compterFreres(Freres* listeFreres) {
    int count = 0;
    Freres* frereCourant = listeFreres;
    while (frereCourant != NULL) {
        count++;
        frereCourant = frereCourant->suivant;
    }
    return count;
}

void afficherListeNoeuds(Nodes* liste) {
    Nodes* elementCourant = liste;
    int count = 0;
    while (elementCourant != NULL) {
        printf("Node ID: %s(%i), Latitude: %f, Longitude: %f, Nb ways : %li,Nb frère %i\n", (char *)elementCourant->nodeid,count, elementCourant->lat, elementCourant->lon,elementCourant->nombreDeWays,compterFreres(elementCourant->listeFreres));
        count++;
        elementCourant = elementCourant->suivant;
    }
}

void libererListeNoeuds(Nodes* liste) {
    Nodes* elementCourant = liste;
    while (elementCourant != NULL) {
        Nodes* elementSuivant = elementCourant->suivant;
        xmlFree(elementCourant->nodeid);
        free(elementCourant);
        elementCourant = elementSuivant;
    }
}

void trouverFrereLePlusProche(Nodes* liste, Ways* wayList, xmlChar* nodeid) {
    // Rechercher le nœud correspondant au nodeid
    Nodes* noeud = chercherNoeud(liste, nodeid);
    if (noeud == NULL) {
        printf("Noeud non trouvé.\n");
        return;
    }

    // Parcourir la liste des ways
    Ways* currentWay = wayList;
    while (currentWay != NULL) {
        // Vérifier si le nodeid est présent dans la way
        for (size_t i = 0; i < currentWay->nodecount; i++) {
            if (xmlStrcmp(currentWay->nodes[i], nodeid) == 0) {
                // Le nodeid est présent dans la way, chercher le frère le plus proche
                double minDistance = INFINITY;
                Nodes* frerePlusProche = NULL;

                for (size_t j = 0; j < currentWay->nodecount; j++) {
                    // Exclure le nœud lui-même de la recherche
                    if (i != j) {
                        Nodes* frere = chercherNoeud(liste, currentWay->nodes[j]);
                        double distance = sqrt(pow(frere->lat - noeud->lat, 2) + pow(frere->lon - noeud->lon, 2));
                        if (distance < minDistance) {
                            minDistance = distance;
                            frerePlusProche = frere;
                        }
                    }
                }

                if (frerePlusProche != NULL) {
                    // Ajouter le frère le plus proche à la liste des frères du nœud
                    ajouterFrere(noeud, frerePlusProche);
                    ajouterFrere(frerePlusProche, noeud);
                    //printf("Le frere le plus proche pour le Node ID %s est le Node ID %s.\n", nodeid, (char *)frerePlusProche->nodeid);
                } else {
                    //printf("Aucun frere trouvé pour le Node ID %s.\n", nodeid);
                }

                return;
            }
        }

        currentWay = currentWay->suivant;
    }

    printf("Le Node ID %s n'est pas présent dans une way.\n", nodeid);
}

int findnodeindex(xmlChar* nodeid,Nodes* liste)
{
    int count = 0;
    Nodes* current = liste;
    while (current->suivant != NULL)
    {
        if (current->nodeid == nodeid)
        {
            return count;
        }
        count++;
        current = current->suivant;
    }
    return count;
}

void ecrireDansFichier(const char* nomFichier, const char* contenu) 
{
    FILE* fichier = fopen(nomFichier, "a"); // Ouvre le fichier en mode écriture ("w")

    if (fichier != NULL) {
        fprintf(fichier, "%s\n", contenu); // Écrit le contenu dans le fichier
        fclose(fichier); // Ferme le fichier
    } else {
        printf("Impossible d'ouvrir le fichier.\n");
    }
}

int main(int argc, char **argv) 
{

    xmlNodePtr racine;

    // Charger le document XML
    // Vérifie que le nom de fichier est passé en argument de la ligne de commande
    if (argc != 3) {
        printf("Veuillez spécifier le nom de fichier XML en entrée et le nom du fichier destination.\n");
        return 1;
    }
    const char* filename = argv[2];
    // Ouvre le fichier XML en lecture
    xmlDoc *doc = xmlReadFile(argv[1], NULL, 0);
    if (doc == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", argv[1]);
        return 1;
    }
    // Obtenir la racine du document
    racine = xmlDocGetRootElement(doc);
    if (racine == NULL) {
        fprintf(stderr, "Document XML vide.\n");
        xmlFreeDoc(doc);
        return 1;
    }
    xmlNodePtr rootNode;
    rootNode = xmlDocGetRootElement(doc);
    Coordinate minCoordinates = findMinCoordinates(rootNode);
    double min;
    if (minCoordinates.x < minCoordinates.y)
    {
        min = minCoordinates.x;
    }
    else
    {
        min = minCoordinates.y;
    }
    min = min * 100000;
    Nodes* liste = NULL;
    Ways* listway = NULL;
    fillWayList(rootNode,&listway);
    // Parcourir les nœuds du document XML
    parcourirNoeuds(racine, &liste);
    //printWayList(listway);
    
    // Afficher la liste des nœuds

    //afficherListeNoeuds(liste);

    Nodes* listbonus = liste;
    Nodes* elementCourant = liste;
    int count = 0;
    while (elementCourant != NULL) {
        char line[MAX_LINE_LENGTH];
        memset(line, 0, MAX_LINE_LENGTH);

        trouverFrereLePlusProche(liste,listway,elementCourant->nodeid);
        sprintf(line,"/%i,%i,%i,%i,",count,(int)(elementCourant->lat * 100000 - min), (int)(elementCourant->lon *100000 - min),compterFreres(elementCourant->listeFreres));
        Freres* frere = elementCourant->listeFreres;
    
        while (frere->suivant != NULL) {
            char link_line[MAX_LINE_LENGTH];
            memset(link_line, 0, MAX_LINE_LENGTH);
            sprintf(link_line,"%i-%i-%i_", findnodeindex(frere->frere->nodeid,listbonus),getRandomInt(1,3),getRandomValue());
            strcat(line, link_line);
            frere = frere->suivant;
        }
        char link_linef[MAX_LINE_LENGTH];
        memset(link_linef, 0, MAX_LINE_LENGTH);
        sprintf(link_linef,"%i-%i-%i*", findnodeindex(frere->frere->nodeid,listbonus),getRandomInt(1,3),getRandomValue());
        strcat(line,link_linef);
        ecrireDansFichier(filename,line);
        //printf("Node ID: %s(%i), Latitude: %f, Longitude: %f, Nb ways : %li,Nb frère %i\n", (char *)elementCourant->nodeid,count, elementCourant->lat, elementCourant->lon,elementCourant->nombreDeWays,compterFreres(elementCourant->listeFreres));
        count++;
        elementCourant = elementCourant->suivant;
    }

    // Libérer la mémoire
    libererListeNoeuds(liste);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    return 0;
}
