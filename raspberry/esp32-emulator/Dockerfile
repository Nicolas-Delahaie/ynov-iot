FROM alpine:latest

RUN apk add --no-cache mosquitto-clients bash

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/bin/sh", "/entrypoint.sh"]