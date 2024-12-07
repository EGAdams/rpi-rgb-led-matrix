import board
import busio
from adafruit_mcp230xx.mcp23017 import MCP23017


i2c = busio.I2C(board.SCL, board.SDA)
mcp = MCP23017(i2c)

pin0 = mcp.get_pin(0)
pin0.direction = Direction.OUTPUT

