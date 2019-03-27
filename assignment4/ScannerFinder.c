#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* 
 * Assignment 4 for CSCI 476, Computer Security
 * Montana State University, Spring 2019
 * @Authors, Michael Pollard, Austin Johnson, Matthew Sagen
 */

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

/* 
 * This function pushes nodes onto the front of the linked list.
 */
void push(struct ip_info** head, char* ip_in, int packetType) {
    int i;
    struct ip_info* new_ip = (struct ip_info*)malloc(sizeof(struct ip_info));
    /* Add struct members to Nodes */
    new_ip->ip  = malloc(strlen(ip_in));
    if (packetType == SYN) {
        new_ip->syn_sent++;
    } else if (packetType == SYNACK) {
        new_ip->syn_ack_recv++;
    }
    new_ip->next = (*head);
    for (i = 0; i < strlen(ip_in); i++) {
        *(char *)(new_ip->ip + i) = *(char *)(ip_in + i);
    }
    (*head) = new_ip;
}

/*
 * This function searches the linked list for certain nodes.
 */
int search(struct ip_info* head, char* ip_in, int packetType) 
{
    struct ip_info* current = head;
    /* Loop through list and increments counts according to packetType */
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

/*
 * This function prints out the list of IP's that sent more SYN's than
 * received SYN'ACK's.
 */
void printList(struct ip_info *node) {
    printf("\nIP's SUSPECTED OF PORT SCANNING\n");
    printf("----------------\n");
    while (node != NULL) {
        if (node->syn_sent > (3 * node->syn_ack_recv)) {
            printf("%s\n", node->ip);
        }
        node = node->next; 
    }
    printf("----------------\n");
}

/* 
 * Callback function for pcap_loop to analyze each packet in the capture and
 * extract needed data.
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
                if(search(start, sourceIp, SYN) == FAILURE) {
                    push(&start, sourceIp, SYN);
                }
            } else if (tcpHeader->syn && tcpHeader->ack) {
                if(search(start, destIp, SYNACK) == FAILURE) {
                    push(&start, destIp, SYNACK);
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
        printf(".pcap file read in correctly.\n");
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