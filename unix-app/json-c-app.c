/*
 * Json-c test application
 *
 * Compile command: gcc test.c -I/usr/include/json-c -L/usr/lib/ -ljson-c
 */

#include <stdio.h>
#include <json-c/json.h>

int main(void)
{
        int i;
        const char * rstr = NULL;
        json_object * robj = NULL;

        int data1 = 10;
        int data2 = 47000000;

        struct json_object *data3 = json_object_new_array();
        robj = json_object_new_object();

        for (i = 0; i < 3; i++) {
                json_object_object_add(robj, "data1", json_object_new_int(i));
                json_object_object_add(robj, "data2", json_object_new_int(i));
                json_object_array_add(data3, robj);
        }

        rstr = json_object_to_json_string_ext(data3, JSON_C_TO_STRING_PLAIN);
        printf("%s\n", rstr);

        json_object_put(robj);
        json_object_put(data3);

        return 0;
}
