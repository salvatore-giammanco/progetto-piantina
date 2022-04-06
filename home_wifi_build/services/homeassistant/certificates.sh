#!/bin/bash
sudo su
openssl genrsa -des3 -out ./mosquitto/certs/ca.key 2048
openssl req -new -x509 -days 1826 -key ./mosquitto/certs/ca.key -out ./mosquitto/certs/ca.crt
openssl genrsa -out ./mosquitto/certs/server.key 2048
openssl req -new -out ./mosquitto/certs/server.csr -key ./mosquitto/certs/server.key
openssl x509 -req -in ./mosquitto/certs/server.csr -CA ./mosquitto/certs/ca.crt -CAkey ./mosquitto/certs/ca.key -CAcreateserial -out ./mosquitto/certs/server.crt -days 360
cp ./mosquitto/certs/ca.crt ca.crt
