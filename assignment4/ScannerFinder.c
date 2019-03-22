#include <stdio.h>
#include <pcap.h>

int main(void) {
    printf("hello world\n");
    char error_buffer[PCAP_ERRBUF_SIZE];

    /* read in file */
    pcap_t *handle = pcap_open_offline("lbl-internal.20041004-1305.port002.dump.pcap", error_buffer);


    return 0;
}