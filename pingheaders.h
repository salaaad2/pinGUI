#ifndef PING_HDR
#define PING_HDR

/*********************************/
/*   cute_ping        (  //      */
/*   structs           ( )/      */
/*   by salade         )(/       */
/*  ________________  ( /)       */
/* ()__)____________)))))   :^}  */
/*********************************/


#include <netinet/ip_icmp.h>
#include <iostream>

#define IP_SIZE sizeof(struct icmphdr)
#define ICMP_SIZE sizeof(struct icmphdr)
#define DATA_SIZE 56
#define PACK_SIZE ICMP_SIZE + DATA_SIZE
#define FULL_SIZE PACK_SIZE + IP_SIZE

#define DATA_CONT "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

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
    struct sockaddr_in * servaddr;
    unsigned long sent;
    unsigned long received;
    char ipstr[16];
    char * url;
} t_ping;

int e_start(std::string & url);

#endif
