#!/bin/sh
# desactive ARP sur eth0 pour eviter que le kernel reponde aux ARP
# a la place de mon prog ft_malcolm
if ip -d link show eth0 >/dev/null 2>&1; then
    ip link set dev eth0 arp off 2>/dev/null || {
        printf '[WARN] Failed to set arp off on eth0 (insufficient privileges?)\n' >&2
    }
else
    printf '[WARN] eth0 not found at entrypoint time, skipping arp off\n' >&2
fi

exec "$@"