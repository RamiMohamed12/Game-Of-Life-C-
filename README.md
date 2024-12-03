Le Jeu de la Vie est un automate cellulaire conçu par le mathématicien britannique John Conway en 1970. Il s'agit d'une simulation où une grille est peuplée de cellules qui peuvent être vivantes ou mortes. L'évolution des cellules est déterminée par des règles simples mais puissantes. Règles du jeu :

Une cellule vivante avec moins de deux voisins vivants meurt (sous-population).

Une cellule vivante avec plus de trois voisins vivants meurt (surpopulation).

Une cellule vivante avec deux ou trois voisins vivants reste en vie.

Une cellule morte avec exactement trois voisins vivants devient vivante (naissance).

Le jeu se déroule en une série de générations (ou itérations). Chaque génération est calculée à partir de l'état précédent en appliquant les règles à chaque cellule.


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


