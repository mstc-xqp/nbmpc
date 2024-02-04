# MPC-TSS (Multi-Party Computation based Threshold Signature Scheme) in Secure Environment
This repository contains a working implementation of MPC processes designed to run inside of a [secure execution environment](https://github.com/Cypherock/x1_wallet_firmware/blob/main/docs/device_provision_auth.md). The implementation make use of the existing [Cypherock-PKI](https://github.com/Cypherock/x1_wallet_firmware/blob/main/docs/device_provision_auth.md) within the [Cypherock ecosystem](https://github.com/Cypherock/x1_wallet_firmware#cypherock_x1_wallet) ([official website](https://www.cypherock.com/)) to establish private and authentic secure channel with the parties. Further we have proposed an extension to the [BIP32 protocol](https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki) (`"Extended BIP32"`) with which after the one time initial setup any individual party would be able to generate verifiable group public keys without other parties required to come online. All the polynomials required in the signature phase are generated randomly and consumed within the signature phase itself.

The setting comprises of 3 sub-processes running independent MPC group sessions over the group of parties. These sub-process are:
1. DKG (Distributed Key Generation) <br/> Setup group account xpub
2. Extended BIP32 <br/> Generates child public keys for the group
3. Transaction signing <br/> Sign ECDSA based transactions 

**NOTE: The implementation is compatible with unix systems only as it mimics the interaction between multiple parties using local socket connections.**

**NOTE: The implementation works for a group setting for 3-of-5 MPC scheme, however it could be extended to other n-of-m schemes as well.**

## Running the project (Build)
### Dependencies (version tested)
1. gcc (version `10.3.0`)
2. cmake (version `3.19.1`)
3. ninja (version `1.10.2`)

### Build steps
Execute the following commands in the project root folder
```
mkdir -p build && cd build
cmake .. -G Ninja
ninja
./mpc <party-id>
```

### Running the group members
```
./mpc 1
./mpc 2
./mpc 3
```

## Working of PoC (Proof of Concept)
The implementation leverages standard Distributed Key Generation algorithm based on additive shares and VSS (Verifiable Secret Sharing) principle for group key generation. The signature generation process again leverages the additive shares generated by COT (Co-related Oblivious Transfer) protocol for secure product evaluations. It is important to note that the Secure Environment helps fulfill the security requirements of the base protocols/algorithms; additionally, the environment helps in further hardening of the security protocols (_not the highlight of this PoC_) over the PKI pre-established within the hardware ecosystem.

The group setting demonstrated in the PoC is 3-of-5 scheme (though it is configurable in the source) where any 3 of the 5 parties can come online to sign any transaction. Each party is mapped to a static port value for the local socket over which the parties communicate and exchange data in all the MPC processes/sub-processes.
