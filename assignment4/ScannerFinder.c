#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SYN 2
#define SYNACK 12
#define SUCCESS 0
#define FAILURE 1

/* nodes for linked-list of potential suspicious IPs*/
struct ip_info {
    char* ip;
    int syn_sent;
    int syn_ack_recv;
    struct ip_info* next;
};
struct ip_info *start = NULL;

/* push nodes onto front on linked list */
void push(struct ip_info** head, char* ip_in) {
    int i; 
    struct ip_info* new_ip = (struct ip_info*)malloc(sizeof(struct ip_info));
    new_ip->ip  = malloc(strlen(ip_in));
    new_ip->next = (*head);
    for (i = 0; i < strlen(ip_in); i++) {
        *(char *)(new_ip->ip + i) = *(char *)(ip_in + i);
    }
    (*head) = new_ip;
}

/* search linked list for certain nodes */
int search(struct ip_info* head, char* ip_in, int packetType) 
{
    struct ip_info* current = head;
    while (current != NULL) {
        if (strcmp(current->ip, ip_in) == 0) {
            if (packetType == SYN) {
                current->syn_sent++;
            } else if (packetType == SYNACK) {
                current->syn_ack_recv++;
            }
            return SUCCESS;
        }
        current = current->next; 
    }
    return FAILURE; 
}

void printList(struct ip_info *node) {
    while (node != NULL) {
        if (node->syn_sent > node->syn_ack_recv) {
            printf("ip = %s\n", node->ip);
        } else {
            if(strcmp(node->ip, "128.3.164.249") == 0) {
                // printf("syn = %d\n", node->syn_sent);
                // printf("synack = %d\n\n", node->syn_ack_recv);
            }
            // printf("syn = %d\n", node->syn_sent);
            // printf("synack = %d\n\n", node->syn_ack_recv);
        }
        node = node->next; 
    }
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

            /* check if syn packet or syn-ack packet */
            if(tcpHeader->syn && !tcpHeader->ack) {
                printf("syn packet\n");
                printf("src_ip = %s\n", sourceIp);
                printf("dest_ip = %s\n", destIp);
                printf("src_PORT = %d\n", sourcePort);
                printf("dest_PORT = %d\n\n", destPort);
                if(search(start, sourceIp, SYN) == FAILURE) {
                    push(&start, sourceIp);
                }
            } else if (tcpHeader->syn && tcpHeader->ack) {
                printf("syn-ack packet\n");
                printf("src_ip = %s\n", sourceIp);
                printf("dest_ip = %s\n", destIp);
                printf("src_PORT = %d\n", sourcePort);
                printf("dest_PORT = %d\n\n", destPort);
                if(search(start, destIp, SYNACK) == FAILURE) {
                    push(&start, destIp);
                }
            }
        }
    }
}

/*
 * Main function to find possible SYN scanning attacks.
 */
int main(int argc, char** argv) {

    /* Check command line arguments. */
    if (argc != 2) {
        printf("Bad command line arguments.\n");
        return FAILURE;
    }

    /* Read in pcap file from command line argument. */
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_offline(argv[1], error_buffer);
    if(handle == NULL) {
        printf("pcap_open_offline() failed: %s\n", error_buffer);
        return FAILURE;
    } else {
        printf("pcap file read in correctly.\n");
    }

    /* Loop through each packet in the capture file. */
    printf("Started packet analysis, this may take some time...\n");
    if (pcap_loop(handle, 0, packetHandler, NULL) < 0) {
        printf("pcap_loop() failed: %s\n", pcap_geterr(handle));
        return FAILURE;
    }
    printf("Packet analysis finished.\n");
    printList(start);

    return SUCCESS;
}