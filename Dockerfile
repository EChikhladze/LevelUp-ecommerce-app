FROM gcc:latest

WORKDIR /app

COPY ./ecommerceapp.cpp /app

RUN g++ -o ecommerceapp ecommerceapp.cpp

EXPOSE 8080

ENV NAME ecommerceapp

CMD ["./ecommerceapp"]