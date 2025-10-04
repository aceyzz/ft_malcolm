#ifndef STRUCTS_H
# define STRUCTS_H

# define MAC_STR_LEN 18
# define IP_STR_LEN INET_ADDRSTRLEN
# define MAC_BIN_LEN 6

typedef uint32_t ipv4;
typedef uint8_t mac_addr[MAC_BIN_LEN];

// requete ARP
typedef struct s_request {
    mac_addr	sender_mac;
    ipv4		sender_ip;
    uint8_t		eth_dst_broadcast;
    ssize_t		raw_len;
    time_t		timestamp;
}	t_request;

// args 
typedef struct s_args {
    char		ip_source_str[IP_STR_LEN];
    char		mac_source_str[MAC_STR_LEN];
    char		ip_target_str[IP_STR_LEN];
    char		mac_target_str[MAC_STR_LEN];

	ipv4		ip_source;
    ipv4		ip_target;
    mac_addr	mac_source;
    mac_addr	mac_target;
    
	bool		validated;
}	t_args;

// interface réseau pour reception et envoi
typedef struct s_interface {
    char				if_name[IFNAMSIZ];
    unsigned int		if_index;
    mac_addr			if_mac;
    ipv4				if_ip;
    int					socket_fd;
    struct sockaddr_ll	sll;
}	t_interface;

// config globale
typedef struct s_data {
    t_args		args;
    t_interface	interface;
    t_request	request;

    bool		already_sent;
    int			timeout_seconds;
    time_t		start_time;
}	t_data;

#endif
