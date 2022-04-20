# Projet de compilation IATIC4

Réalisé par Simon PEREIRA et Lucien Hemmi
Fonctions permettant la génération d'automates finis déterministes destinés à reconnaître un langage composé de mots en caractères ascii.  
Elles permettent de générer des automates finis non déterministes capables de reconnaître un langage composé un unique mot d'un unique caractère puis d'enrichir ce langage via des opérations de concaténation, union et fermeture itérative de Kleene (mise à l'étoile).  

## Utilisation

Tout le code se trouve dans le fichier "automato.c"  
Le makefile contient une commande gcc standard et produit l'exécutable "automato"  
La fonction main contient un cas de test qui utilise toutes les fonctions du sujet pour produire un automate déterministe minimal.  
Elle teste cet automate sur deux mots, un faisant partie du langage de l'automate produit et l'autre non.
Le correcteur est libre de modifier les mots de test en n'oubliant pas de modifier également le nombre de caractères correspondant.  
