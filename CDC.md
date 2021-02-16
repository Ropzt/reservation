% Cahier des charges

<!-- DANS LE FICHIER DES SUJETS
  logiciel de réservation de place de trains pour une compagnie  nationale (SNCF). Seuls les trajets directs intérieurs sont à envisager. La réservation de places, retire le nombre de place des places disponibles. Prévoir plusieurs catégories de places, donc plusieurs prix. Le logiciel doit permettre d'afficher les horaires, les tarifs et d'effectuer les réservations. 
-->

<!-- MES NOTES LORS DE LA PRESENTATION DES SUJETS
  Pareil (que l'avion), y a des trains différents. Les trains sont différents, n'ont pas le même nb de wagons, le même nb de place (tgv duplex)
  Typologie de trains. 
  1re classe, 2e classe, selon si ça existe sur les trains. Le train fait des escales : ça multiplie les possibilités par rapport à l'avion.
  Peut-être gérer la variation du tarif selon le remplissage. 
-->

<!-- DANS LE MAIL DE JANVIER
  Cahier des charges (CDC) - Analyse : Document WORD, PDF ou OpenOffice de 30 pages + ou - 5 pages (25 pages min - 35 pages max) : 
  vous devez d’abord commencer par l’analyse « métier » c’est a dire expliquer le fonctionnement métier. 
  Par exemple pour une bibliothèque il faut expliquer tout son fonctionnement comme si je ne connaissais rien. 
  Vous présentez votre analyse, ce que vous avez compris, vos choix en termes de modélisation des données et de traitements. 
  Vous expliquez vos difficultés rencontrées. Et seulement à la fin vous faites le lien avec votre programme (succinctement donc c’est pas la majorité des pages!). 
-->

# Présentation générale

## Contexte

La SNCF est une compagnie ferroviaire ayant entre autres activités le transport de voyageurs.
Elle souhaite disposer d'un logiciel de réservation de place : `reservation`.

## Portée

`reservation` est un programme de réservation de place dans les différents modes de transport assurés par la SNCF. 
Cette première version est limitée à la réservation de places :

* de TGV uniquement
* à horizon 1 mois pour la réservation
* à 45 places par rame

Cette première version peut supporter une montée en charge, dans la limite des ressources matériels, notamment RAM, allouées.

Il permet 6 fonctions principales :

* la consultation d'horaires de voyage entre deux gares données à une date donnée ;
* la consultation des tarifs de ces voyages selon :
  * la classe du wagon
  * (le profil du voyageur : carte de réduction)
* la réservation de places
* la gestion des réservations d'un client
* la gestion des places disponibles à bord d'un train
* le calcul des tarifs de voyage


`reservation` ne permet pas :

* la réservation de voyages incluant des correspondances.

## Définitions

**billet**, **billet de train** : titre de transport valant pour une place et pour un voyage.

**classe** : catégorie de prestation offerte à bord d'un wagon. Certains trains possèdent des wagons de différentes classes : première classe (classe 1) et seconde classe (classe 2). D'autres trains possèdent  des wagons d'une seule et même classe.

**date système**, **date/heure système** et **heure système** : informations de date, date/heure et heure de l'équipement terminal sur lequel le programme est utilisé.  
D'un point de vue métier, il s'agit de la date, date/heure et heure courantes, c'est-à-dire la date, date/heure et heure d'utilisation du programme, sachant que le comportement du programme dépend de cette date, date/heure et heure (les voyages proposés ont une date/heure postérieure à la date courante).  
Dans le programme, ces informations sont obtenues via l'équipement terminal sur lequel le programme est exécuté. Elles s'appellent donc date système, date/heure système et heure système malgré la consonance très peu « métier ».

* **date** désigne le jour calendaire d'une année donnée (jour de semaine, jour du mois, mois, année). *Exemple : mardi 16/02/2021*.
* **heure** désigne un horaire (heures, minutes et secondes). *Exemple : 23:59:00*.
* **date/heure** désigne un horaire d'une date. *Exemple : mardi 16/02/2021 23:59:00*.

**gare** : station à partir ou au départ de laquelle il est possible de faire un voyage assuré par la SNCF. 
Il peut s'agir d'une gare de chemin de fer ou d'une gare routière. 
Une ville peut posséder plusieurs gares.

**itinéraire** : 

**place** : siège à bord d'un train pour une séquence d'un voyage, réservable par un voyageur.

**rame** : train au sens « matériel roulant», caractérisée par une composition en siège.

**terminal** : équipement terminal, appareil, sur lequel le programme est utilisé. Dans la version courante du programme, seul l'ordinateur peut être un terminal.

**train** : catégorie mixte de véhicule / service commercial, dont la circulation est assurée par la SNCF et dont l'utilisateur peut réserver une place.
Les trains sont de différents types, que la rame circule sur voie ferrée ou routière (TGV, INOUI, TER, OUIGO, Car).

**trajet** : combinaison unique de gares desservies par un train, et d'horaires d'arrivée et de départ dans chaque gare.

**sequence** : portion de trajet entre deux gares d'arrêt consécutives.

**service** : combinaison d'un trajet et d'un calendrier de circulation (circule ou ne circule pas, pour chaque jour de semaine).

**siège** : unité de composition d'une rame où un voyageur peut s'asseoir.

**voyage** : déplacement en train entre deux gares pour un trajet et une date donnée.

## Vue d'ensemble

La suite du document est divisé en deux partie : une analyse métier de la réservation de places et une description des spécifications du programme.

# Analyse métier

La SNCF assure la circulation de différents types de trains. **compléter**

## Utilisateur

L'utilisateur peut être un agent ou un voyageur.

## Train

La SNCF fait circuler différents types de trains, indiqués par le type TGV, INOUI, OUIGO, TER, Intercités, Car.  
Le « type » désigne des réalités mixtes, à la fois commerciales et techniques.

Le train au sens de « matériel roulant » est désigné par « rame ».

### Rame

Un type de train peut utiliser un ou plusieurs types de rames.
Un train de type Car utilisera probablement une rame standard (un car) avec un nombre de places fixe, tandis qu'un train de type TGV utilise différents types de rames, selon la ligne de chemin de fer (une rame duplex sur Paris-Lyon, une rame simplex sur une autre ligne ou sur un autre service.

Chaque type de rame a une composition propre, mais de façon commune :

* une rame est constituée d'un ou plusieurs wagons
* les wagons d'une rame sont de même classe ou de classes différentes
* un wagon est simplex ou duplex, il possède alors une ou deux salles (salle basse et salle haute)
* une salle est consituée de sièges numérotés
* un siège a un placement et un numéro. Selon le type de rame et la classe de wagon dans laquelle se trouve le siège, le placement peut être : fenêtre, couloir, isolé.

L'utilisation d'un type de rame en fonction du service est assez complexe à comprendre/trouver.
L'affectation des rames aux services semble avoir une base en partie régionale, liée aux commandes de rames lors de l'ouverture d'une nouvelle ligne.
À titre d'exemple, un TGV peut utiliser une rame TGV Sud-Est, TGV Atlantique, TGV Réseau, TGV Duplex, TGV Réseau Duplex, TGV Duplex Dasye, TGV 2N2, TGV PBA, TGV TMST, TGV POS, TGV 2N2, TGV IRIS 320, TGV M, TGV PBKA, Eurostar e320 [Wikipédia : Matériel moteur de la SNCF # Parc électrique](https://fr.wikipedia.org/wiki/Mat%C3%A9riel_moteur_de_la_SNCF#Parc_%C3%A9lectrique).

Dans le cadre de ce prototype, nous utilisons une composition de rame identique et échantillonnée pour tous les services d'un même type de train, tout en permettant un paramètrage différent au cas où ces informations deviennent accessibles.

### Wagon

Un wagon possède les caractéristiques suivantes :

* Un numéro
* Une classe : Unique, 1ère classe ou 2nde classe. La classe influe sur le tarif.
* Un nombre de salle : 1 ou 2.

### Salle

Une salle possède les caractéristiques suivantes :

* Un type de salle : unique, haute, basse

Une salle contient des sièges disposés de façon irrégulière.

### Siège
 
Un siège possède les caractéristiques suivantes :

* Une position : fenêtre, couloir ou isolé.
* Un numéro.

### Une Place

Une place est un siège à bord d'une rame pour une séquence donnée d'un voyage donné (trajet et date de circulation).

Une place est identifiée par le numéro de wagon et le numéro de siège.
Elle est aussi caractérisée par sa salle, son placement, et son état de réservation (libre ou occupée).

Une place est réservable lorsque le voyage auquel elle correspond est ouvert à la vente et que la place est libre.
Lorsque le voyage comporte plusieurs séquences, les places de mêmes identifiants (wagon, siège) de chaque séquence doivent être libres pour que la réservation soit possible.

### Composition des rames

La liste des sièges d'une rame en fonction des wagon, salle et position, n'est pas régulière. 
La hiérarchie « wagon, salle, siège » ne peut pas être conservée.
Un plan de rame est utilisé pour caractériser chacun de ses sièges : une rame est décrite par la liste des sièges et ses caractéristiques (wagon, classe, salle, position, numéro).

#### TGV, INOUI

Une rame de train de type TGV ou INOUI est composée de 8 wagons :

* 7 wagons de passagers (n°1 à 3 et n°5 à 8)
* 1 wagon-bar (n°4)

Parmi les 7 wagons de passagers, 3 sont de classe 1 (n°1 à 3), 4 sont de classe 2 (n°5 à 8).

Une rame de train de type TGV ou INOUI peut être simplex ou duplex.

Une salle de wagon de classe 1 propose 3 placements de sièges :
* fenêtre
* couloir
* place isolée

Une salle de wagon de classe 2 propose 3 placements de sièges :
* fenêtre
* couloir

##### Rame simplex

Une rame simplex est composée de wagons à 1 niveau (une salle).

Chaque génération de rame possède son propre plan de placement :

* [Placement des rames TGV Sud-Est Rénové 1](./documentation/rame_TGV_Sud-Est_renove_1_simplex.pdf) (351 places)

##### Rame duplex

Une rame duplex est composée de wagons à 2 niveaux, et possède ainsi une salle basse et une salle haute. 

Une salle de wagon de classe 1 propose 3 placements de sièges :
* fenêtre
* couloir
* place isolé

Une salle de wagon de classe 2 propose 3 placements de sièges :
* fenêtre
* couloir

Chaque génération de rame possède son propre plan de placement :

* [Placement des rames TGV DUPLEX GÉNÉRATION 1 (510 places)](./documentation/rame_TGV_duplex_1re_generation.pdf) (510 places)
* [Placement des rames TGV DUPLEX GÉNÉRATION 3 «EURO DUPLEX »](./documentation/rame_TGV_duplex_3e_generation_Euroduplex.pdf) (509 places)

#### Autres Modes de Transport Gérés par la SNCF

La SNCF gère plusieurs autres modes de transport : 

* Des trains :
  * OUIGO
  * TER
  * Intercités
* Des Cars :
  * Car
  * Navette
* Des tramways

Ces modes de transport n'ont pas les mêmes caractéristiques que les TGV.
La gestion de ces modes de transport a un impact sur les caractéristiques suivantes :
* La classe : possiblement une classe unique au lieu de 2.
* Le nombre de salles : possiblement réduit à une.
* Le nombre de sièges.
* La position des sièges : possiblement moins de positions différentes.

Le programme `reservation` ne permet pas de gérer ces modes de transport. *peut-être replacer à la fin, on n'est pas censé parler de la solution dans l'analyse*

## Circulation des trains

Les trains circulent en suivant des trajets, découpés en séquences caractérisées par une gare de départ et une gare d'arrivée consécutives sur le trajet, selon un calendrier de service.

### Trajet

Un train circule en suivant un trajet. Un trajet est une combinaison unique de gares desservies par un train, et d'horaires d'arrivée et de départ dans chaque gare.

Un trajet peut être décrit comme suit :

> Un trajet effectuant la liaison depuis Bordeaux jusqu'à Nice et s'arrêtant à Marseille, dans cet ordre, partant de Bordeaux à 14h03 et arrivant à Nice à 18h47.

### Service / Calendrier

Un trajet est associé à un service qui définit le calendrier hebdomadaire de circulation, et des dates de début et fin de validité.

Un service peut être décrit comme suit :

> Le service concerne tous les mardis et tous les jeudis du 27 Novembre 2020 au 3 Mars 2021.

Un trajet est donc effectué, toujours suivant les mêmes horaires de départ et d'arrivée, certain(s) jour(s) de la semaine, d'une date de début à une date de fin de validité.

Le trajet peut alors être décrit comme suit :

> Un trajet effectuant la liaison depuis Bordeaux jusqu'à Nice et s'arrêtant à Marseille, dans cet ordre, partant de Bordeaux à 14h03 et arrivant à Nice à 18h47, tous les mardis et tous les jeudis du 27 Novembre 2020 au 3 Mars 2021.

### Séquence

Une séquence est un élément constitutif d'un trajet, ce dernier étant composé d'une suite de séquences continues et cohérentes.

Une séquence est caractérisée par une gare de départ, une heure de départ, une gare d'arrivée et une heure d'arrivée. 
Pour un trajet, il y a donc une séquence de moins qu'il n'y a de gares.

Un numéro de séquence est égale au numéro de "stop" de départ de cette séquence.

Une séquence peut donc être décrite comme suit :

> La séquence numéro 3 commence à la troisième gare du trajet et finit à la quatrième gare du trajet.

### Gare

Une gare est une station où s'arrête un train. Elle est identifiée par un identifiant ou un nom.

Une gare possède des zones d'embarquement, identifiées par un identifiant, et le nom et l'identifiant de la gare à laquelle elle appartient.

Une gare peut être incluse dans plusieurs trajets.
Dans un trajet, une gare représente un arrêt, identifié par un numéro d'arrêt. Une gare peut alors être identifiée par le numéro d'arrêt dans le trajet.

Une gare est incluse dans deux séquences consécutives d'un même trajet : elle est la gare de départ d'une séquence X et la gare d'arrivée de la séquence X-1.

Une gare peut être décrite comme suit :

> son nom : Gare de Lyon-Part-Dieu
>
> son identifiant : StopArea:OCE87723197
>
> l'identifiant d'un de ses points d'arrêt : StopPoint:OCETGV INOUI-87723197
>
> son numéro d'arrêt dans un trajet donné : n°7 du trajet OCESN009877F58058-1323290491
>
> la gare d'arrivée de la séquence 6 du trajet OCESN009877F58058-1323290491
>
> la gare de départ de la séquence 7 du trajet OCESN009877F58058-1323290491

## Ouverture des ventes

Les ventes de billets sont ouvertes avec une antécédence, par rapport au voyage, différente selon le type de train :

* TGV, INOUI & Intercités : 4 mois, sauf pour les vacances d'été, de Noël et d'hiver et de printemps. L'ouverture des ventes des voyages des vacances est plus tardive que les 4 mois habituels.
* TGV Bruxelles, Italo, TGV Luxembourg-Paris, TGV Fribourg-Paris : 4 mois
* Trajets RENFE/SNCF, ALLEO France-Allemagne, TGV Lyria (entre Lille et Genève) : 4 mois
* TGV France-Italie : 4 mois
* TER : en fonction des régions, entre 2 et 5 mois
* OUIGO : entre 2 et 9 mois
* THALYS (vers la Belgique, les Pays-Bas, l'Allemagne) : 4 mois
* Eurostar (vers l'Angleterre) : 6 mois pour Paris, Lille et Calais vers London, Ashford, Ebbsfleet, 150 à 280 jours pour Lyon, Marseille et Avignon, vers London, Ashford, Ebbsfleet

Lorsqu'un voyage est ouvert à la vente, ses places doivent être réservables.

## Tarifs

Le tarif dépend de la tarification et des programmes de réduction.

### Tarification

La tarification dépend du type de train.

#### TGV

Trois tarifs sont proposés pour les voyages en TGV : prix d'appel 2nde, plein tarif loisir 2nde, 1re classe.

* Le prix d'appel 2nde est un tarif proposé à l'ouverture des ventes. Il offre un niveau de prestation réduit concernant les conditions d'échange et d'annulation par rapport au plein tarif loisir 2nde. Il évolue en fonction du remplissage du train.
* Le plein tarif loisir 2nde est un tarif proposé pendant toute la période de réservation, pour un voyage en seconde classe. 
* Le plein 1re classe est un tarif proposé pendant toute la période de réservation, pour un voyage en première classe. 

Chaque tarif est défini pour une gare de départ et une gare d'arrivée.

### Programmes de réduction

Deux types de programmes de réduction sont proposés.

* Des cartes de réduction.
* Des abonnements.

## Réservation

Le processus de réservation d'une place de train suit généralement les étapes suivantes :

1. Recherche
  * choix de la gare/ville de départ
  * choix de la gare/ville d'arrivée
  * choix de la date
  * choix des options de voyage : classe de wagon, programme de réduction.
2. Consultation des résultats
3. Modification des critères de recherche
4. Choix et réservation d'un voyage parmi les résultats, avec renseignement des informations utilisateurs

### Recherche

Un utilisateur doit pouvoir consulter les horaires de voyages en indiquant :

* la ville de départ
* la ville d'arrivée
* la date du voyage

#### Choix de la gare/ville de départ

Certaines villes possèdent plusieurs gares.
C'est le cas :

* des grandes villes et des villes possédant une gare de chemin de fer historique et une gare TGV située en dehors de la ville sur une ligne à grande vitesse.
* des villes possédant une gare de chemin de fer et une gare routière

Le voyageur peut souhaiter réserver une place pour un voyage au départ ou à l'arrivée d'une ville, indépendamment de la gare de départ ou d'arrivée dans cette ville.
Les résultats affichés doivent inclure les différentes gares d'une même ville.

Si aucun train ne circule au départ de cette ville, le processus de recherche doit s'interrompre.
Il peut éventuellement proposer de saisir une autre ville de départ.

#### Choix de la gare/ville d'arrivée

La problématique gare/ville est identique pour la gare/ville d'arrivée.

Si aucun train ne circule entre la ville de départ et la ville d'arrivée, le processus de recherche doit s'interrompre.
Il peut éventuellement proposer de saisir une autre gare/ville d'arrivée en conservant la gare/ville de départ précédemment choisie. 

#### Choix de la date

La date de voyage ne peut pas être antérieure à la date système.

Si la date saisie est antérieure, le processus de recherche doit s'interrompre.
Il peut éventuellement proposer la saisie d'une autre date en conservant la gare/ville de départ et la gare/ville d'arrivée précédemment choisies.

#### Gestion de la date/heure

Concernant la date/heure, le cas de la réservation de voyage et le cas de la consultation d'horaires sont différents. 

Dans le cas d'une réservation de voyage, le programme ne doit pas proposer de résultats de voyages partant à une date/heure antérieure à la date/heure système.

Dans le cas d'une consultation d'horaire, le programme ne doit pas proposer de résultats de voyages arrivant à une date/heure antérieure à la date/heure système.

### Consultation des résultats

Les résultats de la recherche doivent être affichés et des actions doivent être proposées à l'utilisateur.

#### Affichage des résultats

L'affichage des résultats doit faire apparaitre suffisament d'informations pour permettre à l'utilisateur de choisir entre continuer le processus de réservation ou y apporter des modifications, sans qu'il y ait de sur-abondance d'informations.

Il est nécessaire d'afficher :

* La gare de départ
* La gare d'arrivée
* L'heure de départ
* L'heure d'arrivée
* Le type de train
* Le prix ou l'indisponibilité en Première Classe
* Le prix ou l'indisponibilité en Seconde Classe

Les résultats doivent être affichés par heure de départ croissant.
Un autre critère de tri peut être proposé à l'utilsateur.

#### Proposition d'actions

Des résultats proposés doivent découler des actions.

Les actions possibles pour l'utilisateur à la vue des résultats sont :

* Réserver un des trajets proposés
* Modifier sa recherche
* Quitter le processus de recherche

##### Réserver

Si l'utilisateur est satisfait par un des résultats proposés, il doit pouvoir sélectionner l'un des trajets pour lancer le processus de réservation.

##### Modification des critères de recherche

Un utilisateur doit pouvoir modifier les critères suivants de sa recherche :

* la ville de départ
* la ville d'arrivée
* la date du voyage

La modification de la ville départ ou de la ville d'arrivée peut être assimilée au fait de quitter le processus de recherche et d'en recommencer un nouveau.

Nous ne traiterons donc que de la modification du paramètre Date.
Ce dernier peut être modifié de deux façons :

* Par incrémentation : le jour suivant.
* Par décrémentation : le jour précédent.

### Réservation d'un voyage parmi les résultats

Après le choix d'un trajet, plusieurs options sont proposées à l'utilisateur concernant son voyage.

La disponibilité de la combinaison de ces préférences doit être vérifiée.

Lorsque les choix sont fixés, des informations concernant les voyageurs sont demandées à l'utilisateur.

La réservation passe alors en paiement.

Lorsque le paiement est effectué, les billets sont générés.

#### Choix Utilisateur

Après le choix d'un trajet, plusieurs options doivent être proposées à l'utilisateur concernant le choix des sièges.
L'utilisateur doit être invité à renseigner ses préférences concernant :

* la classe du wagon
* le nombre de places : dans la limite du nombre de places disponibles.
* la salle : si le train possède plus d'une salle, proposer la salle basse ou la salle haute.
* la position de son siège : 
  * fenêtre ou couloir s'il voyage en seconde classe
  * choix du siège parmi les sièges disponibles s'il voyage en première classe

#### Verification

La disponibilité de la combinaison de ces préférences doit être vérifiée pour poursuivre le processus de réservation. 
Par exemple, il peut y avoir des sièges restant en première classe, mais aucun côté fenêtre.
Les choix de l'utilisateur étant fait les uns après les autres, chaque choix influe sur la faisabilité du choix suivant.
La vérification doit être faite à chaque étape de choix.

Si la combinaison est disponible, le processus doit continuer. 
Si elle n'est pas disponible, l'utilisateur doit être notifié. 
Le choix doit lui être donné de poursuivre la réservation avec une combinaison de critères différente de ses préférences ou de modifier son choix de trajet.

S'il choisit de poursuivre, le processus de reservation passe au calcul du montant total, puis au paiement, puis à la création des billets et l'inscription de la réservation dans la base de données.

##### Classe du wagon

Les options suivantes dépendent de la classe choisie.

##### Nombre de places

Il est aussi possible que l'utilisateur souhaite réserver plusieurs sièges.
Il faut demander à l'utilisateur de modifier son choix tant qu'il n'a pas renseigné un nombre de sièges désirés inférieur au nombre de sièges disponibles pour une classe donnée.

##### Seconde classe

Lorsque l'utilisateur précise qu'il veut un siège ou plusieurs sièges en seconde classe et que sa demande est faisable, il ne doit être laissé le choix à l'utilisateur de préciser sa demande que si plusieurs solutions sont possibles à chaque fois. Sinon, le choix des caractéristiques de salle et de position doit lui être imposé. 

S'il ne reste des places disponibles que dans une salle, il faut prévenir l'utilisateur.

S'il reste des places disponibles dans les deux salles, il faut proposer à l'utilisateur de choisir.

S'il ne reste des places disponibles que pour une seule position de siège dans cette salle, il faut prévenir l'utilisateur.

S'il reste des places disponibles dans plusieurs positions de sièges dans cette salle, il faut proposer à l'utilisateur de choisir.

Enfin, il faut demander à l'utilisateur si ses choix, ou ces assignations, lui conviennent, ou s'il veut revenir au choix du trajet.

###### Première Classe

Lorsque l'utilisateur précise qu'il veut un siège ou plusieurs sièges en première classe et que sa demande est faisable, il faut lui présenter la liste des places disponibles en première classe, avec les caractéristiques de chaque place.

L'utilisateur renseigne alors son ou ses choix. 

Une place ne pouvant être choisie deux fois, un choix n'est plus affiché par la suite.

Enfin, il faut demander à l'utilisateur si ses choix lui conviennent, ou s'il veut revenir au choix du trajet.

#### Paiement

Lorsque le processus de réservation continue, soit que la combinaison des préférences était disponible, soit que l'utilisateur a choisi de poursuivre la réservation avec une combinaison proposée, le processus passe à une étape de calcul du montant total.
Le montant total de la réservation doit être présentée à l'utilisateur.

Le processus de paiement doit demander des informations de paiement à l'utilisateur :

* Le numéro de carte bancaire
* La date d'expiration de la carte bancaire
* Le code CVC de la carte bancaire

Le succès de la procédure de paiement doit être notifiée à l'utilisateur.

#### Export de la réservation

Une fois le paiement effectué avec succès, la réservation est considérée faite.
Les informations de réservation doivent être stockées dans la base de données. 

Les places sélectionnées doivent être mentionnées comme étant désormais non disponibles sur l'intégralité des séquences sur lesquelles l'utilisateur souhaite voyager.

#### Création du billet

Pour terminer le processus de réservation, un billet récupitulant les informations sur sa réservation doit être donné à l'utilisateur, et une copie de ce billet doit être stockée sur la base de données.

# Spécifications

## Fonctions

### Fonctionnalités Coeur de Métier

#### Fonctionnalités concernant le processus de recherche

* Procédure Mère lançant les procédures et fonctions suivantes :
  * Recherche de trajets contenant la gare désirée
  * Filtrage des trajets ne contenant pas les bonnes gares de départ et d'arrivée ou dans le mauvais sens
  * Filtrage des trajets ne circulant pas le jour de la semaine découlant de la date de voyage désirée
  * Quantification du nombre de places disponibles pour les trajets restants, en Première et Seconde Classe
  * Tri des trajets en fonctions de l'heure de départ
  * Procédure Mère de Réservation

#### Fonctionnalités concernant le processus de réservation

* Procédure Mère lançant les procédures et fonctions suivantes :
  * Écriture des places réservées dans la structure contenant les places
  * Procédure de paiement
  * Écriture des billets créés dans la structure contenant le registre des billets

### Fonctionnalités Support

#### Les horaires

* Récupération des horaires depuis un dossier vers une structure dédiée

#### Les dates ouvertes à la vente

* Création d'un tableau contenant les dates pour lesquelles la vente est ouverte

Au démarrage, le système doit : 

* charger les données de réservation (place)
* créer des places réservables pour les trains qui s'ouvrent à la vente :
  * regarder la date système
  * lister les 
  * pour chaque service, regarder son type de train
  * calculer la date la plus tardive (date limite) à laquelle ses places sont ouvertes à la réservation
  * si le type est TGV INOUI ou Intercités, si la date tombe dans une période de vacances scolaires, l'ignorer
  * sinon, si elle n'existe pas, créer une rame pour chaque date entre la date système et la date limite, avec les caractéristiques correspondant au type de train
* supprimer les rames des trains qui sont arrivés
  * regarder la date/heure système
  * parser les dates
  * si la date est antérieure à la date système, supprimer ses rames

Le programme ne gère pas les vacances.
Le programme ne gère qu'un type de train (4 mois, et 1 seul pour la démo)

#### Les Places

##### Fonctionnalités en Amont

* Récupération des places depuis une architecture de dossiers vers une structure dédiée
* Tri de la structure contenant les places afin de supprimer celles concernant des trajets passés
* Incrémentation de la structure contenant les places afin d'ajouter celles concernant les dates et trajets désormais ouverts à la vente

##### Fonctionnalités en Aval

* Création et/ou écriture depuis la structure contenant les places vers une architecture de dossiers

#### Les Billets

* Récupération du registre des billets depuis un dossier vers une structure dédiée
* Écriture du registre des billets depuis une structure dédiée vers un dossier

### Fonctionnalités Outils

#### Fonctionnalités concernant les dates

* Récupération du la date du système
* Incrémentation/Décrémentation de la date d'un jour (Jour Suivant/Précédent)
* Récupération du jour de la semaine depuis une date
* Séparation d'une date en jour, mois, année

#### Fonctionnalités concernant les conversions et interpretations

* Conversion de chaque caractère d'une chaine de caractères en majuscule
* Conversion de chaque `-` en `space` d'une chaine de caractères
* Conversion de chaque motif dans une chaine de caractères en un autre motif
* Suppression des accents
* Interpretation du jour de la semaine, de son code vers son nom
* Interpretation de la position du siège, de son code vers son nom

#### Fonctionnalités concernant les saisies

* Saisie textuelle sécurisée
* Saisie numérique sécurisée
* Lecture de choix utilisateur sécurisée
* Saisie de date sécurisée

#### Fonctionnalités concernant les interface avec le système

* Vidage du buffer
* Suppression d'un répertoire
* Récupération du contenu d'un répertoire
* Récupération des informations sur un objet




Pour plus tard :
* afficher les horaires
* afficher les tarifs
* calcul du tarif
  * en fonction du trajet
  * en fonction de la catégorie de place
  * en fonction d'un programme de réduction
  * en fonction du remplissage



## Base de données

### Jeux de données

Les ensembles de données utilisés sont ceux mis à disposition par la SNCF à https://ressources.data.sncf.com/explore/

* Horaires
  * Horaires des TGV : https://ressources.data.sncf.com/explore/dataset/horaires-des-train-voyages-tgvinouiouigo/table/
  * Horaires des lignes TER : https://ressources.data.sncf.com/explore/dataset/sncf-ter-gtfs/table/
  * Horaires des lignes Transilien : https://ressources.data.sncf.com/explore/dataset/sncf-transilien-gtfs/table/
  * Horaires des lignes Intercités : https://ressources.data.sncf.com/explore/dataset/sncf-intercites-gtfs/table/
  * Horaires des Tram-Train TER Pays de la Loire : https://ressources.data.sncf.com/explore/dataset/sncf-tram-train-ter-pdl-gtfs/table/
* Tarifs
  * Tarifs TGV par origine-destination : https://data.sncf.com/explore/dataset/tarifs-tgv-par-od/table/
  * Tarifs TER par origine-destination : https://data.sncf.com/explore/dataset/tarifs-ter-par-od/table/?disjunctive.region&sort=region
  * Tarifs Intercités 100% ECO : https://data.sncf.com/explore/dataset/tarifs-intercites-100-eco/table/?sort=origine
  * Tarifs Intercités de jour : https://data.sncf.com/explore/dataset/tarifs-intercites-de-jour/table/?sort=origine

### Architecture des dossiers de données

#### Horaires

Les données relatives aux horaires sont situées dans le dossier `data`, puis le sous dossier `horaire`.
Le chemin relatif est donc : `./data/horaire/`.

#### Tarifs

Les données relatives aux tarifs sont situées dans le dossier `data`, puis le sous dossier `tarif`.
Le chemin relatif est donc : `./data/tarif/`.

#### Billets

Les données relatives aux billets sont situées dans le dossier`data`, puis le sous dossier `billet`.
Le chemin relatif est donc : `./data/billet/`.

#### Places

Les données relatives aux places sont situées dans le dossier `data`, puis le sous dossier `place`, puis un des sous dossiers de trajets ayant comme nom l'identifiant du trajet, puis un des sous dossiers de date ayant comme nom une date au format AAAAMMJJ, puis un des sous dossiers de séquence ayant comme nom le numéro de la séquence.
Le chemin relatif est donc : ```./Data/Places/<idtrajet>/<date>/<sequence>/```.

### Description des données

#### Horaires

Les ensembles de données sont disponibles au format [GTFS](https://fr.wikipedia.org/wiki/General_Transit_Feed_Specification) (*General Transit Feed Specification*), format de ficher standardisé pour les horaires de transports en commun.

##### Description du format GTFS

Parmi les fichiers obligatoires et facultatifs du standard GTFS, chaque ensemble de données Horaires fourni par la SNCF est constitué de 8 fichiers :

* `agency.txt` : Agences de transports en commun ayant un service représenté dans cet ensemble de données.
* `stops.txt` : Arrêts où les voyageurs peuvent monter et descendre. Définit également les stations et leurs entrées.
* `routes.txt` : Itinéraires en transports en commun. Un itinéraire est un ensemble de trajets présentés aux voyageurs  comme relevant du même service.
* `trips.txt` : Trajets pour chaque itinéraire. Un trajet est une série d'au moins deux arrêts desservis à des horaires précis.
* `stop_times.txt` : Heures d'arrivée et de départ d'un train depuis des arrêts spécifiques, pour chaque trajet.
* `calendar.txt` : Dates de service indiquées à l'aide d'un horaire hebdomadaire comportant des dates de départ et d'arrivée.
* `calendar_dates.txt` : Exceptions pour les services définis dans le fichier `calendar.txt`.
* `transfers.txt` : Règles de liaison aux pôles de correspondance entre des itinéraires.


Le schéma logique d'un ensemble de données Horaires est le suivant :

![Schéma logique des ensembles de données Horaires fournis par la SNCF](diagrammes/bdd_sncf_horaires.png "Schéma logique des ensembles de données Horaires fournis par la SNCF")

Les champs de couleur rouge sont ceux traités par le programme.

###### Fichier `agency.txt`

Le fichier `agency.txt` décrit les agences de transports en commun ayant un service représenté dans cet ensemble de données. Il s'agit de la SNCF.
Il comporte les champs :

* `agency_id` : indique la marque du réseau de transports en commun (souvent identique au nom de l'agence)
* `agency_name` : nom complet de l'agence de transports en commun.
* `agency_url` : URL de l'agence de transports en commun.
* `agency_timezone` : fuseau horaire de la zone où se trouve l'agence de transports en commun.
* `agency_lang` : langue principale utilisée par cette agence de transports en commun.

###### Fichier `stops.txt`

Le fichier `stops.txt` décrit les arrêts où les voyageurs peuvent monter et descendre, c'est à dire les gares.
Il est lié à la table `stop_times.txt` par le champ `stop_id`.
Il comporte les champs :

* `stop_id` : identifie un arrêt, une station ou une entrée de station.
  Le terme "entrée de station" désigne à la fois les entrées et les sorties de station. Les arrêts, les stations et les entrées de station sont collectivement appelés "emplacements". Le même arrêt peut être desservi par plusieurs itinéraires.
* `stop_name` : nom de l'emplacement.
* `stop_desc` : description de l'emplacement. Ce champ n'est pas toujours renseigné.
* `stop_lat` : latitude de l'emplacement.
* `stop_lon` : longitude de l'emplacement.
* `zone_id` : définit la zone tarifaire d'un arrêt. Ce champ n'est pas toujours renseigné.
* `stop_url` : URL d'une page Web qui décrit l'emplacement. Ce champ n'est pas toujours renseigné.
* `location_type` : type d'emplacement :
  * 0 ou vide : arrêt ou quai (lieu où les usagers montent dans un véhicule de transport en commun ou en descendent). Le terme "quai" est utilisé lorsque cette valeur est définie au sein d'un champ `parent_station`.
  * 1 : station (zone ou structure physique comprenant un ou plusieurs quais)
  * 2 : entrée ou sortie (lieu où les usagers peuvent entrer dans une station depuis la rue ou en sortir). Si une entrée/sortie appartient à plusieurs stations, tous les chemins correspondants sont indiqués, et le fournisseur de données doit désigner une station en tant que station principale (parente).
  * 3 : intersection générique (un emplacement dans une station qui ne correspond à aucune autre valeur `location_type`).
  * 4 : Zone d'embarquement (un emplacement spécifique sur un quai où les usagers peuvent monter à bord d'un véhicule ou en descendre)
* `parent_station` : spécifie la hiérarchie entre les différents emplacements définis dans le fichier `stops.txt`. Contient l'ID de l'emplacement parent, comme suit :
  * Arrêt/quai (`location_type`=0) : le champ `parent_station` contient l'ID d'une station.
  * Station (`location_type`=1) : ce champ doit être vide.
  * Entrée/sortie (`location_type`=2) ou intersection générique (`location_type`=3) : le champ `parent_station` contient l'ID d'une station (`location_type`=1).
  * zone d'embarquement (`location_type`=4) : le champ `parent_station` contient l'ID d'un quai.

###### Fichier `routes.txt`

Le fichier `routes.txt` décrit les itinéraires en transports en commun. Un itinéraire est un ensemble de trajets présentés aux voyageurs comme relevant du même service.
Il est liée à `agency.txt` par le champ `agency_id`.
Il comporte les champs :

* `route_id` : définit un itinéraire.
* `agency_id` : agence pour l'itinéraire spécifié. Fait référence à `agency.agency_id`.
* `route_short_name` : version courte du nom d'un itinéraire. Il s'agit généralement d'un identifiant court, abstrait, comme "32", "100X" ou "vert", que les usagers utilisent pour identifier un itinéraire, sans donner d'indications sur les lieux desservis
* `route_long_name` : le nom complet d'un itinéraire. Ce nom est généralement plus descriptif que la version courte indiquée dans le champ `route_short_name`. Il inclut souvent la destination ou le terminus de l'itinéraire. 
* `route_desc` : description d'un itinéraire. Les informations fournies doivent être utiles et de qualité, elles ne répètent pas le nom de l'itinéraire. Ce champ n'est pas renseigné.
* `route_type` : décrit le moyen de transport utilisé pour un itinéraire. Les options suivantes sont acceptées :
    * `0` : tramway ou métro léger. Tout système de métro léger ou circulant sur la chaussée dans une zone métropolitaine.
    * `1` : métro. Tout système ferroviaire souterrain circulant au sein d'une zone métropolitaine.
    * `2` : train. Utilisé pour les trajets interurbains ou longue distance.
    * `3` : bus. Utilisé pour les lignes de bus courte et longue distance.
    * `4` : ferry. Utilisé pour le service de bateaux courte et longue distance.
    * `5` : tramway à traction par câble. Utilisé pour les systèmes de tramways au niveau de la chaussée dans lesquels le câble passe sous le véhicule, comme c'est le cas à San Francisco.
    * `6` : téléphérique. Service de transport par câble où les cabines, voitures, télécabines ou sièges sont suspendus à l'aide d'un ou de plusieurs câbles.
    * `7` : funiculaire. Tout système ferroviaire conçu pour les pentes raides.
    * `11` : trolleybus. Autobus électrique alimenté par des lignes aériennes de contact.
    * `12` : monorail. Service de chemin de fer roulant sur une voie constituée d'un rail ou d'une poutre unique.
* `route_url` : URL d'une page Web pour un itinéraire spécifique.
* `route_color` : couleur de l'itinéraire correspondant à celle utilisée dans les supports destinés au public.
* `route_text_color` : couleur lisible pour le texte à afficher sur la couleur d'arrière-plan `route_color`.

###### Fichier `trips.txt`

Le fichier `trips.txt` définit les trajets pour chaque itinéraire.
Il est lié à `routes.txt` par le champ `route_id`.
Il comporte les champs : 

* `route_id` : définit un itinéraire. Fait référence à `routes.route_id`.
* `service_id` : définit les dates auxquelles le service est disponible pour un ou plusieurs itinéraires. Fait référence à `calendar.service_id` ou à `calendar_dates.service_id`
* `trip_id` : définit un trajet.
* `trip_headsign` : texte qui apparaît sur la signalétique indiquant aux voyageurs la destination du trajet (le numéro de train tel qu'affiché en gare et sur les billets). Ce champ permet de différencier les modèles de service sur un même itinéraire.
* `direction_id` : indique la direction du trajet. Ce champ n'est pas utilisé pour les itinéraires, mais il permet de distinguer les trajets en fonction de leur direction lors de la publication des horaires. Les options suivantes sont acceptées :
    * `0` : trajet dans un sens (trajet aller, par exemple).
    * `1` : trajet dans le sens opposé (trajet retour, par exemple)
* `block_id` : identifie le bloc auquel appartient le trajet. Un bloc comprend un seul trajet ou de nombreux trajets séquentiels effectués par le même véhicule. Ce champ n'est pas renseigné.
* `shape_id` : définit une forme géospatiale décrivant le parcours du véhicule lors d'un trajet. Ce champ n'est pas renseigné.

###### Fichier `stop_times.txt`

Le fichier `stop_times.txt` définit les heures d'arrivée et de départ d'un train depuis des arrêts spécifiques, pour chaque trajet.
Il est lié à `stops.txt` par le champ `stop_id`.
Il comporte les champs : 

* `trip_id` : définit un trajet. Fait référence à `trips.trip_id`.
* `arrival_time` : heure d'arrivée à un arrêt donné pour un trajet spécifique inclus dans un itinéraire. Si les heures d'arrivée et de départ sont identiques pour un arrêt spécifique, la même valeur est saisie pour les champs `arrival_time` et `departure_time`. Pour les heures après minuit de la journée de service, la valeur saisie est une supérieure à 24:00:00 au format HH:MM:SS dans l'heure locale du jour où commence le trajet.
* `departure_time` : heure de départ depuis un arrêt donné pour un trajet spécifique inclus dans un itinéraire. Si les heures d'arrivée et de départ sont identiques pour un arrêt spécifique, la même valeur est saisie pour les champs `arrival_time` et `departure_time`. Pour les heures après minuit de la journée de service, la valeur saisie est supérieure à 24:00:00 au format HH:MM:SS de l'heure locale le jour où commence le trajet. 
* `stop_id` : identifie l'arrêt desservi. Fait référence à `stops.stop_id`.
* `stop_sequence` : ordre des arrêts desservis lors d'un trajet particulier. Les valeurs augmentent à mesure du trajet, mais ne sont pas nécessairement consécutives.
* `stop_headsign` : texte qui apparaît sur la signalétique indiquant aux voyageurs la destination du trajet.
* `pickup_type` : indique les possibilités de montée à bord. Les options suivantes sont acceptées :
    * `0` ou vide : les usagers peuvent monter à bord aux horaires standards.
    * `1` : les usagers ne peuvent pas monter à bord.
    * `2` : les usagers doivent téléphoner à l'agence pour pouvoir monter à bord.
    * `3` : les usagers doivent contacter le conducteur pour pouvoir monter.
* `drop_off_type` : indique les possibilités de descente du véhicule. Les options suivantes sont acceptées :
    * `0` ou vide : les usagers peuvent monter à bord aux horaires standards.
    * `1` : les usagers ne peuvent pas monter à bord.
    * `2` : les usagers doivent téléphoner à l'agence pour pouvoir monter à bord.
    * `3` : les usagers doivent contacter le conducteur pour pouvoir monter.
* `shape_dist_traveled` : indique la distance réelle parcourue le long du tracé donné entre le premier arrêt et l'arrêt spécifié dans cet enregistrement.

###### Fichier `calendar.txt` 

Le fichier `calendar.txt` définit les dates de service indiquées à l'aide d'un horaire hebdomadaire comportant des dates de départ et d'arrivée.
Il est liée au fichier `trips.txt` par le champ `service_id`.
Il comporte les champs :

* `service_id` : définit de façon unique les dates auxquelles le service est disponible pour un ou plusieurs itinéraires. Fait référence à `trips.service_id`.
* `monday` : indique si le service est proposé tous les lundis de la plage de dates spécifiée par les champs `start_date` et `end_date`. Les options suivantes sont acceptées :
    * `1` : le service est disponible tous les lundis de la plage de dates.
    * `0` : le service n'est pas disponible les lundis de la plage de dates.
* `tuesday` : fonctionne comme le champ `monday`, mais pour les mardis
* `wednesday` : fonctionne comme le champ `monday`, mais pour les mercredis
* `thursday` : fonctionne comme le champ `monday`, mais pour les jeudis
* `friday` : fonctionne comme le champ `monday`, mais pour les vendredis
* `saturday` : fonctionne comme le champ `monday`, mais pour les samedis
* `sunday` : fonctionne comme le champ `monday`, mais pour les dimanches
* `start_date` : date de début de validité du service
* `end_date` : date de fin de validité du service

###### Fichier `calendar_dates.txt`

Le fichier `calendar_dates.txt` définit les exceptions pour les services définis dans le fichier `calendar.txt`.
Il est liée au fichier `calendar.txt` par le champ `service_id`.
Il comporte les champs : 

* `service_id` : définit les dates auxquelles le service est exceptionnellement disponible ou indisponible pour un ou plusieurs itinéraires. Fait référence à `calendar.service_id`.
* `date` : date à laquelle le service proposé est différent du service standard.
* `exception_type` : indique si le service est disponible à la date spécifiée dans le champ date. Les options suivantes sont acceptées :
    * `1` : le service a été ajouté pour la date spécifiée.
    * `2` : le service a été supprimé pour la date spécifiée.

Nous ne nous servont pas de cette table.

###### Fichier `transferts.txt`

Le fichier `transferts.txt` spécifie des règles et des valeurs de remplacement supplémentaires pour les correspondances sélectionnées.
Il comporte les champs :

* `from_stop_id` : identifie l'arrêt ou la station de départ pour une liaison entre deux itinéraires. Si ce champ fait référence à une station, la règle de correspondance s'applique à tous ses arrêts enfants. Fait référence à `stops.stop_id`.
* `to_stop_id` : identifie l'arrêt ou la station de départ pour une liaison entre deux itinéraires. Si ce champ fait référence à une station, la règle de correspondance s'applique à tous ses arrêts enfants. Fait référence à `stops.stop_id`.
* `transfer_type` : Indique le type de correspondance pour la paire (from_stop_id, to_stop_id) spécifiée. Les options suivantes sont acceptées :
    * `0` ou vide : point de correspondance recommandé entre deux itinéraires.
    * `1` : point de correspondance temporisé entre deux itinéraires. Le véhicule qui part doit attendre celui qui arrive et laisser suffisamment de temps pour que les usagers puissent prendre la correspondance.
    * `2` : correspondance nécessitant une durée minimale entre l'heure d'arrivée et l'heure de départ. Spécifiez la durée en question dans le champ `min_transfer_time`.
    * `3` : aucune correspondance ne peut être assurée à cet emplacement.
* `min_transfer_time` : délai (en secondes) devant être accordé pour permettre une correspondance entre deux itinéraires aux arrêts spécifiés.

Le fichier est vide pour les ensembles de données utilisés. Nous ne nous servons pas de ce fichier.

##### Données traitées par le programme

Toutes les données fournies dans un ensemble de données Horaires ne sont pas utiles pour le programme `reservation`.
Parmi les données des ensembles de données, seuls certains fichiers sont lus par le programme : `trips.txt`, `calendar.txt`, `stop_times.txt` et `stops.txt`.
Parmi les champs des fichiers lus par le programme, seuls certains sont stockés en mémoire : les champs de couleur rouge.

Etant donné que notre programme ne rencontrera pas de modifications des fichiers concernant la circulation, nous avons choisi de na pas gérer les date de début et de fin de validité d'un calendrier. Chaque trajet opère donc pour une durée illimitée.

Un trajet dans le programme peut donc être décrit comme suit :

> Un trajet effectuant la liaison depuis Bordeaux vers Nice en passant par Marseille, dans cet ordre, partant de Bordeaux à 14h03 et arrivant à Nice à 18h47, opérant tous les mardis et tous les mercredis.

Par ailleurs, les données ont été échantillonées pour réduire le volume à traiter. Seul l'ensemble de données horaires TGV a été utilisé. Il a également été réduit.

#### Places

Les fichiers `places.txt` sont tous identiques. 
Ils contiennent 6 colonnes :

* `wagon` : le numéro du wagon dans lequel se trouve la place : allant de 1 à 8.
* `classe` : la classe à laquelle appartient le wagon : allant de 1 à 2.
* `salle` : la salle à laquelle se trouve la place : allant de 0 à 1.
* `siege` : le numéro de siège de la place.
* `position` : la position de la place : allant de 0 à 2. 
* `disponible` : prend la valeur du billet qui a réservé ce siège, ou 0 si disponible.

Ils sont construits du fichier `./data/train/placement_tgv_duplex_echantillon.txt` qui est un échantillonnage du fichier `./data/train/placement_tgv_duplex.txt`(TGV duplex génération 1 ).

##### Structure Places

La structure contenant les places est articulée ainsi :

* `idtrajet` : l'identifiant du trajet
* `type` : le type du train
* `date[]` -> un tableau des dates pour lesquelles ce trajet circule
    * `date` : la date au format AAAAMMJJ
    * `sequence[]` -> un tableau des sequences du trajet 
        * `gd`: la gare de départ de la séquence
        * `ga` : la gare d'arrivée de la séquence
        * `seqdep` : la position de la gare de départ dans son trajet  
        * `seqarr` : la position de la gare d'arrivée dans son trajet 
        * `hd` : l'heure de départ de la séquence
        * `ha` : l'heure d'arrivée de la séquence
        * `place[]` -> un tableau des places (disponibilité et caractéristiques) pour cette séquence
          * `wagon`
          * `classe`
          * `salle`
          * `siege`
          * `position`
          * `billet`

#### Tarifs

La structure contenant les tarifs est articulée ainsi :

* `garedep` : la gare de départ ou d'arrivée
* `garearr`: la gare d'arrivée ou de départ
* `prix0` : le tarif d'appel en seconde classe
* `prix2`: le plein tarif en seconde classe
* `prix1`: le plein tarif en première classe

Elle est construite à partir du fichier `./data/tarif/tarifs-tgv-par-od.csv`.

#### Billets

Le fichier `registre_billets.txt` contient les informations suivantes réparties dans X colonnes :

* `idtrajet` : une chaine de caractères identifiant le trajet.
* `garedep` : une chaine de caractères identifiant la gare de départ par son nom.
* `garearr` : une chaine de caractères identifiant la gare d'arrivée par son nom.
* `nom` : une chaine de caractères contenant le nom du passager.
* `prenom` : une chaine de caractères contenant le prenom du passager.
* `date` : la date de voyage au format AAAAMMJJ.
* `age` : l'age du passager.
* `classe` : la classe du wagon dans lequel le passager voyage.
* `wagon` : le numéro du wagon dans lequel le passager voyage.
* `salle` : la salle du wagon dans lequel le passager voyage.
* `siege` : le numéro du siège dans lequel le passager voyage.
* `position` : la position du siège dans lequel le passager voyage.
* `prix` : le prix que le passager a payé pour voyager.
* `billet` : le code du billet.

## Processus

### Processus Support Amont

Au lancement du programme, une série de procédure sont lancées afin de récupérer les données depuis les dossiers et fichiers.

Chronologiquement, on récupère d'abord les données relatives aux horaires, puis celles relatives aux places.

#### Chargement des données Horaires

* La procédure ouvre le fichier `./data/horaire/trips.txt` et charge les données suivantes en mémoire :
    * `idtrajet`
    * `idservice`
    * `numtrain`
    * `direction`
* La procédure ouvre le fichier `./data/horaire/calendar.txt` et charge les données suivantes en mémoire :
    * `idservice`
    * `lun` ; `mar` ; `mer` ; `jeu` ; `ven` ; `sam` ; `dim`
* La procédure ouvre le fichier `./data/horaire/stop_times.txt` et charge les données suivantes en mémoire :
    * `sequence`
    * `idgare`
    * `hd` : heure de départ
    * `ha` : heure d'arrivée
* La procédure ouvre le fichier `./data/horaire/stops.txt` et charge les données suivantes en mémoire :
    * `idgare`
    * `nomgare`

#### Creation du tableau des dates ouvertes à la vente

* La procédure incrémente les jours à partir de la date système jusqu'à la date limite désirée (1 mois ici)
* La procédure convertit chaque date en une donnée de type `integer` au format AAAAMMJJ et la range dans un tableau

#### Chargement des données Places

* La procédure commence par compter le nombre de dossier présent dans `./data/place` :
    * S'il n'y en a aucun, elle crée tous les dossiers ```./data/place/<idtrajet>```
    * S'il en existe, elle crée les éventuels dossier manquants
* La procédure lit le fichier `places.txt` de référence
* La procédure vérifie, pour chaque date, pour chaque trajet s'il circule à cette date :
    * Si oui et que le dossier ```./data/place/<idtrajet>/<date>``` correspondant est manquant elle le crée 
* La procédure supprime ensuite tous les dossiers ```./data/place/<idtrajet>/<date>``` dont la date ne figure pas dans le tableau des dates ouvertes à la vente
* La procédure vérifie ensuite, pour chaque date pour chaque trajet, s'il existe des dossiers ```./data/place/<idtrajet>/<date>/<séquence>``` et crée les éventuels dossiers manquants
* La procédure vérifie ensuite, pour chaque séquence pour chaque date pour chaque trajet, s'il existe des fichiers ```./data/place/<idtrajet>/<date>/<séquence>/places.txt```
    * Si non, elle les crée en utilisant le fichier `places.txt` de référence
* La procédure lit alors le fichier et le charge dans la structure contenant les places 

### Processus Coeur de Métier

Une fois les données récupérées, on lance la procédure mère de recherche. Enfin, elle lance également la procédure mère de réservation.

#### Recherche

Une fois les données récupérées, on lance la procédure mère de recherche. C'est elle qui appelle la plupart des procédures et fonctions Coeur de Métier, ainsi que les saisies utilisateurs et l'affichage des résultats.

##### Saisies Utilisateurs et Recherches Itératives

* La procédure commence par demander à l'utilisateur de saisir la gare de départ via une saisie sécurisée
* La procédure lance ensuite la fonctionnalité de recherche de trajet en fonction de la gare
    * S'il n'y a pas de résultat, la procédure de recherche se termine ici
* La procédure demande ensuite à l'utilisateur de saisir la gare d'arrivée via une saisie sécurisée
* La procédure lance de nouveau la fonctionnalité de recherche de trajet en fonction de la gare
    * S'il n'y a pas de résultat, la procédure de recherche se termine ici
* La procédure lance ensuite la fonctionnalité de filtrage des trajets, ne conservant que les trajets contenant les bonnes gares de départ et d'arrivée, dans le bon sens
    * S'il n'y a pas de résultat, la procédure de recherche se termine ici
* La procédure demande ensuite à l'utilisateur de saisir la date de voyage via une saisie sécurisée
* La procédure lance ensuite la fonctionnalité de filtrage des trajets en fonction du jour de la semaine découlant de la date de voyage renseignée
    * S'il n'y a pas de résultat, la procédure de recherche se termine ici
* La procédure lance ensuite la fonctionnalité de quantification des places disponibles pour chaque trajet, pour la Première et la Seconde Classe
* La procédure lance ensuite la fonctionnalité de tri en fonction de l'heure de départ de chaque trajet

##### Affichage des Résultats et Choix Utilisateur

* La procédure procède alors à l'affichage des résultats sous forme de tableau
* La procédure affiche un menu sous le tableau des résultats et demande à l'utilisateur de choisir entre :
    * Réserver un des trajets présentés
        * La procédure demande à l'utilisateur de choisir un des trajets proposés
        * La procédure lance la procédure mère de réservation
    * Afficher les résultats pour le jour suivant
        * La procédure lance la fonctionnalité d'incrémentation de la date
        * La procédure relance alors la fonctionnalité de filtrage des trajets en fonction du jour de la semaine découlant de la nouvelle date de voyage
            * S'il n'y a pas de résultat, la procédure de recherche se termine ici
        * La procédure lance de nouveau la fonctionnalité de quantification des places disponibles pour chaque trajet, pour la Première et la Seconde Classe
        * La procédure lance de nouveau la fonctionnalité de tri en fonction de l'heure de départ de chaque trajet
        * La procédure reboucle sur l'affichage des résultats sous formes de tableau, affiche le menu sous le tableau des résultats et demande à l'utilisateur de choisir de nouveau
    * Afficher les résultats pour le jour précédent
    * La procédure lance la fonctionnalité de décrémentation de la date
        * La procédure relance alors la fonctionnalité de filtrage des trajets en fonction du jour de la semaine découlant de la nouvelle date de voyage
            * S'il n'y a pas de résultat, la procédure de recherche se termine ici
        * La procédure lance de nouveau la fonctionnalité de quantification des places disponibles pour chaque trajet, pour la Première et la Seconde Classe
        * La procédure lance de nouveau la fonctionnalité de tri en fonction de l'heure de départ de chaque trajet
        * La procédure reboucle sur l'affichage des résultats sous formes de tableau, affiche le menu sous le tableau des résultats et demande à l'utilisateur de choisir de nouveau
    * Quitter la procédure

#### Reservation

La procédure mère de réservation s'occupe des saisies utilisateurs et des vérifications relatives à la réservation, et lance les fonctions d'écriture dans la structure des places et dans la structure des billets.

##### Saisies Utilisateurs et Sélection de Place(s)

* La procédure commence par demander à l'utilisateur dans quelle classe il souhaite voyager via une saisie sécurisée
  * L'utilisateur a la possibilité de revenir au menu des résultats 
* La procédure demande alors à l'utilisateur de choisir le nombre de sièges qu'il souhaite réserver, dans la limite du nombre de places disponibles pour la classe qu'il a choisi
  * L'utilisateur a la possibilité de revenir au menu des résultats 
* La procédure lance alors une boucle pour chaque siège demandé :
  * La procédure demande à l'utilisateur de saisir un nom, un prenom et un age
  * Si l'utilisateur avait choisi la Première Classe :
    * La procédure affiche alors un tableau des places disponibles en Première Classe et demande à l'utilisateur de choisir une des places proposées
      * L'utilisateur a la possibilité de revenir au menu des résultats
  * Si l'utilisateur avait choisi la Seconde Classe :
    * La procédure propose à l'utilisateur de choisir entre salle haute et salle basse si des places sont disponibles dans les deux. Sinon il l'informe qu'il ne reste que des places dans une des salles.
    * La procédure propose à l'utilisateur de choisir une position de siège entre celles disponibles parmi "fenetre", "couloir" et "isolé" si au moins deux des trois sont disponibles. Sinon il l'informe qu'il ne reste que des places pour une des positions.
      * La procédure demande à l'utilisateur si ses choix, ou ces assignations lui conviennent, ou s'il préfère revenir au menu des résultats
* La procédure affiche le montant total de la réservation
  * L'utilisateur a la possibilité de revenir au menu des résultats
* La procédure lance la fonctionnalité de paiement
* Une fois le paiement effectué avec succès, la procédure lance la fonctionnalité d'écriture des réservations dans la structure contenant les places
* La procédure lance ensuite la fonctionnalité d'écriture des billets dans la structure de registre des billets

### Processus Support Aval



#### Sauvegarde des Places



## Tests

### Tests Métiers



### Tests Techniques


