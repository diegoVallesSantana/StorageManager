/**
* \author Diego: AI was used to write the test code in main()
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include "sensor_db.h"
#include "logger.h"
#include "config.h"

/*
int main()
{
    FILE *f = open_db("sensor_db.csv", true);

    sleep(1);
    sensor_id_t id = 1;
    sensor_value_t v = 0.001;
    sensor_ts_t ts = time(NULL);
    insert_sensor(f, id, v, ts);

    id = 2;
    v = 0.002;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);
    id = 3;
    v = 0.003;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);
    sleep(5);
    insert_sensor(f, 4, v, ts);

    close_db(f);
    return 0;
}
*/
int main()
{
    printf("=== TEST 1: basic open/insert/close ===\n");
    FILE *f = open_db("sensor_db.csv", true);

    sensor_id_t id;
    sensor_value_t v;
    sensor_ts_t ts;

    sleep(1);

    // 3 basic inserts (slow)
    id = 1; v = 0.1; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    sleep(1);

    id = 2; v = 0.2; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    sleep(1);

    id = 3; v = 0.3; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    sleep(1);

    printf("=== TEST 2: fast consecutive writes (tests message splitting) ===\n");

    for (int i = 4; i <= 10; i++) {
        id = i;
        v = i * 0.1;
        ts = time(NULL);
        insert_sensor(f, id, v, ts);
        // No sleep here on purpose — tests robustness!
    }

    printf("=== TEST 3: random inserts ===\n");

    srand(time(NULL));
    for (int i = 11; i <= 20; i++) {
        id = i;
        v = (rand() % 1000) / 1000.0; // random 0.000–0.999
        ts = time(NULL);
        insert_sensor(f, id, v, ts);
        usleep((rand() % 500) * 1000); // random 0–0.5 seconds
    }

    printf("=== TEST 4: large message burst ===\n");

    for (int i = 21; i <= 30; i++) {
        id = i;
        v = i * 0.01;
        ts = time(NULL);
        insert_sensor(f, id, v, ts);
    }

    printf("=== TEST 5: closing DB and reopening ===\n");
    close_db(f);

    sleep(1); // give logger some time

    f = open_db("sensor_db.csv", true);

    id = 31; v = 3.1; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    id = 32; v = 3.2; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    printf("=== TEST 6: testing error behavior (NULL file) ===\n");

    insert_sensor(NULL, 999, 123.456, time(NULL)); // expected to return -1

    printf("=== TEST 7: final inserts + shutdown ===\n");

    id = 33; v = 3.3; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    id = 34; v = 3.4; ts = time(NULL);
    insert_sensor(f, id, v, ts);

    close_db(f);  // clean shutdown of logger

    printf("=== All tests finished ===\n");
    return 0;
}