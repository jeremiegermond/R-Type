# Communication Protocol between UDP Server and Clients

## Message Format
All messages are sent in plain text format and use the following format: `<command>:<param1>,<param2>,...,<paramN>`.

## Supported Commands

| Command | Description | Param1 | Param2 | Param3 | ... | ParamN |
| --- | --- | --- | --- | --- | --- | --- |
| `ping` | Client sends a "ping" to the server to indicate that it is still alive. | - | - | - | ... | - |
| `pong` | Server response to a "ping" message. | - | - | - | ... | - |
| `disconnect` | Client request to disconnect from the server. | - | - | - | ... | - |
| `move` | Client request to move in a certain direction. | `left`, `right`, `up`, or `down` | `x` coordinate | `y` coordinate | - | - |
| `shoot` | Client request to shoot. | - | - | - | ... | - |
| `id` | Server response to a new client, providing a unique identifier for the client. | `id` (integer) | - | - | ... | - |
| `full` | Server response to a new client when the server is full and cannot accept any new clients. | - | - | - | ... | - |
| `new` | Server broadcast to all clients, informing them of a new player that has joined the game. | `id` of new player | `x` coordinate of new player | `y` coordinate of new player | - | - |
| `spawn` | Server broadcast to all clients, informing them of a new enemy that has been spawned. | `id` of new enemy | `x` coordinate of new enemy | `y` coordinate of new enemy | `vx` velocity of new enemy | `vy` velocity of new enemy |
| `damaged` | Sent from server to all clients to inform about enemy/player damage. | `id` of damaged entity | `health` of damaged entity | - | ... | - |
| `dead`    | Sent from server to all clients to inform about enemy death. | `id` of dead enemy | - | - | ... | - |
| `del`     | Sent from server to all clients to inform about player disconnection. | `id` of disconnected player | - | - | ... | - |
