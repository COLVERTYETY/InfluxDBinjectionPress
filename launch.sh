echo "Starting docker"
docker run -p 8086:8086 -v influxdata:/var/lib/influxdb2 -v influxetc:/etc/influxdb2 --privileged -v /dev:/dev  matthis:2