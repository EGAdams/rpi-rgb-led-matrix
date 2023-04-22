That sounds like an interesting project. I will try to help you with the Python script, but I cannot guarantee that it will work as expected. You may need to modify it or debug it yourself. Here is the script:

# This is a Python script that will create a javascript project that simulates a HUB75 64x64 LED panel.

# Import the os module for creating directories and files
import os

# Create a directory called "js-project" in the current working directory
os.mkdir("js-project")

# Change the current working directory to "js-project"
os.chdir("js-project")

# Create a file called "index.html" with the following content
html_content = """
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>HUB75 Simulator</title>
  <style>
    /* Define some styles for the grid and the cells */
    .grid {
      display: grid;
      grid-template-columns: repeat(64, 10px);
      grid-template-rows: repeat(64, 10px);
      gap: 1px;
      margin: 10px;
    }

    .cell {
      width: 10px;
      height: 10px;
      background-color: black;
    }
  </style>
</head>
<body>
  <h1>HUB75 Simulator</h1>
  <p>This is a javascript project that simulates a HUB75 64x64 LED panel.</p>
  <p>The grid below represents the virtual pixels that we can see.</p>
  <p>The inputs below represent the physical pixels that are mapped to the virtual pixels.</p>
  <div class="grid" id="grid">
    <!-- Create 64x64 cells with ids from "cell-0-0" to "cell-63-63" -->
    <!-- Use a nested for loop to generate the cells -->
    <!-- Use string interpolation to create the ids -->
    <!-- Use document.write() to write the html elements -->
    <script>
      for (let i = 0; i < 64; i++) {
        for (let j = 0; j < 64; j++) {
          let id = `cell-${i}-${j}`;
          document.write(`<div class="cell" id="${id}"></div>`);
        }
      }
    </script>
  </div>
  <div id="inputs">
    <!-- Create a set of inputs from (0, 0) to (63, 63) for the simulation test -->
    <!-- Use a nested for loop to generate the inputs -->
    <!-- Use string interpolation to create the ids and values -->
    <!-- Use document.write() to write the html elements -->
    <script>
      for (let i = 0; i < 64; i++) {
        for (let j = 0; j < 64; j++) {
          let id = `input-${i}-${j}`;
          let value = `(${i}, ${j})`;
          document.write(`<input type="text" id="${id}" value="${value}" readonly>`);
        }
        document.write("<br>"); // Add a line break after each row
      }
    </script>
  </div>
  <script src="script.js"></script> <!-- Link to the external javascript file -->
</body>
</html>
"""

# Write the html content to the file
with open("index.html", "w") as f:
  f.write(html_content)

# Create a file called "script.js" with the following content
js_content = """
// This is a javascript file that simulates the mapping of physical pixels to virtual pixels

// Define a function that maps a physical pixel coordinate to a virtual pixel coordinate
// This function is based on the MapVisibleToMatrix() function from https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/lib/transformer.cc
// The function takes two parameters: x and y, which are the physical pixel coordinates
// The function returns an object with two properties: x and y, which are the virtual pixel coordinates
function MapVisibleToMatrix(x, y) {
  
  // Define some constants for the panel size and configuration
  const kPanelWidth = 64;
  const kPanelHeight = 32;
  
  // Define some variables for the output coordinates
  let out_x = x;
  let out_y = y;

  // Apply some transformations based on the physical pixel coordinates
  // These transformations are explained in detail in https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/lib/transformer.cc