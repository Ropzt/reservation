tarif_tgv <- read.csv("C:/Users/pariz/Documents/Github/reservation/Tarifs/tarifs-tgv-par-od.csv", header=TRUE,sep=";", fileEncoding = "UTF-8")
library(stringr)
library(dplyr)
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "BELFORT-MONTBELIARD", "BELFORT45MONTBELIARD")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "BESANCON FRANCHE-COMTE", "BESANCON FRANCHE45COMTE")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "AIX-LES-BAINS - LE REVARD", "AIX45LES45BAINS 45 LE REVARD")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "Chalon-sur-Saône", "Chalon45sur45Saône")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "la-teste", "la45teste")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "LILLE EUROPE-147322", "LILLE EUROPE")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "ST AVRE - LA CHAMBRE", "ST AVRE 45 LA CHAMBRE")
tarif_tgv$OD<-str_replace(tarif_tgv$OD, pattern = "VITRY-LE-FRANÇOIS", "VITRY45LE45FRANÇOIS")

tarif_nolaval <- tarif_tgv%>%filter(str_count(tarif_tgv$OD, pattern = "-")==1)
tarif_laval <- tarif_tgv%>%filter(str_count(tarif_tgv$OD, pattern = "-")>1)
tarif_laval$OD <- str_remove(tarif_laval$OD, pattern="^[^-]{1,}")
tarif_laval$OD <- str_remove(tarif_laval$OD, pattern="^-{1}")
tarif_tgv2<-bind_rows(tarif_nolaval,tarif_laval)

tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[éèêë]", "e")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ÊËÈÉ]", "E")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[àâäáã]", "a")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ÄÂÀÁÃ]", "A")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ÏÎÌÍ]", "I")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ôöòó]", "o")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ÖÔÒÓ]", "O")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ùûüú]", "u")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ÜÛÚÙ]", "U")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "[ÿý]", "y")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "ç", "c")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "Ý", "Y")
tarif_tgv2$OD <- str_replace_all(tarif_tgv2$OD, pattern = "Ç", "C")

tarif_tgv2 <- tarif_tgv2%>%mutate(list = str_split(tarif_tgv2$OD, pattern="-"))
tarif_tgv2 <- tarif_tgv2%>%mutate(dep = lapply(tarif_tgv2$list, function(x) x[1]))
tarif_tgv2 <- tarif_tgv2%>%mutate(arr = lapply(tarif_tgv2$list, function(x) x[2]))
tarif_tgv2$dep<-unlist(tarif_tgv2$dep)
tarif_tgv2$arr<-unlist(tarif_tgv2$arr)

tarif_tgv2$dep<-str_replace_all(tarif_tgv2$dep, pattern="45", "-")
tarif_tgv2$arr<-str_replace_all(tarif_tgv2$arr, pattern="45", "-")

##tarif_tgv2$dep<-str_replace_all(tarif_tgv2$dep, pattern="[:digit:]", "-")
##tarif_tgv2$arr<-str_replace_all(tarif_tgv2$arr, pattern="45", "-")

tarif_tgv2$dep<-str_trim(tarif_tgv2$dep)
tarif_tgv2$arr<-str_trim(tarif_tgv2$arr)

tarif_tgv2$Prix.d.appel.2nde<-str_replace_na(tarif_tgv2$Prix.d.appel.2nde, "0")
tarif_tgv2$Prix.d.appel.2nde<-as.numeric(tarif_tgv2$Prix.d.appel.2nde)

tarif_tgv2<-tarif_tgv2%>%select(c('dep','arr','Prix.d.appel.2nde','Plein.Tarif.Loisir.2nde','X1ère.classe'))

write.table(tarif_tgv2,"tarif_tgv_header.csv", sep=";", quote=FALSE, row.names = FALSE ,col.names = TRUE)


tarif_tgv2$dep<-str_to_upper(tarif_tgv2$dep)
tarif_tgv2$arr<-str_to_upper(tarif_tgv2$arr)


tarif_tgv2$dep <- str_replace_all(tarif_tgv2$dep, pattern = " ", "_")
tarif_tgv2$dep <- str_replace_all(tarif_tgv2$dep, pattern = "-", "_")
tarif_tgv2$dep <- str_replace_all(tarif_tgv2$dep, pattern = "[:digit:]", "_")
tarif_tgv2$arr <- str_replace_all(tarif_tgv2$arr, pattern = " ", "_")
tarif_tgv2$arr <- str_replace_all(tarif_tgv2$arr, pattern = "-", "_")
tarif_tgv2$arr <- str_replace_all(tarif_tgv2$arr, pattern = "[:digit:]", "_")
tarif_tgv2$dep <- str_remove_all(tarif_tgv2$dep, pattern = "_")
tarif_tgv2$arr <- str_remove_all(tarif_tgv2$arr, pattern = "_")




g1 <- tarif_tgv2%>%distinct(dep)
g2 <- tarif_tgv2%>%distinct(arr)


g1$dep<-str_to_upper(g1$dep)
g2$arr<-str_to_upper(g2$arr)


g1$dep <- str_replace_all(g1$dep, pattern = " ", "_")
g1$dep <- str_replace_all(g1$dep, pattern = "-", "_")
g1$dep <- str_replace_all(g1$dep, pattern = "[:digit:]", "_")
g2$arr <- str_replace_all(g2$arr, pattern = " ", "_")
g2$arr <- str_replace_all(g2$arr, pattern = "-", "_")
g2$arr <- str_replace_all(g2$arr, pattern = "[:digit:]", "_")
g1$dep <- str_remove_all(g1$dep, pattern = "_")
g2$arr <- str_remove_all(g2$arr, pattern = "_")


g3 <- full_table%>%distinct(stop_name)
g3$stop_name<-str_to_upper(g3$stop_name)
g3$stop_name  <- str_replace_all(g3$stop_name , pattern = " ", "_")
g3$stop_name  <- str_replace_all(g3$stop_name , pattern = "-", "_")
g3$stop_name  <- str_replace_all(g3$stop_name , pattern = "[:digit:]", "_")
g3$stop_name  <- str_remove_all(g3$stop_name , pattern = "_")



