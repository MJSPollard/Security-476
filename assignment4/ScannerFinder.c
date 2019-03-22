#include <stdio.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

void packetHandler(unsigned char* userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet);

/*
 * Main function to find possible SYN scanning attacks.
 */
int main(int argc, char** argv) {

    /* Check command line arguments. */
    if (argc != 2) {
        printf("Bad command line arguments.\n");
        return 1;
    }

    /* Read in pcap file from command line argument. */
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_offline(argv[1], error_buffer);
    if(handle == NULL) {
        printf("pcap_open_offline() failed: %s\n", error_buffer);
        return 1;
    } else {
        printf("pcap file read in correctly.\n");
    }

    /* Loop through each packet in the capture file. */
    printf("Started packet analysis, this may take some time...\n");
    if (pcap_loop(handle, 0, packetHandler, NULL) < 0) {
        printf("pcap_loop() failed: %s\n", pcap_geterr(handle));
        return 1;
    }
    printf("Packet analysis finished.\n");

    return 0;
}


/* 
 * Callback function for pcap_loop to analyze each packet in the capture.
 */
void packetHandler(unsigned char* userData, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) {
    const struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct tcphdr* tcpHeader;
    char sourceIp[INET_ADDRSTRLEN];
    char destIp[INET_ADDRSTRLEN];
    int sourcePort, destPort;

    /* check if packet is an IP packet */
    ethernetHeader = (struct ether_header*)packet;
    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {
        ipHeader = (struct ip*)(packet + sizeof(struct ether_header));
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

        /* check if packet is a tcp packet */
        if (ipHeader->ip_p == IPPROTO_TCP) {
            tcpHeader = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            sourcePort = ntohs(tcpHeader->source);
            destPort = ntohs(tcpHeader->dest);

            /* check if syn packet */
            if(tcpHeader->syn && !tcpHeader->ack) {
                printf("syn packet\n");
                printf("src_ip = %s\n", sourceIp);
                printf("dest_ip = %s\n", destIp);
                printf("src_PORT = %d\n", sourcePort);
                printf("dest_PORT = %d\n\n", destPort);
            } 
            else if (tcpHeader->syn && tcpHeader->ack) { /*check if syn-ack packet */
                printf("syn-ack packet\n");
                printf("src_ip = %s\n", sourceIp);
                printf("dest_ip = %s\n", destIp);
                printf("src_PORT = %d\n", sourcePort);
                printf("dest_PORT = %d\n\n", destPort);
            }

            /* TODO */
            /* if the number of syn packets sent to the host is more than the amount of syn-ack
            packets sent back to the original sender, count it as a suspicious ip */
        }
    }
}