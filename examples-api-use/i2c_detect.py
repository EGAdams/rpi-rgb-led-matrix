import smbus
bus = smbus.SMBus(  1)
try:
    bus.read_byte( 0x68 )  # Replace 0x68 with your device's address
    print( "Device Found" )
except:
    print( "Device Not Found" )
    