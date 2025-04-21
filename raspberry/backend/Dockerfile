FROM node:latest

WORKDIR /app

COPY . .

EXPOSE 8080
EXPOSE 3000

RUN npm install

CMD ["node","/app/server.js"]