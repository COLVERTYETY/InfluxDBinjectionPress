# docker image for influxdb, ubuntu

FROM influxdb:latest


WORKDIR /WORKDIR

#  install python3
RUN apt-get update && apt-get install -y python3 python3-pip

COPY serial2influxdb.py /WORKDIR

#  copy the requirements file
COPY requirements.txt /WORKDIR

#  install the requirements
RUN python3 -m pip install -r requirements.txt

#  enable admin interface GUI on port 8083
ENV INFLUXDB_HTTP_AUTH_ENABLED=true

#  copy run.sh
COPY run.sh /WORKDIR

#  run the python script
# CMD ["python3", "serial2influxdb.py"]
CMD ["sh", "run.sh"]

