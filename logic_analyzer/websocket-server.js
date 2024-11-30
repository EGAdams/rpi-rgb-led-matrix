const WebSocket = require('ws');
const Gpio = require('onoff').Gpio;

// Set up WebSocket server
const wss = new WebSocket.Server({ port: 8080 });

// Configure GPIO pins
const pins = [new Gpio(3, 'in', 'both'), new Gpio(2, 'in', 'both')]; // Add other pins as needed
let pinStates = Array(pins.length).fill(0); // Initialize pin states

// Function to read pin states
const readPinStates = () => {
    for (let i = 0; i < pins.length; i++) {
        pinStates[i] = pins[i].readSync(); // Read the current state of the pin
    }
    return pinStates;
};

// Emit GPIO data at regular intervals
setInterval(() => {
    const states = readPinStates(); // Get the current pin states
    const data = {
        timestamp: Date.now(),
        values: states, // Send array of pin states
    };

    console.log(data.timestamp + " " + data.values);

    // Send data to all connected clients
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(data));
        }
    });
}, 250); // Emit every 250ms

// Handle WebSocket connections
wss.on('connection', (ws) => {
    console.log('New client connected');

    // Log incoming messages from the client
    ws.on('message', (message) => {
        console.log(`Received message from client: ${message}`);
    });

    // Clean up when the client disconnects
    ws.on('close', () => {
        console.log('Client disconnected');
    });
});

// Cleanup GPIO on exit
process.on('SIGINT', () => {
    console.log('\nCleaning up GPIO...');
    pins.forEach((pin) => pin.unexport());
    process.exit();
});

console.log('WebSocket server is running on ws://localhost:8080');
