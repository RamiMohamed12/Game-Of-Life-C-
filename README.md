**Introduction :**

Le Jeu de la Vie est un automate cellulaire conçu par le mathématicien britannique John Conway en 1970. Il s'agit d'une simulation où une grille est peuplée de cellules qui peuvent être vivantes ou mortes. L'évolution des cellules est déterminée par des règles simples mais puissantes. Règles du jeu :

Une cellule vivante avec moins de deux voisins vivants meurt (sous-population).

Une cellule vivante avec plus de trois voisins vivants meurt (surpopulation).

Une cellule vivante avec deux ou trois voisins vivants reste en vie.

Une cellule morte avec exactement trois voisins vivants devient vivante (naissance).

Le jeu se déroule en une série de générations (ou itérations). Chaque génération est calculée à partir de l'état précédent en appliquant les règles à chaque cellule.


**Architecture du Project :**

Nous travaillons sur une simulation du Jeu de la Vie (Game of Life) en développant deux versions distinctes :

Une version en mode console : Dans cette version, la simulation sera entièrement affichée dans le terminal. L'objectif est de garder ce mode simple et fonctionnel, en se concentrant sur les règles fondamentales du jeu et leur affichage textuel. Ce mode offre une expérience minimaliste et directe, idéale pour explorer les bases du fonctionnement du jeu.

Une version en mode graphique : Contrairement au mode console, cette version est pensée comme une application plus complexe et immersive. Elle ne sera pas simplement une extension du mode console mais une véritable application autonome, presque comme un jeu complet en soi. Ce choix est motivé par la complexité inhérente à la gestion graphique, qui inclut des éléments comme l'interface utilisateur, les animations et potentiellement des fonctionnalités interactives avancées.

En résumé, le mode console vise à offrir une version simple et fonctionnelle de la simulation, tandis que le mode graphique sera une application indépendante et plus élaborée, mettant en avant une expérience utilisateur riche et engageante.

Pour la réalisation de notre projet, notamment la version graphique de la simulation du Jeu de la Vie, nous avons adopté une architecture centrée sur l'IHM (Interface Homme-Machine), structurée autour de fichiers distincts qui regroupent Services, Interfaces, et Composants. Voici comment cette organisation se traduit concrètement :
1. Services

Les services constituent la couche logique du système. Ils sont responsables de la gestion des règles et de la logique sous-jacente de la simulation, indépendamment de l'interface graphique. Par exemple :

Service de simulation : calcule les états futurs de la grille selon les règles du Jeu de la Vie.
Service de gestion des fichiers : permet de sauvegarder ou de charger des configurations (grilles initiales, états sauvegardés, etc.).
Ces services sont totalement découplés de l’IHM, ce qui permet de les réutiliser dans d’autres contextes, comme le mode console.

2. Interfaces

Les interfaces agissent comme un contrat entre les différentes parties du système. Elles définissent :

Les interactions utilisateur : quelles actions sont possibles (par exemple, démarrer ou arrêter la simulation, modifier la taille de la grille, changer les paramètres).
Les données échangées : elles spécifient les formats d'entrée et de sortie entre les composants (par exemple, une matrice représentant l'état actuel de la grille).
L’objectif est de garantir que chaque composant respecte une API définie, assurant ainsi une modularité et une flexibilité optimales.

3. Composants

Les composants sont les blocs de construction principaux de l'IHM. Chaque composant est spécialisé et indépendant :

Composant graphique de la grille : affiche la grille et ses mises à jour en fonction des états calculés par le service de simulation.
Composant de contrôle : contient les boutons, sliders, et autres éléments interactifs pour gérer la simulation (démarrer, arrêter, configurer).
Composant de gestion des fichiers : permet à l'utilisateur d'importer/exporter des configurations via des menus dédiés.

Ces composants communiquent entre eux en respectant les interfaces définies, ce qui permet une séparation claire des responsabilités.
Pourquoi cette architecture ?

