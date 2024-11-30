import RPi.GPIO as GPIO
import time
import json
import socket

# Pin configuration
PINS = [14, 2]  # List of GPIO pins to read

GPIO.setmode(GPIO.BCM)
for pin in PINS:
    GPIO.setup(pin, GPIO.IN)

# Create a TCP socket to communicate with the WebSocket server
HOST = "192.168.84.39"  # WebSocket server running locally
PORT = 8080  # Port to match the WebSocket server

def send_to_server(data):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            s.sendall(json.dumps(data).encode('utf-8'))
    except ConnectionRefusedError:
        print("Connection to server failed. Is it running?")

try:
    while True:
        # print the values of the pins
        print( "\n" )
        print( "GPIO 14:: ", GPIO.input(14))
        print( "GPIO   2: ", GPIO.input(2))
        print( "\n" )

        pin_states = {f"pin_{pin}": GPIO.input(pin) for pin in PINS}
        send_to_server({"timestamp": time.time(), "states": pin_states})
        time.sleep(0.1)  # Adjust the delay as needed
except KeyboardInterrupt:
    GPIO.cleanup()

