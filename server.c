#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stddef.h>
#include <string.h>

#include "secret_prov.h"

int main(int argc, char **argv) {
  // Init mongo driver
  mongoc_init();

  // Connect
  const char *uri_string =
      argv[1];  // e.g "mongodb://fedn_admin:password@localhost:6534"

  fprintf(stderr, "Connecting to %s ...\n", uri_string);
  bson_error_t error;
  mongoc_uri_t *uri = mongoc_uri_new_with_error(uri_string, &error);
  if (!uri) {
    fprintf(stderr,
            "failed to parse URI: %s\n"
            "error message:       %s\n",
            uri_string, error.message);
    return EXIT_FAILURE;
  }
  fprintf(stderr, "Connected to %s\n", uri_string);

  mongoc_client_t *client = mongoc_client_new_from_uri(uri);
  if (!client) {
    return EXIT_FAILURE;
  }
  mongoc_client_set_appname(client, "server");

  // Look for network.reducer collection
  mongoc_collection_t *collection = mongoc_client_get_collection(
      client, "fedn-test-network", "network.reducer");
  bson_t *query = bson_new();
  mongoc_cursor_t *cursor =
      mongoc_collection_find_with_opts(collection, query, NULL, NULL);
  const bson_t *doc;
  while (!mongoc_cursor_next(cursor, &doc)) {  // wait for FEDn to write token
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    fprintf(stderr,
            "fedn-test-network[\"network.reducer\"] document not found. "
            "Sleeping ...\n");
    sleep(5);
  }

  // Get token
  bson_iter_t iter;
  bson_iter_t token_iter;
  const char *token;
  if (bson_iter_init(&iter, doc) &&
      bson_iter_find_descendant(&iter, "token", &token_iter) &&
      BSON_ITER_HOLDS_UTF8(&token_iter)) {
    token = bson_iter_utf8(&token_iter, NULL);
  } else {
    return EXIT_FAILURE;
  }

  // Start server
  fprintf(stderr, "Starting provisioning server (token: %s)\n", token);
  int successs = secret_provision_start_server(
      (uint8_t *)token, strlen(token) + 1, "4433",
      "/app/certs/server2-sha256.crt", "/app/certs/server2.key", NULL, NULL);
  if (successs) {
    fprintf(stderr, "Provisioning server failed.\n");
    return EXIT_FAILURE;
  } else {
    return 0;
  }
}