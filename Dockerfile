FROM node:18

WORKDIR /usr/src/pebble_code_server
COPY package*.json ./
RUN npm install

COPY . .

EXPOSE 8080
CMD [ "node", "index.js" ]