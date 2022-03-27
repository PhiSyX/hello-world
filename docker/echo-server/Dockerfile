FROM golang:1-alpine as build

workdir /app

COPY docker/ src/
RUN go build src/docker.go

FROM alpine:latest
WORKDIR /app
COPY --from=build /app/docker /app/docker

EXPOSE 8080
CMD ["./docker"]
