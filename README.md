# Intel SGX Attestation via Gramine
In this proof of concept we securely provision FEDn client and combiner tokens with [Gramine](https://github.com/gramineproject/gramine). The `docker-compose.yalm` file simulates a scenario in which the reducer runs in a trusted environment while the combiner and clients run on an untrusted environment with Intel SGX support. A provisioning server (`sever.c`) makes use of [EPID-based remote attestation](https://gramine.readthedocs.io/en/v1.1/attestation.html#remote-attestation-flows-for-epid-and-dcap) to verify the authenticity of the provinioned enclave and passes the token using an encrypted connection.

## Table of Contents
- [Intel SGX Attestation via Gramine](#intel-sgx-attestation-via-gramine)
  - [Table of Contents](#table-of-contents)
  - [Running the PoC](#running-the-poc)
  - [Known issues](#known-issues)

## Running the PoC

First you need to set up EPID credentials in the `.env` file as it follows.

```bash
RA_TLS_EPID_API_KEY=<your-primary-key>
RA_CLIENT_SPID=<your-spid>
```

> To get these credentials you need to create an account at: https://api.portal.trustedservices.intel.com/EPID-attestation.

Once you are done you can spin up the PoC by running:

```bash
docker-compose up -d
```

To check that evrything has worked as expected you can upload the dummy compute package and seed model that we provide under `resources/`.

```bash
# Upload package
curl -k -X POST \
    -F file=@resources/package.tar.gz \
    -F helper="keras" \
    https://localhost:8090/context

# Upload seed
curl -k -X POST \
    -F seed=@resources/seed.npz \
    https://localhost:8090/models
```

Now navigate to `https://localhost:8090` and verify that combiner and clients have connected successfully.

## Known issues
  - The the PoC is not setup to run the compute package in the enclave.
  - All the services run on `localhost` with the Docker containers attaching straight to the host network.