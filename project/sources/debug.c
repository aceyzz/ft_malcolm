#include "ft_malcolm.h"

void	debug_print_args(t_args *args)
{
	printf(LIME "[DEBUG] " RST "Args structure:\n");
	printf(BLUE "  ↳ ip_source_str : " RST "%s\n", args->ip_source_str);
	printf(BLUE "  ↳ mac_source_str : " RST "%s\n", args->mac_source_str);
	printf(BLUE "  ↳ ip_target_str : " RST "%s\n", args->ip_target_str);
	printf(BLUE "  ↳ mac_target_str : " RST "%s\n", args->mac_target_str);
	char ip_buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &args->ip_source, ip_buf, sizeof(ip_buf));
	printf(BLUE "  ↳ ip_source : " RST "%s\n", ip_buf);
	printf(BLUE "  ↳ mac_source : " RST "%02x:%02x:%02x:%02x:%02x:%02x\n",
		args->mac_source[0], args->mac_source[1], args->mac_source[2],
		args->mac_source[3], args->mac_source[4], args->mac_source[5]);
	inet_ntop(AF_INET, &args->ip_target, ip_buf, sizeof(ip_buf));
	printf(BLUE "  ↳ ip_target : " RST "%s\n", ip_buf);
	printf(BLUE "  ↳ mac_target : " RST "%02x:%02x:%02x:%02x:%02x:%02x\n",
		args->mac_target[0], args->mac_target[1], args->mac_target[2],
		args->mac_target[3], args->mac_target[4], args->mac_target[5]);
}

void	debug_print_interface(t_interface *iface)
{
	printf(LIME "[DEBUG] " RST "Interface structure:\n");
	printf(EMRD "  ↳ if_name : " RST "%s\n", iface->if_name);
	printf(EMRD "  ↳ if_index : " RST "%u\n", iface->if_index);
	printf(EMRD "  ↳ if_mac : " RST "%02x:%02x:%02x:%02x:%02x:%02x\n",
		iface->if_mac[0], iface->if_mac[1], iface->if_mac[2],
		iface->if_mac[3], iface->if_mac[4], iface->if_mac[5]);
	char if_ip_buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &iface->if_ip, if_ip_buf, sizeof(if_ip_buf));
	printf(EMRD "  ↳ if_ip : " RST "%s\n", if_ip_buf);
	printf(EMRD "  ↳ socket_fd : " RST "%d\n", iface->socket_fd);
}

void	debug_print_request(t_request *req)
{
	printf(LIME "[DEBUG] " RST "Request structure:\n");
	printf(PURP "  ↳ sender_mac : " RST "%02x:%02x:%02x:%02x:%02x:%02x\n",
		req->sender_mac[0], req->sender_mac[1], req->sender_mac[2],
		req->sender_mac[3], req->sender_mac[4], req->sender_mac[5]);
	char sender_ip_buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &req->sender_ip, sender_ip_buf, sizeof(sender_ip_buf));
	printf(PURP "  ↳ sender_ip : " RST "%s\n", sender_ip_buf);
	printf(PURP "  ↳ eth_dst_broadcast : " RST "%u\n", req->eth_dst_broadcast);
	printf(PURP "  ↳ raw_len : " RST "%zd\n", req->raw_len);
	printf(PURP "  ↳ timestamp : " RST "%ld\n", req->timestamp);
}

void	debug_print_data(t_data *data)
{
	debug_print_args(&data->args);
	debug_print_interface(&data->interface);
	debug_print_request(&data->request);
	printf(LIME "[DEBUG] " RST "Data structure:\n");
	printf(CYAN "  ↳ ARGS : " RST "%p\n", (void *)&data->args);
	printf(CYAN "  ↳ INTERFACE : " RST "%p\n", (void *)&data->interface);
	printf(CYAN "  ↳ REQUEST : " RST "%p\n", (void *)&data->request);
	printf(CYAN "  ↳ ALREADY_SENT : " RST "%s\n", data->already_sent ? "true" : "false");
	printf(CYAN "  ↳ TIMEOUT_SECONDS : " RST "%d\n", data->timeout_seconds);
	printf(CYAN "  ↳ START_TIME : " RST "%ld\n", data->start_time);
}
