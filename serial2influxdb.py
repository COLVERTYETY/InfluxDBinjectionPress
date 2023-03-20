#  this script reads the serial input from the arduino and writes it to influxdb

import serial
import time
import datetime
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS

# influxdb settings
host = "0.0.0.0"
port = 8086
bucket = "press"
org = "matthis"
my_token = "3M-FgGlKNYodFZmELe7f0EWuZNdY69uN_X2xgeT9yg9wGfDLzQW-jkG0d1Y89EzkNVRppWqC9tZSK8pWSWBvRw=="
while True:
    try:
        #  helloworld
        print("Hello from the docker container")

        # serial settings
        ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

        # influxdb client
        client = influxdb_client.InfluxDBClient(url = f'{host}:{port}', token=my_token, org=org, bucket=bucket)
        write_api = client.write_api(write_options=SYNCHRONOUS)
        #  send a hello to the influxdb
        # client.write_points([{"measurement": "hello", "tags": {"verison": "1.0"}, "time": datetime.datetime.utcnow(), "fields": {"value": 1}}])


        # main loop
        while True:
            # read serial data
            line = ser.readline()
            # split the data
            data = line.decode("utf-8").split(" ")
            # data = line.split(" ")
            if len(data) < 3:
                continue
            #   handle nan
            if data[-3] == "nan":
                data[-3] = 0
            if data[-2] == "nan":
                data[-2] = 0
            target = float(data[-3])
            current = float(data[-2])
            # print the data
            print("FOUND", target, current)
            # create the json data
            json_body = [
                {
                    "measurement": "arduino",
                    "tags": {
                        "verison": "1.0"
                    },
                    "time": datetime.datetime.utcnow(),
                    "fields": {
                        "target_temp": target,
                        "current_temp": current,
                    }
                }
            ]
            # write the data to influxdb
            print(json_body)
            # client.write_points(json_body)
            write_api.write(bucket, org, influxdb_client.Point.from_dict(json_body[0]))
            # wait a second
            # time.sleep(0.1)
    except Exception as e:
        print("Error", e)
        time.sleep(1)