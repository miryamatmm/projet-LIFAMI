# Projet de développement d'un mini jeu en C++ avec Grapic

## Tank Battle

## Description
Le jeu Tank Battle est un jeu multijoueur inspiré du jeu 2,3,4 players. Il offre également une version solo avec des cibles. Les joueurs incarnent des tanks et l'objectif est d'éliminer l'adversaire en récupérant des bonus pour obtenir un avantage stratégique.

## Partie Science
Il était imposé une partie science dans notre jeu, j'ai donc incorporé les éléments de science suivant dans le code :
- **Déplacements :** Utilisation de la géométrie complexe pour les mouvements des tanks.
- **Projectiles :** Utilisation des forces pour simuler le tir des projectiles.

## Ressources Graphiques
Les images utilisées dans le jeu ont été obtenues à partir du site eggpng. En outre, certaines images, comme la carte, ont été créées en combinant plusieurs sources avec le logiciel GIMP.

## Historique du Développement

### Semaine 1
- Mise en place des structures pour les projectiles et les tanks.
- Mise en œuvre des fonctions de dessin et de mise à jour associées (les mises à jour ne sont pas encore finalisées).
- Implémentation du mouvement des tanks avec les touches de déplacement (haut, bas, gauche, droite).

### Semaine 2
- Ajout de la fonctionnalité de tir pour les projectiles.
- Tentative d'ajout d'un intervalle de temps entre les tirs pour une meilleure gestion (au lieu de tirs simultanés).
- Changement de la méthode de déplacement en utilisant cos et sin avec un angle pour définir la direction.

### Semaine 3
- Implémentation du fonctionnement des projectiles.
- Début de l'implémentation des collisions, rencontrant des difficultés, notamment des dégâts excessifs au lieu d'un seul.

### Semaine 4
- Intégration du mode de jeu solo avec des cibles et des obstacles.
- Ajout de toutes les images nécessaires.
- Création d'un menu pour les modes de jeu, les vies et le temps pour le mode solo.
- Résolution des problèmes liés à l'utilisation du temps écoulé (elapsedTime).

## Contrôles
- **Joueur 1 :** Utilise les touches fléchées (haut pour avancer, gauche et droite pour changer de direction).
- **Joueur 2 :** Utilise la touche "e" pour avancer, et les touches "s" et "f" pour changer de direction.

## Notes sur le Code
Le code est largement commenté pour faciliter la compréhension, sauf pour les éléments évidents.
Le projet peut être compilé avec Code::Blocks en utilisant la bibliothèque [grapic](https://perso.liris.cnrs.fr/alexandre.meyer/grapic/html/) à télécharger.

## Auteur
Atamna Miryam

