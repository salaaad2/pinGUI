#ifndef PING_HDR
#define PING_HDR

#include <netinet/ip_icmp.h>
#include <iostream>

#define IP_SIZE sizeof(struct icmphdr)
#define ICMP_SIZE sizeof(struct icmphdr)
#define DATA_SIZE 56
#define PACK_SIZE ICMP_SIZE + DATA_SIZE

typedef struct s_pack {
    struct icmphdr hdr;
    char load[DATA_SIZE];
} t_pack ;

typedef struct s_reply {
    struct iphdr ip;
    struct icmphdr hdr;
    char load[DATA_SIZE];
} t_reply;

typedef struct s_time {
    long double itv, ntv, avg, lapse, min, max, mdev, total;
} t_time ;

typedef struct s_ping {
    t_time * timer;
    t_pack * pack;
    t_reply * reply;
    unsigned long sent;
    unsigned long received;
    char ipstr[16];
    char * url;
} t_ping;

int e_start(std::string & url);

#endif
