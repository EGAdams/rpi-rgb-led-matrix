class StripMultiplexMapper {
    mapVisibleToInternal(x, y) {
      const matrixWidth = 64;
      const matrixHeight = 64;
      const tileWidth = 8;
      const tileHeight = 8;
      const tilesX = matrixWidth / tileWidth;
      const tilesY = matrixHeight / tileHeight;
      const evenVblock = Math.floor(y / (2 * tileHeight));
      const evenHblock = Math.floor(x / (2 * tileWidth));
      const evenTile = evenVblock * Math.floor(tilesX / 2) + evenHblock;
      const oddTile = Math.floor(tilesX * tilesY / 2) + evenTile;
      const tile = (x % 2) ? oddTile : evenTile;
      const posInTile = (Math.floor((y % (2 * tileHeight)) / 2) * tileWidth) + Math.floor(x / 2);
      const matrixX = tile * tileWidth + posInTile % tileWidth;
      const matrixY = Math.floor(posInTile / tileWidth);
      return { x: matrixX, y: matrixY };
    }
  }