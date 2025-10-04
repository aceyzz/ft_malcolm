FROM debian:stable-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    iproute2 \
	iputils-ping \
	tcpdump \
	procps \
	ethtool \
	build-essential \
	make \
	vim \
	iptables \
	net-tools \
	iputils-arping \
	valgrind \
	&& apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /work

COPY ./entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

WORKDIR /work
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]