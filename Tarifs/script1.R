tgv_calendar <- read.csv("C:/Users/pariz/Documents/M2 MIMO/Langage C/SNCF/reservation-main/data_sncf/export_gtfs_voyages/calendar.txt", header=TRUE, sep=",", stringsAsFactors = FALSE)
tgv_stops <- read.csv("C:/Users/pariz/Documents/M2 MIMO/Langage C/SNCF/reservation-main/data_sncf/export_gtfs_voyages/stops.txt", header=TRUE, sep=",", stringsAsFactors = FALSE, fileEncoding = "UTF-8")
tgv_stoptime <- read.csv("C:/Users/pariz/Documents/M2 MIMO/Langage C/SNCF/reservation-main/data_sncf/export_gtfs_voyages/stop_times.txt", header=TRUE, sep=",", stringsAsFactors = FALSE)
tgv_trips <- read.csv("C:/Users/pariz/Documents/M2 MIMO/Langage C/SNCF/reservation-main/data_sncf/export_gtfs_voyages/trips.txt", header=TRUE, sep=",", stringsAsFactors = FALSE)

tgv_stops <- tgv_stops %>% select(c('stop_id', 'stop_name', 'parent_station'))
tgv_stoptime <- tgv_stoptime %>% select(c('trip_id','arrival_time','departure_time','stop_id','stop_sequence'))
tgv_trips <- tgv_trips %>% select(c('trip_id','service_id','trip_headsign'))

tgv_stops_time <- left_join(tgv_stoptime,tgv_stops, by="stop_id")
tgv_stops_time_trips <- left_join(tgv_stops_time,tgv_trips, by="trip_id")
tgv_stops_time_trips_calend <- left_join(tgv_stops_time_trips, tgv_calendar, by="service_id")

##tgv_stops_time_trips_calend <- tgv_stops_time_trips_calend %>% filter(!is.na(monday) && !is.na(tuesday) && !is.na(wednesday) && !is.na(thursday) && !is.na(friday) && !is.na(saturday) && !is.na(sunday))
full_table <- tgv_stops_time_trips_calend %>% filter(!is.na(monday))

full_table$stop_name <- str_remove_all(full_table$stop_name, pattern = "Gare de ")

full_table <- full_table %>% filter(stop_name != "")

full_join<-full_table
full_table <- full_table%>% mutate(stop_proche = stop_sequence+1)
full_join <- full_join%>% mutate(stop_proche = stop_sequence)

full_join <- left_join(full_join,full_table, by="stop_proche")

full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[éèêë]", "e")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ÊËÈÉ]", "E")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[àâäáã]", "a")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ÄÂÀÁÃ]", "A")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ÏÎÌÍ]", "I")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ôöòó]", "o")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ÖÔÒÓ]", "O")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ùûüú]", "u")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ÜÛÚÙ]", "U")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "[ÿý]", "y")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "ç", "c")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "Ý", "Y")
full_table$stop_name <- str_replace_all(full_table$stop_name, pattern = "Ç", "C")

full_table$stop_name  <- str_replace_all(full_table$stop_name , pattern = " ", "_")
full_table$stop_name  <- str_replace_all(full_table$stop_name , pattern = "-", "_")
full_table$stop_name  <- str_replace_all(full_table$stop_name , pattern = "[:digit:]", "_")
full_table$stop_name  <- str_remove_all(full_table$stop_name , pattern = "_")

full_table$stop_name<-str_to_upper(full_table$stop_name)

write.table(full_table,"horaire_tgv_header.csv", sep=";", quote=FALSE, row.names = FALSE ,col.names = TRUE)

for(i in sequence(18399)){
  full_table[i,]$stop_proche = full_table[i+1,]$stop_name
}

k=0
j=0
i=0
for(i in sequence(18399)){
  for(j in sequence(4161)){
    if(full_table[i,]$stop_name==tarif_tgv2[j,]$dep)
    { if(full_table[i,]$stop_proche==tarif_tgv2[j,]$arr)
      {k=k+1}
    }
    else if(full_table[i,]$stop_name==tarif_tgv2[j,]$arr)
    { if(full_table[i,]$stop_proche==tarif_tgv2[j,]$dep)
      {k=k+1}
    }
  }
}
