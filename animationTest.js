async function animateMapping() {
    for (let y = 0; y < 64; y++) {
      for (let x = 0; x < 64; x++) {
        document.getElementById("x-coordinate").value = x;
        document.getElementById("y-coordinate").value = y;
        updateVisualization();
        await new Promise(resolve => setTimeout(resolve, 5));
      }
    }
  }
  
  function setupAnimateMappingButton() {
    const button = document.createElement("button");
    button.type = "button";
    button.id = "animate-mapping-button";
    button.textContent = "Animate Mapping";
    document.getElementById("input-form").appendChild(button);
  
    const animateMappingButton = document.getElementById("animate-mapping-button");
    animateMappingButton.addEventListener("click", animateMapping);
  }
  
  setupAnimateMappingButton();
  