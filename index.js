//Standard importe
const express = require("express");
//randomString
const { randomUUID } = require("crypto");
//bodyparser macht aus string json
const bodyParser = require("body-parser");
//terminalbefehle können in js ausgeführt werden bei aufruf
const { execSync } = require("child_process");
//filesystem datei ordner etc erstellen
const fs = require("fs");

//Express server init
const app = express();
//cors ermöglich mir domain übergreifend zu arbeiten
const cors = require("cors");

//wird dem server hinzugefügt
app.use(bodyParser.json());
app.use(cors());

app.post("/run", function (req, res) {
  //zufälliger ordner
  const folder = randomUUID();
  fs.mkdirSync(folder);

  //hole code aus website und schreibe es in code
  const code = req.body.code;
  //schreibt code aus website in die datei main.cpp imn random ordner
  fs.writeFileSync(`${folder}/main.cpp`, code);

  //gleich für labyrinth code
  const labyrinth = req.body.labyrinth;
  fs.writeFileSync(`${folder}/labyrinth.json`, JSON.stringify(labyrinth));

  //erstelle Dockerfile jede instanz neu
  const Dockerfile = `# Choose a base image that supports C++
FROM gcc:latest

# Copy the C++ executable to the container
ADD . / 

# Set the working directory
WORKDIR /

RUN chmod +x run.sh

# Set the entry point to run the executable
CMD ["./run.sh"]`;

  const RunScript = `#!/bin/bash
#2>&1 nimmt auch Fehlermeldungen mit
g++ main.cpp turtle.cpp -o Turtleprogramm -std=c++17 > compile.txt 2>&1
compile=$(cat compile.txt)

./Turtleprogramm > turtleprogramm.txt 2>&1
output=$(cat turtleprogramm.txt)

path=$(cat kroeten_path.json)

echo $compile
echo --------
echo $output
echo --------
echo $path
`;
  //kopiere file in anderen ordner
  fs.copyFileSync("import_turtle/turtle.cpp", `${folder}/turtle.cpp`);
  fs.copyFileSync("import_turtle/turtle.h", `${folder}/turtle.h`);
  fs.copyFileSync("import_turtle/json.hpp", `${folder}/json.hpp`);
  //erstelle file in anderen ordner
  fs.writeFileSync(`${folder}/Dockerfile`, Dockerfile);
  fs.writeFileSync(`${folder}/run.sh`, RunScript);

  let programm_output = "";

  try {
    // kompiliere den code im docker
    execSync(`cd ${folder} && docker build -t ${folder} .`);
    // Run the code
    programm_output = execSync(
      `cd ${folder} && docker run ${folder}`
    ).toString();
  } catch (err) {
    console.log(err);
  }

  //split bei programm:output jeweils bei dem zeichen in die Konstante
  //x=>x.trim() zeilenumbrüche raus
  const [compile, output, stringpath] = programm_output
    .split("--------")
    .map((x) => x.trim());
  const path = stringpath ? JSON.parse(stringpath) : [];

  // Delete the folder
  execSync(`rm -rf ${folder}`);

  //string wird zurück an die website geschickt
  res.send({ compile: compile, output: output, path: path });
});

//wartet im port bis website sich wieder bei mir meldet
app.listen(5236);
