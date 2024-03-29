#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "bip32.h"
#include "config.h"
//#include "ecdsa.h"
//#include "hasher.h"
#include "mpc.h"
//#include "mpc_helpers.h"
//#include "mpc_utils.h"
#include "names.h"
//#include "network.h"
#include <sys/time.h>

static void valid_parties() {
    fprintf(stderr, "Possible <id> values:\n");
    for (size_t i = 0; i < N_PARTIES; i++) {
        fprintf(stderr, "%lu\n", i + 1);
    }
};

int main(int argc, char** argv) {
    MPC_STATUS status = MPC_OP_SUCCESS;

    mpc_party party;

    if (argc != 2) {
        fprintf(stderr, "format: %s <id>\n", argv[0]);
        valid_parties();
        exit(EXIT_FAILURE);
    };

    uint8_t party_id = atoi(argv[1]);

    if (party_id == 0 || party_id > N_PARTIES) {
        valid_parties();
        exit(EXIT_FAILURE);
    }

    status = mpc_init_party(&party, party_id,
                            names[(party_id - 1) % 100]);//初始化

    if (status != MPC_OP_SUCCESS)
        raise_error("MPC party initialization failed", status);

    printf("Initialised party %u\n", party.id);

    if (open_socket(party.port, &party.address, &party.server_fd) < 0) {
        fprintf(stderr, "Failed to open socket at %u", party.port);
        exit(EXIT_FAILURE);
    }

    printf("Opened socket at %u\n", party.port);

    printf(
        "\n=============\n"
        "DKG EXTENSION\n"
        "=============\n\n");
    
    struct timeval start,end; 
    
    gettimeofday(&start, NULL ); 
    status = mpc_dkg_extension(&party);//执行dkg操作
    gettimeofday(&end, NULL ); 
    long timeuse =1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;  
    printf("DKG time=%f\n",timeuse /1000000.0);  

    if (status != MPC_OP_SUCCESS)
        raise_error("MPC DKG Extension failed", status);

    if (party.id > THRESHOLD + 1) {
        printf(
            "\nParties from 1 to t + 1 inclusive will take part in the "
            "signature phase\n");
        return 0;
    }

    printf(
        "\n=============\n"
        "SIGNATURE PHASE\n"
        "=============\n\n");

    struct timeval start1,end1; 

    gettimeofday(&start1, NULL ); 
    status = mpc_signature_phase(&party);//执行签名操作
   gettimeofday(&end1, NULL ); 
    timeuse =1000000 * ( end1.tv_sec - start1.tv_sec ) + end1.tv_usec - start1.tv_usec;  
    printf("Sign time=%f\n",timeuse /1000000.0);  
   
    
    if (status != MPC_OP_SUCCESS)
        raise_error("MPC Signature phase failed", status);

    return 0;
}
