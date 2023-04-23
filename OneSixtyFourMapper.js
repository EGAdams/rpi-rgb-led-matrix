class OneSixtyFourMapper {
    constructor( matrixWidth, matrixHeight ) {
        this.matrixWidth = matrixWidth;
        this.matrixHeight = matrixHeight;
        this.parallel = 1;
      }
  
    getName() {
      return "164-mapper";
    }
  
    setParameters(chain, parallel, param) {
      this.parallel = parallel;
      return true;
    }
  
    getSizeMapping(matrixWidth, matrixHeight) {
      const visibleWidth = (matrixWidth / 64) * 32;
      const visibleHeight = 2 * matrixHeight;
  
      if (matrixHeight % this.parallel !== 0) {
        console.error(
          `${this.getName()} For parallel=${this.parallel} we would expect the height=${matrixHeight} to be divisible by ${this.parallel} ??`
        );
        return false;
      }
  
      return { visibleWidth, visibleHeight };
    }
  
    mapVisibleToInternal(x, y) {
        const panelWidth = 64;
        const panelHeight = 64;
        const panelsPerRow = this.matrixWidth / panelWidth;
      
        const panelIndexX = Math.floor(x / panelWidth);
        const panelIndexY = Math.floor(y / panelHeight);
      
        if (x % 64 === 0 && y % 64 === 0) {
          console.log(`panelIndexX: ${panelIndexX}`);
          console.log(`panelIndexY: ${panelIndexY}`);
        }
      
        const panelX = panelIndexX * panelWidth;
        const panelY = panelIndexY * panelHeight;
      
        let internalX = x % panelWidth;
        let internalY = y % panelHeight;
      
        if (internalY >= 32) {
          internalX += 32;
          internalY -= 32;
        }
      
        if (x % 64 === 0 && y % 64 === 0) {
          console.log(`internalX before adjustment: ${internalX}`);
          console.log(`internalY before adjustment: ${internalY}`);
        }
      
        internalX += panelIndexX * panelWidth;

        // internalY += panelIndexY * panelHeight;
        // internalY %= this.matrixHeight; // Ensures the y-coordinate remains within the matrix_height

        internalY = (internalY + panelIndexY * panelHeight) % this.matrixHeight;
      
        if (x % 64 === 0 && y % 64 === 0) {
          console.log(`internalX after adjustment: ${internalX}`);
          console.log(`internalY after adjustment: ${internalY}`);
        }
      
        return { x: internalX, y: internalY };
      }      
  }
  