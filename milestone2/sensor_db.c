/**
* \author Diego
 */


#include "sensor_db.h"
#include "logger.h"
#include <stdbool.h>

static bool start = false; //extra safety
static bool stop = false;


FILE * open_db(char * filename, bool append) {
    const char *mode;
    if (append) {mode = "a";}
    else {mode = "w";}

    FILE *f = fopen(filename,mode);
    if (f == NULL) {
        fprintf(stderr, "Error: could not open file in open_db\n");
        return NULL;
    }
    if (!start) {
        create_log_process();
        start = true;
    }
    write_to_log_process("Opened CSV file");
    return f;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts) {
    if( f == NULL ) {return -1;}

    int success = fprintf(f,"%u,%f,%ld\n",id,value,ts);
    if(success < 0){return -1;}
    write_to_log_process("New sensor value inserted");
    return 0;
}

int close_db(FILE * f) {
    if (f==NULL){return -1;}

    int check = fclose(f);

    if (check < 0){
        write_to_log_process("failed to close CSV file");
        if (!stop) {
            end_log_process();
            stop = true;
        }
    }
    else{
        write_to_log_process("Closed CSV file");
        if (!stop) {
            end_log_process();
            stop = true;
        }
    }
    return check;
}