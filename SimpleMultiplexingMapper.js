class SimpleMultiplexingMapper {
    mapVisibleToInternal(x, y) {
      const newY = Math.floor(y / 2) + (y % 2) * 32;
      return { x, y: newY };
    }
  }