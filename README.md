
# Tic-Tac-Toe Multiplayer Game (C++)

This project implements a multiplayer Tic-Tac-Toe game using TCP sockets for communication between the server and clients. The game allows two players to connect to the server and play against each other.

---

## Features

- **Server**:
  - Manages client connections.
  - Facilitates the Tic-Tac-Toe game logic.
  - Sends game updates and results to clients.
- **Client**:
  - Connects to the server.
  - Allows players to send moves and receive game state updates.
- **Multiplayer Support**:
  - Two players can connect and play simultaneously.

---

## File Overview

### 1. `server.cpp`
- Implements the server-side logic for the game.
- Features:
  - Creates and manages a listening socket.
  - Handles multiple clients using threads.
  - Implements the game rules and checks for winners.

### 2. `client.cpp`
- Implements the client-side functionality.
- Features:
  - Connects to the server via a specified IP address and port.
  - Sends player moves to the server.
  - Displays the game state and results.

---

## Prerequisites

- **Compiler**: A C++ compiler with support for WinSock (e.g., MinGW, Visual Studio).
- **Operating System**: Windows (for WinSock compatibility).

---

## Installation and Usage

1. **Clone the Repository**:
   ```bash
   git clone <this repository>
   cd <this repository>
   ```

2. **Compile the Code**:
   - Compile the server:
     ```bash
     g++ server.cpp -o server -lws2_32
     ```
   - Compile the client:
     ```bash
     g++ client.cpp -o client -lws2_32
     ```

3. **Run the Server**:
   - Start the server and specify a port:
     ```bash
     ./server
     ```
   - Enter a port number when prompted.

4. **Run the Clients**:
   - Launch the client and connect to the server:
     ```bash
     ./client
     ```
   - Enter the server's IP address, port, and your name.

---

## How to Play

1. Player 1 and Player 2 connect to the server.
2. Players take turns entering a number (1-9) corresponding to the grid position:
   ```
   1 | 2 | 3
   ---------
   4 | 5 | 6
   ---------
   7 | 8 | 9
   ```
3. The server checks for a winner or draw after each move and sends updates to both players.
4. The game ends when a player wins or the grid is full.

---

## License

This project is licensed under the [MIT License](LICENSE).
