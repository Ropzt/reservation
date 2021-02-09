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
Elle souhaite disposer d'un logiciel (programme ?) de réservation de place : `reservation`.

## Portée

`reservation` est un programme de réservation de place dans les différents modes de transport assurés par la SNCF.

Il permet xx fonctions principales : (ne pas s'arrêter aux interfaces présentées à l'utilisateur ?)

* la consultation d'horaires de voyage entre deux gares données à une date donnée ;
* la consultation des tarifs de ces voyages selon (préciser plus tard ?)
  * la classe du wagon
  * (le profil du voyageur : carte de réduction)
* la réservation de places
* (la gestion des réservations d'un client)

* la gestion des places disponibles à bord d'un train
* le calcul des tarifs de voyage


`reservation` ne permet pas :

* la réservation de voyages incluant des correspondances.

## Définitions

**billet**, **billet de train** : titre de transport valant pour une place et pour un voyage.

**classe** : catégorie de prestation offerte à bord d'un wagon. Certains trains possèdent des wagons de différentes classes : première classe (classe 1) et seconde classe (classe 2). D'autres trains possèdent  des wagons d'une seule et même classe.

**date système**, **date/heure système** et **heure système** : informations de date, date/heure et heure de l'équipement terminal sur lequel le programme est utilisé.  
D'un point de vue métier, il s'agit de la date, date/heure et heure courantes, c'est-à-dire la date, date/heure et heure d'utilisation du programme, sachant que le comportement du programme dépend de cette date, date/heure et heure (les voyages proposés ont une date/heure postérieure à la date courante).  
Dans le programme, ces informations sont obtenues via l'équipement terminal sur lequel le programme est exécuté. Elles s'appellent donc date système, date/heure système et heure système malgré la consonnance très peu « métier ».
* **date** désigne le jour calendaire d'une année donnée (jour de semaine, jour du mois, mois, année). *Exemple : dimanche 14/02/2021*.
* **heure** désigne un horaire (heures et minutes). *Exemple : 23:59*.
* **date/heure** désigne un horaire d'une date. *Exemple : dimanche 14/02/2021 23:59*.

**gare** : station à partir ou au départ de laquelle il est possible de faire un voyage assuré par la SNCF. 
Il peut s'agir d'une gare de chemin de fer ou d'une gare routière. 
Une ville peut posséder plusieurs gares.

**rame** : train au sens « matériel roulant». *compléter : dire quels attributs sont propres à la rame (le nb de places, etc.) là où d'autres attributs sont propres au train ou au voyage*

**terminal** : équipement terminal, appareil, sur lequel le programme est utilisé. Dans la version courante du programme, seul l'ordinateur peut être un terminal.

**train** : tout véhicule dont la circulation est assurée par la SNCF et dont l'utilisateur peut réserver une place. 
Les trains sont de différents types, qu'ils circulent sur voie ferrée ou routière (TGV, INOUI, TER, OUIGO, Car).

**service** : ligne commerciale définie par une gare de départ, une gare d'arrivée, des gares d'arrêt, des horaires de départ et d'arrivée dans chacune des gares de départ, d'arrivée et d'arrêt, et un calendrier de circulation (circule ou ne circule pas, pour chaque jour de semaine). Le service est le numéro de train utilisé en gare et sur un billet de train pour la communication avec les voyageurs.

**voyage** déplacement en train entre deux gares à une date donnée.

...

## Vue d'ensemble

Dire comment est structurée la suite du document

# Analyse métier

La SNCF assure la circulation de différents types de trains.

## Train

La SNCF fait circuler différents types de trains, indiqués par le type TGV, INOUI, OUIGO, TER, Intercités, Car.  
Le « type » désigne des réalités mixtes, à la fois commerciales et techniques.

Le train au sens de « matériel roulant » est désigné par « rame ».
Un type de train peut utiliser un ou plusieurs types de rames.
Un train de type Car utilisera probablement une rame standard (un car) avec un nombre de places fixe, tandis qu'un train de type TGV utilise différents types de rames, selon la ligne de chemin de fer (une rame duplex sur Paris-Lyon, une rame simplex sur une autre ligne ou sur un autre service.

Chaque type de rame a une composition propre, mais de façon commune :

* une rame est constituée d'un ou plusieurs wagons
* les wagons d'une rame sont de même classe ou de classes différentes
* un wagon est simplex ou duplex, il possède alors une ou deux salles (salle basse et salle haute)
* une salle est consituée de places
* une place a un placement. Selon le type de wagon dans laquelle elle se trouve, le placement peut être : fenêtre, couloir, isolée.

L'utilisation d'un type de rame en fonction du service est assez complexe à comprendre/trouver.
L'affectation des rames aux services semble avoir une base en partie régionale, liée aux commandes de rames lors de l'ouverture d'une nouvelle ligne.
À titre d'exemple, un TGV peut utiliser une rame TGV Sud-Est, TGV Atlantique, TGV Réseau, TGV Duplex, TGV Réseau Duplex, TGV Duplex Dasye, TGV 2N2, TGV PBA, TGV TMST, TGV POS, TGV 2N2, TGV IRIS 320, TGV M, TGV PBKA, Eurostar e320 (https://fr.wikipedia.org/wiki/Mat%C3%A9riel_moteur_de_la_SNCF#Parc_%C3%A9lectrique).
Dans le cadre de ce prototype, nous utilisons une composition de rame identique pour tous les services d'un même type de train, tout en permettant un paramètrage différent au cas où ces informations deviennent accessibles.

### Composition des rames

#### TGV, INOUI

Une rame de train de type TGV ou INOUI est composée de 8 wagons :

* 1 wagon-bar (n°4)
* 7 wagons de passagers (n°1 à 3 et n°5 à 8)

Parmi les 7 wagons de passagers, 3 sont de classe 1 (n°1 à 3), 4 sont de classe 2 (n°5 à 8).

Une rame de train de type TGV ou INOUI peut être simplex ou duplex.

*Infos wikipédia TGV Duplex (1997 à 2012):  182 en 1re, 328 en 2e (total 510)
TGV 2N2 (duplex à partir de 2011) : 510
simplex : total 133 places de moins*

##### Rame simplex

*Infos wikipédia : => 377places*

Une rame simplex est composée de wagons à 1 niveau.

##### Rame duplex

Une rame duplex est composée de wagons à 2 niveaux, et possède ainsi une salle basse et une salle haute.
Dans une salle, la disposition des sièges n'est pas régulière. Certains sièges sont remplacés par des espaces d'entrepot des bagages.

Une salle de wagon de classe 1 est composée au maximum de 9 rangs de sièges.
Un rang est composé au maxium de 3 sièges, dont 
* un siège situé côté fenêtre
* un siège situé côté couloir
* un siège isolé

Une salle de wagon de classe 2 est composée de 13 rangs de sièges.
Un rang de composé de 4 sièges, dont
* deux sièges côté fenêtre
* deux sièges côté couloir

#### OUIGO

*Infos wikipédia : 634 places en classe unique*

Un train de type OUIGO est composé de 8 wagons de passagers à 2 niveaux.

Chaque wagon est composé de 


## Circulation des trains

Les trains circulent selon un calendrier



## Ouverture des ventes

Les ventes de billets sont ouvertes avec une antécédence, par rapport au voyage, différente selon le type de train :

* TGV, INOUI & Intercités : 4 mois, sauf pour les vacances d'été, de Noël et d'hiver
* TGV Bruxelles, Italo, TGV Luxembourg-Paris, TGV Fribourg-Paris : 4 mois
* Trajets RENFE/SNCF, ALLEO France-Allemagne, TGV Lyria (entre Lille et Genève) : 4 mois
* TGV France-Italie : 4 mois
* TER : en fonction des régions, entre 2 et 5 mois
* OUIGO : entre 2 et 9 mois
* THALYS (vers la Belgique, les Pays-Bas, l'Allemagne) : 4 mois
* Eurostar (vers l'Angleterre) : 6 mois pour Paris, Lille et Calais vers London, Ashford, Ebbsfleet, 150 à 280 jours pour Lyon, Marseille et Avignon, vers London, Ashford, Ebbsfleet

L'ouverture des ventes vacances de printemps (pour des circulations du 29 mars au 15 mai 2021) se fait le 12 janvier 2021.

Lorsqu'un train est ouvert à la vente, ses places doivent être réservables.

Au démarrage du système, il faut 

* créer des places réservables pour les trains qui s'ouvrent à la vente :
  * regarder la date système
  * parser les services
  * pour chaque service, regarder son type de train
  * calculer la date la plus tardive (date limite) à laquelle ses places sont ouvertes à la réservation
  * si elle n'existe pas, créer une rame pour chaque date entre la date système et la date limite, avec les caractéristiques correspondant au type de train
* supprimer les rames des trains qui sont arrivés
  * regarder la date/heure système
  * parser les dates
  * si la date est antérieure à la date système, supprimer ses rames




## Réservation

Le processus de réservation d'une place de train suit généralement les étapes suivantes :

1. Recherche
  * choix de la gare/ville de départ
  * choix de la gare/ville d'arrivée
  * choix de la date
  * choix des options de voyage : classe de wagon, tarif particulier.
2. Consultation des résultats
3. (optionnel) Modification des critères de recherche
4. Choix et réservation d'un voyage parmi les résultats

### Recherche

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

### Gestion de la date/heure

Concernant la date/heure, le cas de la réservation de voyage et le cas de la consultation d'horaires sont différents.  
Dans le cas d'une réservation de voyage, le programme ne doit pas proposer de résultats de voyages partant à une date/heure antérieure à la date/heure système.
Dans le cas d'une consultation d'horaire, le programme ne doit pas proposer de résultats de voyages arrivant à une date/heure antérieure à la date/heure système.




Un voyageur/agent doit pouvoir consulter les horaires de voyages en indiquant :

* la ville de départ
* la ville d'arrivée

Un voyageur/agent doit pouvoir réserver des places en choisissant :

* la gare de départ
* la gare d'arrivée
* la date du voyage
* le nombre de places



# Spécifications (le lien avec notre programme)



------------------


## Fonctions

* afficher les horaires

* afficher les tarifs

* calcul du tarif

  * en fonction du trajet
  * en fonction de la catégorie de place
  * en fonction d'un programme de réduction
  * en fonction du remplissage

* réserver des places

* gestion du nombre de places disponibles

* catégories de places

* éditer les billets

## Base de données


### Jeux de données

Les jeux de données utilisés sont ceux de la SNCF, mis à disposition à [](https://ressources.data.sncf.com/explore/)

* Horaires
  * Horaires des TGV : https://ressources.data.sncf.com/explore/dataset/horaires-des-train-voyages-tgvinouiouigo/table/
  * Horaires des lignes TER : https://ressources.data.sncf.com/explore/dataset/sncf-ter-gtfs/table/
  * Horaires des lignes Transilien : https://ressources.data.sncf.com/explore/dataset/sncf-transilien-gtfs/table/
  * Horaires des lignes Intercités : https://ressources.data.sncf.com/explore/dataset/sncf-intercites-gtfs/table/
  * Horaires des Tram-Train TER Pays de la Loire : https://ressources.data.sncf.com/explore/dataset/sncf-tram-train-ter-pdl-gtfs/table/
* Tarifs
  * **à compléter**

### Description des données

#### Horaires

Les jeux de données Horaires (de façon identique pour chaque jeu) sont constitués de 8 tables :

* `agency.txt`
* `calendar_dates.txt`
* `calendar.txt`
* `routes.txt`
* `stop_times.txt`
* `stops.txt`
* `transfers.txt`
* `trips.txt`

Le schéma relationnel est le suivant :

[mettre le schéma]

La table `agency.txt` décrit les **??agences??**.
Elle comporte les champs :

* `agency_id` : 
* `agency_name` : 
* `agency_url` :
* `agency_timezone` :
* `agency_lang` :

La table `routes.txt` décrit les lignes de voie ferrée.
Elle est liée à `agency.txt` par le champ `agency_id`.
Elle comporte les champs :

* `route_id` :
* `agency_id` :
* `route_short_name` :
* `route_long_name` :
* `route_desc` :
* `route_type` :
* `route_url` :
* `route_color` :
* `route_text_color` :

La table `trips.txt` décrit les lignes commerciales (par exemple : ).
Elle est liée à `routes.txt` par le champ `route_id`.
Elle comporte les champs : 

* `route_id` : l'identifiant de route sur laquelle la ligne commerciale circule.
* `service_id` : l'identifiant du **??service??**
* `trip_id` : l'identifiant du **??trip??**
* `trip_headsign` : le numéro de train tel qu'affiché en gare et sur les billets
* `direction_id` :
* `block_id` :
* `shape_id` :

La table `stop_times.txt` décrit les arrêts en gare pour 
Elle comporte les champs : 

* `trip_id` : le **trip** concerné
* `arrival_time` : l'heure d'arrivée en gare
* `departure_time` : l'heure de départ de la gare
* `stop_id` : l'identifiant de la gare d'arrêt
* `stop_sequence` :
* `stop_headsign` :
* `pickup_type` :
* `drop_off_type` :
* `shape_dist_traveled` :

La table `stops.txt` décrit les lieux d'arrêt, c'est-à-dire les gares.
Elle est liée à la table `stop_times.txt` par le champ `stop_id`.
Elle comporte les champs :

* `stop_id` : l'identifiant de la gare d'arrêt
* `stop_name` : le nom de la gare d'arrêt
* `stop_desc`
* `stop_lat` : la latitude de la gare d'arrêt
* `stop_lon` : la longitute de la gare d'arrêt
* `zone_id` : 
* `stop_url` : 
* `location_type` : 
* `parent_station` :

La table `calendar.txt` décrit les jours de service pour un service donné.
Elle est liée à la table `trips.txt` par le champ `service_id`.
Elle comporte les champs :

* `service_id` : l'identifiant du **service** concerné
* `monday` : valeur boléenne de circulation le lundi
* `tuesday` : valeur boléenne de circulation le mardi
* `wednesday` : valeur boléenne de circulation le mercredi
* `thursday` : valeur boléenne de circulation le jeudi
* `friday` : valeur boléenne de circulation le vendredi
* `saturday` : valeur boléenne de circulation le samedi
* `sunday` : valeur boléenne de circulation le dimanche
* `start_date` : date de début de validité du service
* `end_date` : date de fin de validité du service

La table `calendar_dates.txt` décrit ??des exceptions au calendrier de circulation défini par `calendar.txt` pour un service donné et une date donnée, en précisant la nature de l'exception.
Elle est liée à la table `trips.txt` ou `calendar.txt` par le champ `service_id`
Elle comporte les champs : 

* `service_id` : l'identifiant du **service** concerné
* `date` : la date de circulation concernée
* `exception_type` : le type d'exception

Nous ne nous servont pas de cette table.

La table `transferts.txt` décrit ??
Elle comporte les champs :

* `from_stop_id`
* `to_stop_id`
* `transfer_type`
* `min_transfer_time`

La table est vide pour les jeux de données TGV, TER, d'autres ?