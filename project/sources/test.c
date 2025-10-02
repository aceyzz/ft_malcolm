#include "ft_malcolm.h"

static inline void test_ip_args(void)
{
	const struct { const char *s; bool expect; } iptc[] = {
		{NULL, false},
		{"", false},
		{".", false},
		{"...", false},
		{"0.0.0.0", true},
		{"255.255.255.255", true},
		{"1.1.1", false},
		{"1.1.1.1.1", false},
		{"1..1.1", false},
		{"1.1.1.", false},
		{".1.1.1", false},
		{"300.1.1.1", false},
		{"1.300.1.1", false},
		{"1.1.1.256", false},
		{"01.02.03.004", false},
		{"001.002.003.004", false},
		{"1.01.1.1", false},
		{"a.b.c.d", false},
		{"1.1.a.1", false},
		{" 1.1.1.1", false},
		{"1.1.1.1 ", false},
		{"10.0.0.1", true},
		{"123.45.67.89", true}
	};
	size_t ok = 0, ko = 0;
	for (size_t i = 0; i < sizeof(iptc)/sizeof(iptc[0]); ++i)
	{
		bool r = is_valid_ip(iptc[i].s);
		if (r == iptc[i].expect) {
			printf(GRNN "[OK]" RST " is_valid_ip(\"%s\") -> %s\n", iptc[i].s ? iptc[i].s : "(null)", r ? "true" : "false"); ok++;
		} else {
			printf(REDD "[KO]" RST " is_valid_ip(\"%s\") -> %s (expected %s)\n", iptc[i].s ? iptc[i].s : "(null)", r ? "true" : "false", iptc[i].expect ? "true" : "false"); ko++;
		}
	}
	printf(BOLD "IP RESULT: " RST GRNN "ok=%zu " RST REDD "ko=%zu " RST YLLW "total=%zu\n\n" RST, ok, ko, ok+ko);
}

static inline void test_mac_args(void)
{
	const struct { const char *s; bool expect; } mactc[] = {
		{NULL, false},
		{"", false},
		{"00:11:22:33:44:55", true},
		{"12:34:56:78:90:12", true},
		{"99:99:99:99:99:99", true},
		{"0a:11:22:33:44:55", true},
		{"AA:BB:CC:DD:EE:FF", true},
		{"00:11:22:33:44", false},
		{"00:11:22:33:44:55:66", false},
		{"00:11:22:33:44:5", false},
		{"0:11:22:33:44:55", false},
		{"00-11-22-33-44-55", false},
		{"0011:2233:4455", false},
		{":00:11:22:33:44:55", false},
		{"00:11:22:33:44:55:", false},
		{"00:11:22:33::44:55", false},
		{" 00:11:22:33:44:55", false},
		{"00:11:22:33:44:5G", false},
		{"3C:5A:B4:1C:2D:3E", true},
		{"00:1a:2b:3C:4D:5e", true},
		{"F4:cE:46:12:34:56", true}
	};
	size_t ok = 0, ko = 0;
	for (size_t i = 0; i < sizeof(mactc)/sizeof(mactc[0]); ++i)
	{
		bool r = is_valid_mac(mactc[i].s);
		if (r == mactc[i].expect) {
			printf(GRNN "[OK]" RST " is_valid_mac(\"%s\") -> %s\n", mactc[i].s ? mactc[i].s : "(null)", r ? "true" : "false"); ok++;
		} else {
			printf(REDD "[KO]" RST " is_valid_mac(\"%s\") -> %s (expected %s)\n", mactc[i].s ? mactc[i].s : "(null)", r ? "true" : "false", mactc[i].expect ? "true" : "false"); ko++;
		}
	}
	printf(BOLD "MAC RESULT: " RST GRNN "ok=%zu " RST REDD "ko=%zu " RST YLLW "total=%zu\n\n" RST, ok, ko, ok+ko);
}

void test_args(void)
{
	test_ip_args();
	test_mac_args();
}
