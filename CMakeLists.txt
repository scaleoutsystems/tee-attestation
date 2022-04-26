# Project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(attestation)

# Mongo
find_package(mongoc-1.0 1.7 REQUIRED)

# Gramine
include_directories("/opt/gramine/Pal/src/host/Linux-SGX/tools/ra-tls")
link_directories("/opt/conda/envs/default/lib/x86_64-linux-gnu")

# Executable
add_executable(server "server.c")
target_link_libraries(server PRIVATE secret_prov_verify_epid)
target_link_libraries (server PRIVATE mongo::mongoc_static)