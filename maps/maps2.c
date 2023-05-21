#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define M_PI 3.14
#include <math.h>
#include "libxml/parser.h"
#include "libxml/tree.h"

#define MAX_LINE_LENGTH 1000

typedef struct 
{
    xmlNodePtr way;
    xmlChar** nodes;
    int nodecount;

} Ways;

typedef struct 
{   
    double lat;
    double lon;
    xmlNodePtr node;

} Nodes;

xmlChar** countNodeOccurrences(xmlNodePtr rootNode, const xmlChar* nodeId, int* occurrenceCount) {
    xmlChar** wayid = NULL;
    xmlNodePtr wayNode;
    // Parcourir les nœuds du document
    for (wayNode = rootNode->children; wayNode != NULL; wayNode = wayNode->next) {
        // Vérifier si le nœud est de type "way"
        if (xmlStrcmp(wayNode->name, (const xmlChar*)"way") == 0) {
            // Vérifier si le nœud est référencé dans les nœuds de la way
            xmlNodePtr ndNode;
            wayid = realloc(wayid,(*occurrenceCount + 1) * sizeof(xmlChar));
            for (ndNode = wayNode->children; ndNode != NULL; ndNode = ndNode->next) {
                if (xmlStrcmp(ndNode->name, (const xmlChar*)"nd") == 0) {
                    xmlChar* ref = xmlGetProp(ndNode, (const xmlChar*)"ref");
                    if (ref != NULL && xmlStrcmp(ref, nodeId) == 0) {
                        (*occurrenceCount)++;
                        wayid[*occurrenceCount] = ref;
                    }
                    xmlFree(ref);
                }
            }
        }
    }
    return wayid;
}


Ways countNodesInWay(xmlNodePtr wayNode) {
    int count = 0;
    Ways way;
    way.way = wayNode;
    way.nodes = NULL;
    way.nodecount = 0;

    // Parcourir les enfants du nœud way
    xmlNodePtr childNode = wayNode->children;
    while (childNode != NULL) {
        // Vérifier si le nœud est de type "nd"
        if (xmlStrcmp(childNode->name, (const xmlChar*)"nd") == 0) {
            xmlChar* ref = xmlGetProp(childNode, (const xmlChar*)"ref");
            if (ref != NULL) {
                way.nodes = realloc(way.nodes, (count + 1) * sizeof(xmlNodePtr));
                if (way.nodes == NULL) {
                    printf("Erreur lors de l'allocation mémoire pour les nœuds.\n");
                    return way;
                }
                way.nodes[count] = ref;
                count++;
            }
        }

        // Passer au prochain nœud enfant
        childNode = childNode->next;
    }
    way.nodecount = count;

    return way;
}



/*int main(int argc, char **argv) {

    xmlNodePtr rootNode, wayNode;

    // Vérifie que le nom de fichier est passé en argument de la ligne de commande
    if (argc != 2) {
        printf("Veuillez spécifier le nom de fichier XML en entrée.\n");
        return 1;
    }

    // Ouvre le fichier XML en lecture
    xmlDoc* doc = xmlReadFile(argv[1], NULL, 0);
    if (doc == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", argv[1]);
        return 1;
    }
    // Obtenir le nœud racine du document
    rootNode = xmlDocGetRootElement(doc);

    // Parcourir les nœuds du document
    for (wayNode = rootNode->children; wayNode != NULL; wayNode = wayNode->next) {
        // Vérifier si le nœud est de type "way"
        if (xmlStrcmp(wayNode->name, (const xmlChar*)"way") == 0) {
            // Compter les nœuds associés à cette way
            Ways way = countNodesInWay(wayNode);
            const xmlChar* id = xmlGetProp(wayNode, (const xmlChar*)"id");
            printf("La way : %s a %d nœud qui sont les noeuds : \n ",id, way.nodecount);
            for(size_t i = 0; i < way.nodecount;i++)
            {
                int occurrenceCount = countNodeOccurrences(rootNode, way.nodes[i]);
                printf("Occurrences du nœud avec l'ID %s dans les ways : %d\n", way.nodes[i], occurrenceCount);

                //printf("%s, ", way.nodes[i]);
            }
            printf("\n");
        }
    }

    // Libérer les ressources
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}*/


typedef struct {
    double lat;
    double lon;
} Coordinate;

double calculateDistance(Coordinate coord1, Coordinate coord2) {
    double dLat = (coord2.lat - coord1.lat) * M_PI / 180.0;
    double dLon = (coord2.lon - coord1.lon) * M_PI / 180.0;
    double a = sin(dLat / 2.0) * sin(dLat / 2.0) +
               cos(coord1.lat * M_PI / 180.0) * cos(coord2.lat * M_PI / 180.0) *
               sin(dLon / 2.0) * sin(dLon / 2.0);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = 6371.0 * c; // Rayon de la Terre en kilomètres
    return distance;
}