Modularité : Chaque fichier (ou ensemble de fichiers) a une responsabilité unique, ce qui simplifie le développement, les tests et la maintenance.
Réutilisabilité : Les services peuvent être partagés entre le mode console et le mode graphique, tandis que l’IHM reste spécifique à la version graphique.
Extensibilité : Si nous souhaitons ajouter de nouvelles fonctionnalités, comme une vue 3D ou une compatibilité avec d'autres formats de fichiers, l'architecture actuelle facilite leur intégration.

**Pour Quoi le C++ :**

Nous avons choisi d'utiliser C++ pour le développement de notre projet en raison de ses caractéristiques qui répondent parfaitement aux besoins de notre simulation du Jeu de la Vie. Voici les raisons principales de ce choix :
1. Performances élevées

C++ est reconnu pour offrir des performances exceptionnelles grâce à :

Son contrôle précis de la mémoire : L'utilisation de pointeurs et la gestion manuelle de la mémoire permettent d'optimiser les ressources, un aspect crucial pour une simulation impliquant potentiellement de grandes grilles et de nombreux calculs.
Sa rapidité d'exécution : Les simulations nécessitent des mises à jour fréquentes de l'état de la grille, et C++ garantit que ces calculs sont exécutés rapidement, même avec des configurations complexes.

2. Paradigme orienté objet

L'approche orientée objet de C++ offre des outils puissants pour organiser et structurer le code, ce qui est particulièrement utile dans notre projet :

Encapsulation : Chaque entité (par exemple, la grille, les cellules, les règles du jeu) peut être modélisée comme une classe, ce qui permet de regrouper les données et les comportements associés.
Héritage : Permet d'étendre et de réutiliser des fonctionnalités, ce qui facilite l'ajout de nouveaux comportements ou règles au jeu.
Polymorphisme : Rends le code plus flexible en permettant de définir des comportements spécifiques pour différents composants tout en utilisant une interface commune.

3. Richesse de ses bibliothèques

C++ dispose d’un écosystème de bibliothèques standard et tierces qui simplifient le développement :

STL (Standard Template Library) : offre des structures de données performantes (comme les vecteurs) et des algorithmes qui simplifient la gestion des données du jeu.
Bibliothèques graphiques : Des bibliothèques comme SDL, SFML ou Qt peuvent être intégrées pour la version graphique, exploitant pleinement les capacités de C++ pour gérer des interfaces complexes.

4. Portabilité

Les programmes en C++ peuvent être exécutés sur une large variété de plateformes. Cela garantit que notre projet peut être compilé et exécuté aussi bien sur des systèmes légers que sur des machines plus puissantes.
5. Équilibre entre bas niveau et haut niveau

C++ combine :

Un accès bas niveau : utile pour manipuler directement la mémoire ou optimiser les calculs.
Des abstractions haut niveau : grâce aux classes, templates, et autres fonctionnalités orientées objet, ce qui permet de garder le code clair et modulaire.

**Diagramme UML :**

