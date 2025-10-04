Check leaks avec Valgrind : 

```bash
sudo -E valgrind --leak-check=full --show-leak-kinds=all ./ft_malcolm ...
```


IP VMs
- Debian 13.1 main : 192.168.64.3 4e:64:05:a8:bf:68
- Debian 13.1 bis : 192.168.64.5 a2:4c:49:b4:fd:0e

Flush ARP cache :
```bash
sudo ip -s -s neigh flush all
```

Lister les entrées ARP :
```bash
ip neigh show
```

Monitorer les requetes ARP :
```bash
sudo tcpdump -i enp0s1 -n -e arp
```