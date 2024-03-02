// Written by 0@monero.church - XMPP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define HORIZONTAL_LINE "----------------------------------------------------------------------"
#define VERTICAL_LINE "|"

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

void print_line_with_borders(const char *label, const char *value) {
    printf(VERTICAL_LINE " %-20s: %-44s " VERTICAL_LINE "\n", label, value);
}

void print_horizontal_border() {
    printf("%s\n", HORIZONTAL_LINE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ip_address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing curl\n");
        return EXIT_FAILURE;
    }

    char api_url[] = "https://ipapi.co/";
    char url_with_ip[strlen(api_url) + strlen(argv[1]) + 6]; 
    sprintf(url_with_ip, "%s%s/json/", api_url, argv[1]);

    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL, url_with_ip);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "iplookup-agent/1.0");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        struct json_object *parsed_json, *field;
        parsed_json = json_tokener_parse(s.ptr);

        print_horizontal_border();

        if (json_object_object_get_ex(parsed_json, "ip", &field)) {
            print_line_with_borders("IP", json_object_get_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "city", &field)) {
            print_line_with_borders("City", json_object_get_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "region", &field)) {
            print_line_with_borders("Region", json_object_get_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "country_name", &field)) {
            print_line_with_borders("Country", json_object_get_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "postal", &field)) {
            print_line_with_borders("Postal Code", json_object_get_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "latitude", &field)) {
            print_line_with_borders("Latitude", json_object_to_json_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "longitude", &field)) {
            print_line_with_borders("Longitude", json_object_to_json_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "asn", &field)) {
            print_line_with_borders("ASN", json_object_get_string(field));
        }
        if (json_object_object_get_ex(parsed_json, "org", &field)) {
            print_line_with_borders("Organization", json_object_get_string(field));
        }

        print_horizontal_border();

        json_object_put(parsed_json);
    }

    free(s.ptr);
    curl_easy_cleanup(curl);

    return res == CURLE_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}


