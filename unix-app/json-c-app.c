/*
 * Json-c test application
 *
 * Compile command: gcc test.c -I/usr/include/json-c -L/usr/lib/ -ljson-c
 */

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define CNT     1
void fetch(int *buf, json_object * data_array)
{
    int i;

    for (i = 0; i < 3; i++)
        json_object_array_add(data_array, json_object_new_int(buf[i]));
}

int main(void)
{
    int i;
    const char * str = NULL;
    int buf[3];
    json_object * dutycycle_obj = NULL;
    json_object * chanusage_array = NULL;
    //json_object * data_array[CNT] = { NULL };
    //json_object * data_obj[CNT] = { NULL} ;
    json_object * data_array = NULL;
    json_object * data_obj = NULL;

    memset(buf, 0, sizeof(buf));

    dutycycle_obj = json_object_new_object();

    json_object_object_add(dutycycle_obj, "active_node", json_object_new_int(10));
    json_object_object_add(dutycycle_obj, "unusual_node", json_object_new_int(0));
    json_object_object_add(dutycycle_obj, "received", json_object_new_int(1024));
    json_object_object_add(dutycycle_obj, "transmitted", json_object_new_int(1024));

    chanusage_array = json_object_new_array();
    //for (i = 0; i < CNT; i++) {
    //    data_array[i] = json_object_new_array();
    //    data_obj[i] = json_object_new_object();
    //}

    data_array = json_object_new_array();
    data_obj = json_object_new_object();

    for (i = 0; i < CNT; i++) {
        //json_object_object_add(data_obj[i], "freq", json_object_new_int(470));
        //fetch(buf, data_array[i]);
        //json_object_object_add(data_obj[i], "data", data_array[i]);
        //json_object_object_add(data_obj[i], "endtime", json_object_new_int(100));
        //json_object_object_add(data_obj[i], "step", json_object_new_int(10));
        //json_object_array_add(chanusage_array, data_obj[i]);

        json_object_object_add(data_obj, "freq", json_object_new_int(470));
        fetch(buf, data_array);
        json_object_object_add(data_obj, "data", data_array);
        json_object_object_add(data_obj, "endtime", json_object_new_int(100));
        json_object_object_add(data_obj, "step", json_object_new_int(10));
        json_object_array_add(chanusage_array, data_obj);

    }
    printf("success\n");
    json_object_object_add(dutycycle_obj, "ChanUsage", chanusage_array);
    printf("step\n");

    str = json_object_to_json_string_ext(dutycycle_obj, JSON_C_TO_STRING_PLAIN);
    printf("step\n");
    if (NULL == str) {
        printf("string for NULL\n");
        return 0;
    }
    json_object_put(data_array);
    json_object_put(data_obj);
    printf("%s\n", str);
    json_object_put(chanusage_array);
    json_object_put(dutycycle_obj);
    //for (i = 0; i < CNT; i++) {
    //    json_object_put(data_array[i]);
    //    json_object_put(data_obj[i]);
    //}

    return 0;
}
