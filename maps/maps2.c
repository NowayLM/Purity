#include <stdio.h>
#include <string.h>
#include "libxml/parser.h"
#include "libxml/tree.h"

#define MAX_LINE_LENGTH 1000

int main(int argc, char **argv)
{
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
            sprintf(line, "/%s,%i,%i", node_id, (int) (atof(node_lat) * 100000), (int) (atof(node_lon) * 100000));

            // Parcours les noeuds fils du sommet pour récupérer les liens
            int link_count = 0;
            for (xmlNode *node = cur_node->children; node; node = node->next) {
                if (xmlStrcmp(node->name, (const xmlChar *)"tag") == 0) {
                    char *tag_key = (char *)xmlGetProp(node, (const xmlChar *)"k");
                    char *tag_value = (char *)xmlGetProp(node, (const xmlChar *)"v");

                    // Si le tag correspond au nombre de liens, récupère la valeur
                    /*if (strcmp(tag_key, "link_count") == 0) {
                        link_count = atoi(tag_value);
                    }*/
                    xmlFree(tag_key);
                    xmlFree(tag_value);
                }
		link_count++;
            }
            // Ajoute les lignes pour chaque lien
            for (int i = 1; i <= link_count; i++) {
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
            }
            if (link_count != 0)
            {
                // Affiche la ligne complète
                printf("%s\n", line);
            } 

            xmlFree(node_id);
            xmlFree(node_lat);
            xmlFree(node_lon);
        }
    }

    // Libère la mémoire allouée
    xmlFreeDoc(doc);

    return 0;
}
