#include <stdio.h>
#include <string.h>
#include "libxml/parser.h"
#include "libxml/tree.h"

#define MAX_LINE_LENGTH 1000

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
            updateMinMaxCoordinates(&minCoord, NULL, lon, lat);

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
        printf("Écriture terminée avec succès.\n");
    } else {
        printf("Impossible d'ouvrir le fichier.\n");
    }
}

int main(int argc, char **argv)
{
    const char* filename = "PurityMap.txt";
    // Vérifie que le nom de fichier est passé en argument de la ligne de commande
    if (argc != 2) {
        printf("Veuillez spécifier le nom de fichier XML en entrée.\n");
        return 1;
    }

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

            // Construit la ligne dans le format de fichier personnalisé
            sprintf(line, "/%i,%i,%i,%f,%f", node_value, (int) (atof(node_lat) * 100000 - min), (int) (atof(node_lon) * 100000 - min));

            // Parcours les noeuds fils du sommet pour récupérer les liens
            int link_count = 0;

            xmlNode* child = xmlFirstElementChild(cur_node);
            while (child != NULL) 
            {
                // Vérifier si le fils est un élément "highway"
                if (xmlStrcmp(child->name, (const xmlChar*)"highway") == 0) 
                {
                    link_count++;
                    const xmlChar* linkid = xmlGetProp(child, (const xmlChar*)"nodeid");
                    const xmlChar* maxspeed = xmlGetProp(child, (const xmlChar*)"maxspeed");
                    const xmlChar* congestion = xmlGetProp(child, (const xmlChar*)"congestion");

                    // Ajoute la ligne pour le lien au format de fichier personnalisé
                    char link_line[MAX_LINE_LENGTH];
                    memset(link_line, 0, MAX_LINE_LENGTH);
                    sprintf(link_line, ",%s-%s-%s_", linkid, congestion, maxspeed);
                    strcat(line, link_line);

                    xmlFree((void*)linkid);
                    xmlFree((void*)congestion);
                    xmlFree((void*)maxspeed);
                }
                // Passer au fils suivant
                child = xmlNextElementSibling(child);
            }
            /*for (xmlNode *node = cur_node->children; node; node = node->next) {
                if (xmlStrcmp(node->name, (const xmlChar *)"tag") == 0) {
                    char *tag_key = (char *)xmlGetProp(node, (const xmlChar *)"k");
                    char *tag_value = (char *)xmlGetProp(node, (const xmlChar *)"v");

                    // Si le tag correspond au nombre de liens, récupère la valeur
                    if (strcmp(tag_key, "link_count") == 0) {
                        link_count = atoi(tag_value);
                    }
                    xmlFree(tag_key);
                    xmlFree(tag_value);
                }
                link_count++;
            }*/
            // Ajoute les lignes pour chaque lien
            /*for (int i = 1; i <= link_count; i++) {
                char *link_id = (char *)xmlGetProp(cur_node, (const xmlChar *)"link_id");
                char *link_capacity = (char *)xmlGetProp(cur_node, (const xmlChar *)"link_capacity");
                char *link_speed = (char *)xmlGetProp(cur_node, (const xmlChar *)"link_speed");

                // Ajoute la ligne pour le lien au format de fichier personnalisé
                char link_line[MAX_LINE_LENGTH];
                memset(link_line, 0, MAX_LINE_LENGTH);
                sprintf(link_line, ",%s-%s-%s_", link_id, link_capacity, link_speed);
                strcat(line, link_line);

                xmlFree(link_id);
                xmlFree(link_capacity);
                xmlFree(link_speed);
            }*/

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
