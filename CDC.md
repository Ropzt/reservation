# Cahier des charges

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

## Présentation générale

### Contexte

La SNCF est une compagnie ferroviaire ayant entre autres activités le transport de voyageurs.
Elle souhaite disposer d'un logiciel (programme ?) de réservation de place : `reservation`.

### Portée

`reservation` est un programme de réservation de place dans les différents modes de transport assurés par la SNCF.

Il permet xx fonctions principales : (ne pas s'arrêter aux interfaces présentées à l'utilisateur ?)

- la consultation d'horaires de voyage entre deux gares données à une date donnée ;
- la consultation des tarifs de ces voyages selon (préciser plus tard ?)
  - la classe du wagon
  - (le profil du voyageur : carte de réduction)
- la réservation de places
- (la gestion des réservations d'un client)

- la gestion des places
- le calcul des tarifs de voyage

`reservation` ne permet pas :

- la réservation de voyages incluant des correspondances.

### Définitions

**gare** : (dire que c'est toutes les stations de départ et d'arrivée possibles parce que des fois je suppose que c'est des gares routières)

**train** : tout véhicule dont la circulation est assurée par la SNCF et dont l'utilisateur peut réserver une place. Les trains sont de différents types, qu'ils circulent sur voie ferrée ou routière (TGV, INOUI, TER, OUIGO, Car).

**voyage** (on peut changer de terme) : déplacement entre deux gares

...

### Vue d'ensemble

Dire comment est structurée la suite du document

## Analyse métier

La SNCF assure la circulation de différents types de trains.


### Voyage

Le processus de réservation d'une place de train suit généralement les étapes suivantes :

- choix de la gare/ville de départ
- choix de la gare/ville d'arrivée
- choix de la date
- choix des options de voyage : classe de wagon, tarif particulier.

Certaines villes possèdent plusieurs gares.
C'est le cas des grandes villes et des villes possédant une gare historique et une gare TGV située en dehors de la ville sur une ligne à grande vitesse.
Le voyageur peut souhaiter réserver une place pour un voyage au départ ou à l'arrivée d'une ville, indépendamment de la gare.
Les résultats affichés doivent inclure les différentes gares d'une même ville.

### Train

Un train est composé de 



Un voyageur/agent doit pouvoir consulter les horaires de voyages en indiquant :

- la ville de départ
- la ville d'arrivée

Un voyageur/agent doit pouvoir réserver des places en choisissant :

- la gare de départ
- la gare d'arrivée
- la date du voyage
- le nombre de places




## Spécifications (le lien avec notre programme)



------------------


## Fonctions

- afficher les horaires

- afficher les tarifs

- calcul du tarif

  - en fonction du trajet
  - en fonction de la catégorie de place
  - en fonction d'un programme de réduction
  - en fonction du remplissage

- réserver des places

- gestion du nombre de places disponibles

- catégories de places

- éditer les billets

## Base de données


### Jeux de données

Les jeux de données utilisés sont ceux de la SNCF, mis à disposition à [](https://ressources.data.sncf.com/explore/)

- Horaires
  - Horaires des TGV : https://ressources.data.sncf.com/explore/dataset/horaires-des-train-voyages-tgvinouiouigo/table/
  - Horaires des lignes TER : https://ressources.data.sncf.com/explore/dataset/sncf-ter-gtfs/table/
  - Horaires des lignes Transilien : https://ressources.data.sncf.com/explore/dataset/sncf-transilien-gtfs/table/
  - Horaires des lignes Intercités : https://ressources.data.sncf.com/explore/dataset/sncf-intercites-gtfs/table/
  - Horaires des Tram-Train TER Pays de la Loire : https://ressources.data.sncf.com/explore/dataset/sncf-tram-train-ter-pdl-gtfs/table/
- Tarifs
  - **à compléter**

### Description des données

#### Horaires

Les jeux de données Horaires (de façon identique pour chaque jeu) sont constitués de 8 tables :

- `agency.txt`
- `calendar_dates.txt`
- `calendar.txt`
- `routes.txt`
- `stop_times.txt`
- `stops.txt`
- `transfers.txt`
- `trips.txt`

Le schéma relationnel est le suivant :

[mettre le schéma]

La table `agency.txt` décrit les **??agences??**.
Elle comporte les champs :

- `agency_id` : 
- `agency_name` : 
- `agency_url` :
- `agency_timezone` :
- `agency_lang` :

La table `routes.txt` décrit les lignes de voie ferrée.
Elle est liée à `agency.txt` par le champ `agency_id`.
Elle comporte les champs :

- `route_id` :
- `agency_id` :
- `route_short_name` :
- `route_long_name` :
- `route_desc` :
- `route_type` :
- `route_url` :
- `route_color` :
- `route_text_color` :

La table `trips.txt` décrit les lignes commerciales (par exemple : ).
Elle est liée à `routes.txt` par le champ `route_id`.
Elle comporte les champs : 

- `route_id` : l'identifiant de route sur laquelle la ligne commerciale circule.
- `service_id` : l'identifiant du **??service??**
- `trip_id` : l'identifiant du **??trip??**
- `trip_headsign` : le numéro de train tel qu'affiché en gare et sur les billets
- `direction_id` :
- `block_id` :
- `shape_id` :

La table `stop_times.txt` décrit les arrêts en gare pour 
Elle comporte les champs : 

- `trip_id` : le **trip** concerné
- `arrival_time` : l'heure d'arrivée en gare
- `departure_time` : l'heure de départ de la gare
- `stop_id` : l'identifiant de la gare d'arrêt
- `stop_sequence` :
- `stop_headsign` :
- `pickup_type` :
- `drop_off_type` :
- `shape_dist_traveled` :

La table `stops.txt` décrit les lieux d'arrêt, c'est-à-dire les gares.
Elle est liée à la table `stop_times.txt` par le champ `stop_id`.
Elle comporte les champs :

- `stop_id` : l'identifiant de la gare d'arrêt
- `stop_name` : le nom de la gare d'arrêt
- `stop_desc`
- `stop_lat` : la lattitude de la gare d'arrêt
- `stop_lon` : la longitute de la gare d'arrêt
- `zone_id` : 
- `stop_url` : 
- `location_type` : 
- `parent_station` :

La table `calendar.txt` décrit les jours de service pour un service donné.
Elle est liée à la table `trips.txt` par le champ `service_id`.
Elle comporte les champs :

- `service_id` : l'identifiant du **service** concerné
- `monday` : valeur boléenne de circulation le lundi
- `tuesday` : valeur boléenne de circulation le mardi
- `wednesday` : valeur boléenne de circulation le mercredi
- `thursday` : valeur boléenne de circulation le jeudi
- `friday` : valeur boléenne de circulation le vendredi
- `saturday` : valeur boléenne de circulation le samedi
- `sunday` : valeur boléenne de circulation le dimanche
- `start_date` : date de début de validité du service
- `end_date` : date de fin de validité du service

La table `calendar_dates.txt` décrit ??des exceptions au calendrier de circulation défini par `calendar.txt` pour un service donné et une date donnée, en précisant la nature de l'exception.
Elle est liée à la table `trips.txt` ou `calendar.txt` par le champ `service_id`
Elle comporte les champs : 

- `service_id` : l'identifiant du **service** concerné
- `date` : la date de circulation concernée
- `exception_type` : le type d'exception

Nous ne nous servont pas de cette table.

La table `transferts.txt` décrit ??
Elle comporte les champs :

- `from_stop_id`
- `to_stop_id`
- `transfer_type`
- `min_transfer_time`

La table est vide pour les jeux de données TGV, TER, d'autres ?