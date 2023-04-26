# tcp-support

TCP client/server app for multi level support, built for a 2nd year networking project at TELECOM Nancy

## Sujet

En partant du couple client-serveur TCP, développer une application simple de tchat
d’assistance multi-niveaux, avec les trois niveaux suivants :

- le niveau 1 correspond à un robot automatique, qui traite les requêtes simples (nous demandons un fonctionnement très basique du robot avec reconnaissance de certains mots clés),
- le niveau 2 envoie la requête à des techniciens pour la résolution d’incidents courants (si le niveau 1 ne trouve pas de solution),
- le niveau 3 envoie la requête à des experts du domaine pour la résolution d’incidents plus complexes (si aucun des techniciens de niveau 2 ne trouvent de solution).

Le nombre de techniciens et d’experts est variable, ceux-ci sont localisés sur des sites
différents et sont contactés en parallèle (pour un même niveau).
