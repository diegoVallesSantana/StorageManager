#include "sensor_db.h"
#include <stdbool.h>

FILE * open_db(char * filename, bool append) {
    const char *mode;
    if (append) {mode = "a";}
    else {mode = "w";}

    FILE *f = fopen(filename,mode);
    if (f == NULL) {
        fprintf(stderr, "Error: could not open file in open_db\n");
        return NULL;
    }
    return f;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    if( f == NULL ) {return -1;}

    int success = fprintf(f,"%u,%f,%ld\n",id,value,ts);
    if(success < 0){return -1;}
    return 0;
}

int close_db(FILE * f) {
    if (f==NULL){return -1;}
    return fclose(f);
}