# Aetherion_code
  Les différents fichiers regroupe l'intégralité du code conçues et utilisé pour la fusée Ætherion.

## aerofrein
  Code utilisé pour contrôler les aérofrein de la fusée. Le déclenchement des aérofrein est contrôlé par un timer. Ce timer était réglé en fonction des simulations numérique de la fusée. Le déclenchement du compte à rebours est assuré par un signal exterieur, indiquand le décollage de la fusée. 

## lecture_pcb/lecture_2pcb/lecture_2pcb_c
  Code utilisé pour faire la lecture des pcb comportants les capteurs de pression. Ce code fait la lecture en pipeline de 4 (lecture_pcb) ou de 8 capteurs de pression (lecture_2pcb/lecture_2pcb_c), puis enregistre ses données dans une carte XTSD (carte SD soudé).

## libaltimetreV2
  Librairie pour utliser le capteur de pression MPL3115a2 sans avoir de "delay". Ce que ne propose par la libraire Adafruit.

## logger
  Code pour enregister les informations envoyé par série de l'ordinateur de bord. Les données sont stockées sur une XTSD (carte SD soudé).

## ordi_de_bord_ae
  Code de l'ordinateur de bord de la fusée Ætherion. Lecture d'une central inertiel (BNO055), d'un altimètre (MPL3115a2) et d'un GPS (ubloxM8n). Envoie de données par LoRa sur plus de 3km, et aussi par série au logger. Détection du décollage via optocoupleur par le séquenceur, et renvoie de l'information à tout les autres système.

## sequenceur
  Code du séquenceur de la fusée Ætherion. Permet l'éjection du parachute par 2 méthode, soit par fenêtrage temporel (au bout d'un certain temps la trappe parachute s'ouvre), soit par détection de chute, en dérivant l'altitude.

## station_sol_ae
  Code pour recevoir les données LoRa émis par l'ordinateur de bord, et de les afficher sur un terminal par liaison Série.


