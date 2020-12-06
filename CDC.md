# Cahier des charges

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