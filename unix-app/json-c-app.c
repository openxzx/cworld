/*
 * Json-c test application
 *
 * Compile command: gcc test.c -I/usr/include/json-c -L/usr/lib/ -ljson-c
 */

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define CNT     100

void fetch(int *buf, json_object * data_array)
{
    int i;

    for (i = 0; i < CNT; i++)
        json_object_array_add(data_array, json_object_new_int(buf[i]));
}

char * json_test(void)
{
    int i;
    int buf[CNT];
    char * buff = NULL;
    const char * str = NULL;
    json_object * dutycycle_obj = NULL;
    json_object * chanusage_array = NULL;
    json_object * data_array[CNT] = { NULL };
    json_object * data_obj[CNT] = { NULL} ;

    memset(buf, 0, sizeof(buf));
    for (i = 0; i < sizeof(buf); i++)
        buf[i] = i;

    dutycycle_obj = json_object_new_object();

    json_object_object_add(dutycycle_obj, "active_node", json_object_new_int(10));
    json_object_object_add(dutycycle_obj, "unusual_node", json_object_new_int(0));
    json_object_object_add(dutycycle_obj, "received", json_object_new_int(1024));
    json_object_object_add(dutycycle_obj, "transmitted", json_object_new_int(1024));

    chanusage_array = json_object_new_array();

    for (i = 0; i < CNT; i++) {
        data_array[i] = json_object_new_array();
        data_obj[i] = json_object_new_object();

        json_object_object_add(data_obj[i], "freq", json_object_new_int(470));
        fetch(buf, data_array[i]);
        json_object_object_add(data_obj[i], "data", data_array[i]);
        json_object_object_add(data_obj[i], "endtime", json_object_new_int(100));
        json_object_object_add(data_obj[i], "step", json_object_new_int(10));
        json_object_array_add(chanusage_array, data_obj[i]);
    }
    json_object_object_add(dutycycle_obj, "ChanUsage", chanusage_array);

    str = json_object_to_json_string_ext(dutycycle_obj, JSON_C_TO_STRING_PLAIN);
    if (NULL == str) {
        printf("string for NULL\n");
        return NULL;
    }
    buff = malloc(strlen(str)+1);
    memcpy(buff, str, strlen(str)+1);

    for (i = 0; i < CNT; i++) {
        json_object_put(data_array[i]);
        json_object_put(data_obj[i]);
    }
    json_object_put(chanusage_array);
    json_object_put(dutycycle_obj);

    return buff;
}

int main(void)
{
        char * p = NULL;
        while (1) {
            p = json_test();
            printf("psizeof: %ld, pstrlen: %ld\n", sizeof(p), strlen(p));
            free(p);
            p = NULL;
        }

        return 0;
}
