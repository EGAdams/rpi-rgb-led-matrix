class CheckerboardMultiplexMapper {
    mapVisibleToInternal(x, y) {
      const shift = (x + (y % 2)) % 2;
      const matrixX = Math.floor(x / 2) * 4 + (x % 2) + shift * 2;
      const matrixY = y;
      return { x: matrixX, y: matrixY };
    }
  }  