Mode Graphique : 
![image](https://github.com/user-attachments/assets/10661a52-f29e-49a7-aac9-82f53fe86180)
![image](https://github.com/user-attachments/assets/1833dac3-8a6a-44a4-87e3-ea89b80434f7)
![image](https://github.com/user-attachments/assets/00ba98fb-ef15-4149-9e15-6b40e373917a)

**Exécution du programme - Détail des étapes:**

**Cloner le dépôt:**

Clonez le dépôt Git en utilisant la commande suivante dans votre terminal. Cela téléchargera tout le code source dans un répertoire local : 

    git clone https://github.com/RamiMohamed12/Game-Of-Life-C-.git

Ensuite, accédez au répertoire du projet cloné :

    cd Game-Of-Life-C-

**Pour la Version Console:** 

    cd ConsoleProgram

**Configuration du fichier d'entrée**

Avant de lancer le programme, vous devez configurer le fichier input_file contenant l'état initial de la grille. Voici un exemple pour une grille 5x5 avec ses dimensions incluses :

    5 5
    0 0 0 0 0
    0 0 1 0 0
    0 0 0 1 0
    0 1 1 1 0
    0 0 0 0 0

Ici, 5 représente la hauteur, et 5 la largeur de la grille.

Compiler le programme avec make

    make 

Cela va compiler tous les fichiers sources et générer un exécutable dans le répertoire actuel.

Exécutez le programme après la compilation :

    ./GameOfLife input_file

**Interaction avec le programme**

Choix du mode d'exécution :

Lorsque le programme démarre, il vous demande combien de générations vous souhaitez simuler. Vous avez deux options :

Nombre fixe de générations :
        Entrez un entier correspondant au nombre de générations à simuler.
        Le programme vous demandera la vitesse de simulation (en millisecondes entre chaque génération).
        Une fois configuré, la simulation démarre automatiquement.

Mode manuel :
        Entrez 0 pour activer le mode manuel.
        En mode manuel :
            Appuyez sur ESPACE pour avancer à la génération suivante.
            Appuyez sur ESC pour arrêter la simulation.

Vérifier les fichiers de générations : Vous pouvez vérifier l'état des générations sauvegardées dans le répertoire input_file_out en listant les fichiers :

    ls input_file_out

Vous verrez des fichiers comme :

    generation_1.txt
    generation_2.txt
    generation_3.txt

Chaque fichier contient l'état de la grille pour une génération donnée. Vous pouvez ouvrir ces fichiers pour voir l'évolution de la grille à chaque étape de la simulation.

**Version Console :**

Déplacez-vous dans le répertoire contenant le programme graphique en exécutant la commande suivante :

    cd GraphicProgram

Compile the program : 

    make

Après avoir accédé au répertoire GraphicProgram, vous devez vous déplacer dans le sous-répertoire bin, où se trouve l'exécutable du programme graphique. 

    cd bin

Aprés :

    ./game

**Extensions :**

Notre program du Jeu de la Vie inclut des extensions avancées qui enrichissent l'expérience utilisateur. Voici un aperçu de ces fonctionnalités supplémentaires :
1. Grille toroïdale

    La grille peut être configurée pour fonctionner en mode toroïdal.
    En mode toroïdal, les bords sont connectés, ce qui signifie que les cellules situées à l'extrémité droite "sont voisines" de celles à l'extrémité gauche, et les cellules du bas "sont voisines" de celles du haut.
    Cette option permet de simuler un environnement infini et d'observer des comportements différents des structures de cellules.

2. Interaction utilisateur avec la souris et le clavier

    Souris :
        Cliquez sur les cellules pour les activer ou les désactiver.
        Faites glisser pour dessiner ou effacer des motifs sur la grille.
    Clavier :
        Utilisez des touches prédéfinies pour :
            Démarrer et arrêter la simulation.
            Basculer entre différents modes, comme la grille toroïdale ou standard.
            Réinitialiser la grille ou charger des motifs prédéfinis.
        Les raccourcis clavier simplifient la navigation et permettent un contrôle rapide.

Ces fonctionnalités rendent la simulation plus immersive et interactive, offrant aux utilisateurs une expérience qui dépasse la simple observation et les engage activement dans la création et l'exploration des motifs dynamiques du Jeu de la Vie. Profitez de l'expérience ! 😊

**Conculsion :** 

En conclusion, notre simulation du Jeu de la Vie combine puissance, interactivité et flexibilité grâce à une architecture robuste et une approche graphique avancée. En utilisant C++, nous avons pu maximiser les performances tout en offrant une structure orientée objet pour une gestion claire et évolutive des composants du jeu.

Avec des extensions comme la grille toroïdale et des options d'interaction via la souris et le clavier, nous enrichissons l'expérience utilisateur, permettant à chacun de personnaliser et d'explorer les dynamiques du jeu de manière intuitive. Que vous choisissiez de jouer en mode console simple ou de vous immerger dans la version graphique avancée, ce projet offre une plateforme complète pour expérimenter et découvrir les fascinantes règles de la vie artificielle.

Nous espérons que vous apprécierez le jeu et l'expérience interactive qui l'accompagne. Amusez-vous bien ! 🎮
