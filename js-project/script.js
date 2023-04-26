

// Define a function that maps a physical pixel coordinate to a virtual pixel coordinate
// This function is based on the MapVisibleToMatrix() function from the https://github.com/hzeller/rpi-rgb-led-matrix/ repository.
// The function takes two parameters: x and y, which are the physical pixel coordinates.
// The function returns an object with two properties: x and y, which are the virtual pixel coordinates
function MapVisibleToMatrix(x, y) {
  const kPanelWidth = 64;
  const kPanelHeight = 32;

  let out_x = x;
  let out_y = y;

  // Calculate the panel index within a row
  const panelIdx = Math.floor(x / kPanelWidth);
  // Calculate the x coordinate within the current panel
  const xInPanel = x % kPanelWidth;

  // Check if it's an even or odd panel in the row
  if (panelIdx % 2 === 1) {
    // If it's an odd panel, we need to reverse the y direction
    out_y = kPanelHeight - 1 - y;
  }

  // Update the output x coordinate according to the panel index
  out_x = panelIdx * kPanelWidth + xInPanel;

  return { x: out_x, y: out_y };
}

// Define a function that simulates the output of the mapping 
// function for a given input.  This function takes two parameters: 
// input_id and cell_id, which are the ids of the input element and 
// the cell element.  The function reads the value of the input 
// element, which is a physical pixel coordinate.  The function calls
//  the mapping function to get the virtual pixel coordinate.  
// The function changes the background color of the cell element that 
// corresponds to the virtual pixel coordinate

function simulateOutput(input_id, cell_id) {
  const DELAY = 250;
  // Get the input element by id
  let input = document.getElementById(input_id);

  // Get the value of the input element, which is a string like "(x, y)"
  let value = input.value;

  // Parse the value to get the x and y coordinates as numbers
  let x = parseInt(value.slice(1, -1).split(", ")[0]);
  let y = parseInt(value.slice(1, -1).split(", ")[1]);
  
  // Call the mapping function to get the virtual pixel coordinate as an object
  let output = MapVisibleToMatrix(x, y);

  // Get the cell element by id
  let cell = document.getElementById(cell_id);

  // Set a timeout of 200ms and pass a function that changes the 
  // background color of the cell element to red as a callback
  setTimeout(function() {
    cell.style.backgroundColor = "green";
  }, DELAY );
}

// Define a function that simulates all the outputs for all the inputs
// This function loops through all the input elements and calls the 
// simulateOutput function for each one
function simulateAllOutputs() {

  // Loop through all the input elements by using a nested for loop
  for (let i = 0; i < 64; i++) {
    for (let j = 0; j < 64; j++) {
      // Create the ids of the input element and the cell element 
      // using string interpolation
      let input_id = `input-${i}-${j}`;
      let cell_id = `cell-${i}-${j}`;

      // Call the simulateOutput function with these ids as arguments
      simulateOutput(input_id, cell_id);
    }
  }
}

// Call the simulateAllOutputs function when the window loads
window.onload = simulateAllOutputs;

