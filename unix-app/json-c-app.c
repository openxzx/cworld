/*
 * Json-c test application
 *
 * Compile command: gcc test.c -I/usr/include/json-c -L/usr/lib/ -ljson-c
 */

#include <stdio.h>
#include <json-c/json.h>

int main(void)
{
        const char * rstr = NULL;
        json_object * robj = NULL;

        int data1 = 10;
        int data2 = 47000000;

        robj = json_object_new_object();

        json_object_object_add(robj, "data1", json_object_new_int(data1));
        json_object_object_add(robj, "data2", json_object_new_int(data2));

        struct json_object *data3 = json_object_new_array();
        json_object_array_add(data3, json_object_new_int(1));
        json_object_array_add(data3, json_object_new_int(2));
        json_object_array_add(data3, json_object_new_int(3));
        json_object_object_add(robj, "data3", data3);

        rstr = json_object_to_json_string_ext(robj, JSON_C_TO_STRING_PLAIN);
        printf("%s\n", rstr);

        json_object_put(robj);

        return 0;
}
