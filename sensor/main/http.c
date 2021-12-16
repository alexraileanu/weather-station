#include "http.h"

#define HOST CONFIG_HTTP_LOG_HOST
#define ENDPOINT CONFIG_HTTP_LOG_ENDPOINT

esp_http_client_handle_t get_client() {
    esp_http_client_config_t cfg = {
            .host = HOST,
            .path = "/log",
    };
    return esp_http_client_init(&cfg);
}

void post_log(char *msg) {
    esp_http_client_handle_t client = get_client();

    size_t length = (size_t) snprintf(NULL, 0, "{\"msg\": \"%s\"}", msg);
    char *post_data = malloc(length + 1);
    snprintf(post_data, length + 1, "{\"msg\": \"%s\"}", msg);

    esp_http_client_set_url(client, ENDPOINT);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_perform(client);

    esp_http_client_cleanup(client);
    free(post_data);
}