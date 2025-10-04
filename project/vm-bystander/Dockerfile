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
	&& apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root
