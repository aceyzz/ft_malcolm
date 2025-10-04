<img title="42_ft_malcolm" alt="42_ft_malcolm" src="./utils/banner.png" width="100%">

<br>

# `ft_malcolm` — 42

Mettre en œuvre un **ARP spoofing/poisoning** minimal : attendre une **requête ARP broadcast** du *target* pour l’IP *source*, envoyer **un unique ARP Reply** falsifié, puis **quitter proprement**. Objectif pédagogique : comprendre la couche 2 (Ethernet) / ARP (RFC 826) et le *Man‑in‑the‑Middle* (MITM) de base.

> **Disclaimer** — Ne spoofer **que** des IP qui vous appartiennent (VM locale, labo isolé). Toute utilisation hors cadre pédagogique peut être illégale.

<br>

## Sommaire
- [Objectifs du projet](#objectifs-du-projet)
- [Installation & environnement](#installation--environnement)
- [Lancement rapide (labo Docker)](#lancement-rapide-labo-docker)
- [Fonctionnalités principales](#fonctionnalités-principales)
- [Bonus implémentés](#bonus-implémentés)
- [Détails d’implémentation](#détails-dimplémentation)
- [Architecture du repo](#architecture-du-repo)
- [Choix techniques & limites](#choix-techniques--limites)
- [Évaluation](#évaluation)
- [Liens utiles](#liens-utiles)
- [Grade](#grade)

<br>

## Objectifs du projet

- Implémenter un **programme C** nommé `ft_malcolm` qui :
  - gère **uniquement IPv4** (partie obligatoire) ;
  - prend **4 arguments obligatoires** *dans cet ordre* :  
    `source_ip source_mac target_ip target_mac` ;
  - **attend** une requête ARP **broadcast** émise par *target* pour l’**IP source**, puis **envoie un seul ARP Reply** (**spoofé**) à *target* et **sort** ;
  - réagit aux **signaux** (Ctrl+C) et nettoie correctement les ressources.
- Respecter les **règles de code** 42 : Makefile, gestion d’erreurs robuste, pas de crash.
- Comprendre **ARP** (RFC 826) et son rôle dans **OSI Layer 2** ; connaître ses failles.

<br>

## Installation & environnement

- **VM Linux** (kernel **> 3.14** recommandé).  
- Compilation standard avec **Makefile**.  
- **Droits root** requis (raw sockets).  
- Fonctions autorisées :  
  `sendto`, `recvfrom`,  
  `socket`, `setsockopt`,  
  `inet_pton`, `inet_ntop`, `inet_addr`,  
  `if_nametoindex`, `getifaddrs`, `freeifaddrs`,  
  `gethostbyname`, `getaddrinfo`, `freeaddrinfo`,  
  `getuid`, `close`,  
  `sigaction`, `signal`,  
  `sleep`,  
  `htons`, `ntohs`,  
  `strerror`, `gai_strerror`,  
  `printf` et sa famille  
  **`libft`** autorisée

<br>

## Lancement rapide (labo Docker)

> Au mieux, **ouvrir 3 terminaux** (bystander / attacker / victim) pour visualiser l’attaque ARP dans un réseau isolé.

### Terminal 1 — **bystander** : démarrer & sniffer
```bash
docker-compose up -d --build
docker exec -it bystander bash
tcpdump -i eth0 -n -e arp
# On observe les requêtes ARP en broadcast de victim
```

### Terminal 2 — **attacker** : compiler & lancer
```bash
docker exec -it attacker bash
make
# options
make run    # ./ft_malcolm 10.10.0.3 02:42:0a:0a:00:07 10.10.0.5 02:42:0a:0a:00:05
./ft_malcolm <IP_SOURCE> <MAC_SOURCE> <IP_TARGET> <MAC_TARGET>
make leaks  # valgrind --leak-check=full --show-leak-kinds=all ./ft_malcolm ...
```

### Terminal 3 — **victim** : nettoyer & pinger
```bash
docker exec -it victim bash
ip neigh flush all
ip neigh show
ping -c 1 10.10.0.3
ip neigh show
# Sortie attendue : l’IP source (10.10.0.3) associée à la MAC bystander (02:42:0a:0a:00:07)
```

<br>

## Fonctionnalités principales

- **Sniff ARP** : écoute des trames `Ethernet + ARP` via **raw socket** `AF_PACKET` lié à l’interface de `source_ip`.
- **Validation stricte** de la requête ARP reçue :  
  - trame **ARP** (`ethertype = ARP`, `op = request`) ;  
  - **dest MAC = broadcast** ;  
  - `tpa == source_ip` et `spa == target_ip` ;  
  - **SHA** (MAC émetteur) == `target_mac`.
- **Sélection d’interface** automatique à partir de `source_ip` (`getifaddrs`, `if_nametoindex`), récupération **MAC** locale.
- **Forge & envoi** d’un **ARP Reply** (Ethernet + ARP) vers `target_mac` :  
  `sha = source_mac`, `spa = source_ip`, `tha = target_mac`, `tpa = target_ip`.
- **Nettoyage** robuste & gestion **SIGINT/SIGTERM/SIGQUIT/SIGHUP**.
- **Logs** lisibles & **mode verbeux** (macro `DEBUG`), *dump* **hex** du paquet envoyé/reçu.
- **Valgrind** prêt à l’emploi (`make leaks`).

<br>

## Bonus implémentés

- Mode **verbeux** activable par macro `DEBUG` (messages colorés, *dump* brut des paquets)  

<br>

## Détails d’implémentation

- **Structures** (`includes/structs.h`) : `t_args`, `t_interface`, `t_request`, `t_data`.  
  - `t_args` stocke les 4 paramètres (**strings + binaires**) et un flag `validated`.  
  - `t_interface` : nom, index, **MAC locale**, **IP locale**, `sockaddr_ll`, `socket_fd`.  
  - `t_request` : champs de la **requête ARP** capturée (SHA, SPA, broadcast, timestamp…).
- **Initialisation** (`sources/initializer.c`) :  
  - parsing **MAC** → 6 octets ; parsing **IPv4** (`inet_pton`) ;  
  - **is_local_ip** vérifie que `source_ip` appartient à une **interface locale** ;  
  - **iface_from_ip** retrouve l’interface liée à `source_ip` (+ MAC locale) ;  
  - **open_iface_socket** : `socket(AF_PACKET, SOCK_RAW, ETH_P_ARP)` + `bind` sur `ifindex`.
- **Capture ARP** (`sources/comm.c`) :  
  - vérifie `eth->h_proto == ARP`, `arp->ar_op == REQUEST` ;  
  - lit **SHA/SPA/THA/TPA** et contrôle les correspondances avec `args` ;  
  - *debug* optionnel : **dump hex** de la trame reçue.
- **Réponse ARP** (`sources/comm.c`) :  
  - construction *packed* `{ ethhdr, arphdr, sha, spa, tha, tpa }` ;  
  - envoi via `sendto` sur `sockaddr_ll` du **target** ;  
  - *debug* : **dump hex** de la trame envoyée.
- **Erreurs & signaux** : `check.c` centralise les **OK/KO** colorés ; `signals.c` pose les handlers ; `cleanup.c` ferme le socket.
- **Mode DEBUG** : `#define DEBUG 1` (activer/désactiver la verbosité).

<br>

## Architecture du repo
```
.
├─ includes/
│  ├─ colors.h
│  ├─ ft_malcolm.h
│  └─ structs.h
├─ sources/
│  ├─ check.c
│  ├─ cleanup.c
│  ├─ comm.c
│  ├─ debug.c
│  ├─ initializer.c
│  ├─ interface.c
│  ├─ main.c
│  └─ signals.c
├─ libft/                 # votre libft (liée au projet)
├─ docker-compose.yml     # labo réseau (attacker / victim / bystander)
├─ vm-attacker/           # Dockerfile VM attacker (ft_malcolm)
├─ vm-victim/             # Dockerfile VM victim (ARP request)
├─ vm-bystander/          # Dockerfile VM bystander (tcpdump)
├─ Makefile
└─ utils/
   └─ banner.png
```
> **Note** : le labo Docker n’est utile que pour la démo/évaluation. Le binaire fonctionne aussi **hors Docker** dans une VM Linux.

<br>

## Choix techniques & limites

- **IPv4 only** (obligatoire). Pas d’IPv6 (ND) dans cette version.  
- **Un seul ARP Reply**, puis **exit** (comportement demandé par le sujet).  
- **Linux‑only** côté émission/réception bas‑niveau (`AF_PACKET`).  
- **Root requis** (ou *capabilities* adaptées).  
- **Sécurité** : ne jamais spoofer des IP externes. Labo **isolé** uniquement.  
- **Dépend de l’ARP** : protections locales (static ARP, ARP inspection, etc.) peuvent empêcher la démonstration.

<br>

## Évaluation

- **Binaire** `ft_malcolm`   
- **4 arguments** dans l’ordre : `source_ip source_mac target_ip target_mac`   
- **Attente requête ARP broadcast** du *target* pour l’**IP source**   
- **Envoi d’un unique ARP Reply** vers *target*, puis **sortie**   
- **Gestion d’erreurs** robuste (aucun crash)   
- **SIGINT** → arrêt propre   
- **VM Linux** > 3.14   

<br>

## Liens utiles

- [RFC 826 – *An Ethernet Address Resolution Protocol*](https://datatracker.ietf.org/doc/html/rfc826)
- [RFC 7042 – *IANA/IETF Guidelines for ARP Parameters*](https://datatracker.ietf.org/doc/html/rfc7042)
- [ARP (Wikipedia)](https://fr.wikipedia.org/wiki/Address_Resolution_Protocol)
- [OSI Layer 2 (Wikipedia)](https://fr.wikipedia.org/wiki/Mod%C3%A8le_OSI#Couche_2_:_liaison_de_donn%C3%A9es)

<br>

## Grade

> En cours d’évaluation
