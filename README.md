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

Avant de lancer le programme, vous devez vous assurer que le fichier input_file contient l'état initial de la grille dans un format spécifique. 

Ouvrez le fichier input_file et modifiez son contenu pour qu'il ressemble à ceci (c'est un exemple pour une grille 7x5) :

    0 0 1 0 1
    0 1 1 1 1
    0 0 1 0 1
    0 0 1 0 1
    0 1 1 1 1
    0 0 1 0 1

Compiler le programme avec make

    make 

Cela va compiler tous les fichiers sources et générer un exécutable dans le répertoire actuel.

Exécutez le programme après la compilation :

    ./GameOfLife input_file

Interaction avec le programme

Le programme va vous demander les dimensions de la grille (largeur et hauteur), puis il lira l'état initial depuis le fichier input_file, affichera la grille et vous demandera si vous voulez passer à la génération suivante.

Si vous appuyez sur y, il mettra à jour la grille en fonction des règles du jeu de la vie. Vous pourrez continuer à appuyer sur y pour avancer à la prochaine génération ou appuyer sur n pour arrêter.

Après chaque génération, le programme sauvegarde l'état actuel de la grille dans un répertoire nommé input_file_out. Ce répertoire sera créé automatiquement si nécessaire.

Ouvrir le répertoire de sortie : Le programme créera un dossier input_file_out où il enregistrera chaque génération sous la forme de fichiers nommés generation_number.txt (par exemple, generation_1.txt, generation_2.txt, etc.).

Vérifier les fichiers de générations : Vous pouvez vérifier l'état des générations sauvegardées dans le répertoire input_file_out en listant les fichiers :

    ls input_file_out

Vous verrez des fichiers comme :

    generation_1.txt
    generation_2.txt
    generation_3.txt

Chaque fichier contient l'état de la grille pour une génération donnée. Vous pouvez ouvrir ces fichiers pour voir l'évolution de la grille à chaque étape de la simulation.


