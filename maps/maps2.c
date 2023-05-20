#include <stdio.h>
#include <string.h>
#include "libxml/parser.h"
#include "libxml/tree.h"

#define MAX_LINE_LENGTH 1000

typedef struct {
    double x;
    double y;
} Coordinate;


int countNodeHighways(xmlNodePtr node) {
    int count = 0;

    // Parcourir les nœuds enfants du nœud actuel
    xmlNodePtr child = node->children;
    while (child != NULL) {
        // Vérifier si le nœud est un élément "way"
        if (child->type == XML_ELEMENT_NODE && xmlStrcmp(child->name, (const xmlChar*)"way") == 0) {
            // Parcourir les sous-éléments du nœud "way"
            xmlNodePtr subChild = child->children;
            while (subChild != NULL) {
            // Vérifier si le sous-élément est un élément "tag" avec l'attribut "k" égal à "highway"
                if (subChild->type == XML_ELEMENT_NODE && xmlStrcmp(subChild->name, (const xmlChar*)"tag") == 0) 
                {
                    xmlChar* kAttr = xmlGetProp(subChild, (const xmlChar*)"k");
                    if (kAttr != NULL && xmlStrcmp(kAttr, (const xmlChar*)"highway") == 0) 
                    {
                        count++;
                    }
                    xmlFree(kAttr);
                }
                subChild = subChild->next;
            }
        }
        child = child->next;
    }

    return count;
}

//Fonction nous permettant de coompter lme nombre de fils d'un noeud
int countNodeChildren(xmlNodePtr node) 
{
    int count = 0;
    xmlNodePtr child = node->children;

    // Parcourir les enfants du nœud
    while (child != NULL) {
        // Vérifier si l'élément est un nœud
        if (child->type == XML_ELEMENT_NODE) {
            count++;
        }
        child = child->next;
    }

    return count;
}

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
    
    // Récupère le noeud racine
    xmlNode *root_element = xmlDocGetRootElement(doc);
    Coordinate minCoordinates = findMinCoordinates(root_element);
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
    int node_value = 0;
    // Parcours tous les noeuds fils du noeud racine
    for (xmlNode *cur_node = root_element->children; cur_node; cur_node = cur_node->next) 
    {   

        // Si le noeud est un sommet
        if (xmlStrcmp(cur_node->name, (const xmlChar *)"node") == 0) {
            char line[MAX_LINE_LENGTH];
            memset(line, 0, MAX_LINE_LENGTH);

            // Récupère l'ID, la latitude et la longitude du sommet
            char *node_id = (char *)xmlGetProp(cur_node, (const xmlChar *)"id");
            char *node_lat = (char *)xmlGetProp(cur_node, (const xmlChar *)"lat");
            char *node_lon = (char *)xmlGetProp(cur_node, (const xmlChar *)"lon");
            int childcount = countNodeHighways(cur_node);
            // Construit la ligne dans le format de fichier personnalisé
            sprintf(line, "/%i,%i,%i,%i", node_value, (int) (atof(node_lat) * 100000 - min), (int) (atof(node_lon) * 100000 - min),childcount);

            // Parcours les noeuds fils du sommet pour récupérer les liens
            int link_count = 0;

                        xmlNode *child = xmlFirstElementChild(cur_node);
            while (child != NULL) {

                // Vérifier si le fils est un élément "way"
                if (xmlStrcmp(child->name, (const xmlChar *)"way") == 0) {
                    link_count++;
                    char link_id[20];
                    char congestion[10];
                    char maxspeed[10];

                    xmlNode *link_child = xmlFirstElementChild(child);
                    while (link_child != NULL) {
                        // Vérifier si le fils est un élément "tag"
                        if (xmlStrcmp(link_child->name, (const xmlChar *)"tag") == 0) {
                            // Vérifier les attributs "k" et "v" pour extraire les informations de lien
                            xmlChar *k_attr = xmlGetProp(link_child, (const xmlChar *)"k");
                            xmlChar *v_attr = xmlGetProp(link_child, (const xmlChar *)"v");

                            if (xmlStrcmp(k_attr, (const xmlChar *)"name") == 0) {
                                // Ignorer l'attribut "name" pour l'instant
                            } else if (xmlStrcmp(k_attr, (const xmlChar *)"congestion") == 0) {
                                strcpy(congestion, (const char *)v_attr);
                            } else if (xmlStrcmp(k_attr, (const xmlChar *)"maxspeed") == 0) {
                                strcpy(maxspeed, (const char *)v_attr);
                            }

                            xmlFree(k_attr);
                            xmlFree(v_attr);
                        }

                        link_child = xmlNextElementSibling(link_child);
                    }

                    sprintf(link_id, "m%i", link_count);

                    // Ajoute la ligne pour le lien au format de fichier personnalisé
                    char link_line[MAX_LINE_LENGTH];
                    memset(link_line, 0, MAX_LINE_LENGTH);
                    sprintf(link_line, "_%s-%s-%s", link_id, congestion, maxspeed);
                    strcat(line, link_line);
                }

                // Passer au fils suivant
                child = xmlNextElementSibling(child);
            }

            // Affiche la ligne complète
            ecrireDansFichier(filename,line);
            //printf("%s\n", line);

            xmlFree(node_id);
            xmlFree(node_lat);
            xmlFree(node_lon);
            node_value++;
        }
    }

    // Libère la mémoire allouée
    xmlFreeDoc(doc);

    return 0;
}
