/*
** showip.c -- modern version
*/

#include <stdio.h> // fprintf, printf
#include <stdlib.h> // free, malloc
#include <string.h> // memset
#include <arpa/inet.h>// inet_ntop
#include <netdb.h> // getaddrinfo, struct addrinfo
#include <sys/types.h> // AF_INET, SOCK_STREAM, struct sockaddr
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res, *p; // hint is shittonne of data and res returns pointer to the first LL 
    int status; // stores return value from ADDRINFO
    char ipstr[INET6_ADDRSTRLEN]; // allocates a character buffer large enough to hold largest possible IPv6 string

    if (argc != 2) {
        fprintf(stderr,"usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints); // ai_family, ai_flags {0}
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP (doesn't really matter here)

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    /*
     argv[1] hostname string
     NULL no port specified
     &hints pointer to filter struct
     &res pointer to pointer, function will write into this
     */

    printf("IP addresses for %s:\n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr; // pointer to raw binary IP
        char *ipver; // string label IPv4 IPv6

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; // cast generic sockaddr pointer to IPv4-specific structure
            addr = &(ipv4->sin_addr); // extract 32 bit IPv4 address
            ipver = "IPv4"; // labelling
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        /*
         * p->ai_family v4/v6
         * addr points to raw bytes
         * ipstr is destination buffer
         * sizeof ipstr safety bound
         */
        printf("  %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
    return 0;
}