xmlNodePtr findClosestNode(xmlNodePtr rootNode, xmlNodePtr targetNode, xmlNodePtr wayNode) {
    double targetLat, targetLon;
    sscanf((const char*)xmlGetProp(targetNode, (const xmlChar*)"lat"), "%lf", &targetLat);
    sscanf((const char*)xmlGetProp(targetNode, (const xmlChar*)"lon"), "%lf", &targetLon);

    double closestDistance = INFINITY;
    xmlNodePtr closestNode = NULL;

    xmlNodePtr ndNode;
    for (ndNode = wayNode->children; ndNode != NULL; ndNode = ndNode->next) {
        if (xmlStrcmp(ndNode->name, (const xmlChar*)"nd") == 0) {
            xmlChar* ref = xmlGetProp(ndNode, (const xmlChar*)"ref");
            xmlNodePtr relatedNode = NULL;
            xmlNodePtr childNode;
            for (childNode = rootNode->children; childNode != NULL; childNode = childNode->next) {
                if (xmlStrcmp(childNode->name, (const xmlChar*)"node") == 0) {
                    xmlChar* nodeId = xmlGetProp(childNode, (const xmlChar*)"id");
                    if (xmlStrcmp(nodeId, ref) == 0) {
                        relatedNode = childNode;
                        break;
                    }
                    xmlFree(nodeId);
                }
            }
            if (relatedNode != NULL) {
                double nodeLat, nodeLon;
                sscanf((const char*)xmlGetProp(relatedNode, (const xmlChar*)"lat"), "%lf", &nodeLat);
                sscanf((const char*)xmlGetProp(relatedNode, (const xmlChar*)"lon"), "%lf", &nodeLon);
                Coordinate targetCoord = { targetLat, targetLon };
                Coordinate nodeCoord = { nodeLat, nodeLon };
                double distance = calculateDistance(targetCoord, nodeCoord);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestNode = relatedNode;
                }
            }
            xmlFree(ref);
        }
    }

    return closestNode;
}

xmlNodePtr findClosestNodeInWay(xmlNodePtr rootNode, xmlNodePtr targetNode, const char* wayId) {
    xmlNodePtr wayNode;
    for (wayNode = rootNode->children; wayNode != NULL; wayNode = wayNode->next) {
        if (xmlStrcmp(wayNode->name, (const xmlChar*)"way") == 0) {
            xmlChar* id = xmlGetProp(wayNode, (const xmlChar*)"id");
            if (xmlStrcmp(id, (const xmlChar*)wayId) == 0) {
                xmlNodePtr closestNode = findClosestNode(rootNode, targetNode, wayNode);
                xmlFree(id);
                return closestNode;
            }
            xmlFree(id);
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    // Vérifier que le nom de fichier est passé en argument de la ligne de commande
    if (argc != 2) {
        printf("Veuillez spécifier le nom de fichier XML en entrée.\n");
        return 1;
    }

    // Ouvre le fichier XML en lecture
    xmlDoc* doc = xmlReadFile(argv[1], NULL, 0);
    if (doc == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", argv[1]);
        return 1;
    }

    xmlNodePtr rootNode = xmlDocGetRootElement(doc);
    xmlNodePtr targetNode = rootNode->children;
    const char* wayId = "YOUR_WAY_ID_HERE";
    // Parcourir les nœuds du document
    for (xmlNodePtr wayNode = rootNode->children; wayNode != NULL; wayNode = wayNode->next) {
        // Vérifier si le nœud est de type "way"
        if (xmlStrcmp(wayNode->name, (const xmlChar*)"way") == 0) {
            // Compter les nœuds associés à cette way
            Ways way = countNodesInWay(wayNode);
            const xmlChar* id = xmlGetProp(wayNode, (const xmlChar*)"id");
            printf("La way : %s a %d nœud qui sont les noeuds : \n ",id, way.nodecount);
            for(size_t i = 0; i < way.nodecount;i++)
            {
                int occ = 0;
                xmlChar** wayid = countNodeOccurrences(rootNode, way.nodes[i],&occ);
                for(size_t i = 0; i < occ;i++)
                {
                    wayId = wayid[i];
                    xmlNodePtr closestNode = findClosestNodeInWay(rootNode, targetNode, wayId);
                    if (closestNode != NULL) {
                        xmlChar* nodeId = xmlGetProp(closestNode, (const xmlChar*)"id");
                        printf("Le noeud le plus proche du noeud %s via la way %s est le noeud %s.\n",
                            (const char*)xmlGetProp(targetNode, (const xmlChar*)"id"),
                            wayId,
                            (const char*)nodeId);
                        xmlFree(nodeId);
                    } else {
                        printf("Aucun noeud trouvé pour la way %s.\n", wayId);
                    }
                }

                //printf("%s, ", way.nodes[i]);
            }
            printf("\n");
        }
    }
    xmlNodePtr closestNode = findClosestNodeInWay(rootNode, targetNode, wayId);
    if (closestNode != NULL) {
        xmlChar* nodeId = xmlGetProp(closestNode, (const xmlChar*)"id");
        printf("Le noeud le plus proche du noeud %s via la way %s est le noeud %s.\n",
               (const char*)xmlGetProp(targetNode, (const xmlChar*)"id"),
               wayId,
               (const char*)nodeId);
        xmlFree(nodeId);
    } else {
        printf("Aucun noeud trouvé pour la way %s.\n", wayId);
    }

    // Libérer les ressources
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}