# tcp-support

TCP client/server app for multi level support, built for a 2nd year networking project at TELECOM Nancy

## Getting Started

### Prerequisites

This project requires the use of clang and make. If you don't have them, you can install them with the following commands:

```
sudo apt-get install clang
sudo apt-get install make
```

### Setting up the project 

Once you have the project, you can compile it with make using the following command:

```
make clean && make
``` 

### Setting up the server

To start the server, you can use the following command:

```
./server <port>
```

### Setting up the client

To start the client, you can use the following command:

```
./client <server_ip> <server_port>
```

Easiest way to test the app is to run the server on your machine, therefore the `<server_ip>` will be `127.0.0.1` 

### Using the app

Once you have the server and the client running, you'll be asked to enter a username and what level are you (client, level 2 agent, level 3 agent)

#### Client

If you're a client, you'll be asked to enter a message to send to the server. Then a BOT will answer you. You can then continue asking the BOT questions. 
If the BOT cannot help you, he'll tell you to type in `superieur` to talk to a level 2 agent. 
When talking to a level 2 agent, you'll be talking to a real human (if a level 2 agent is connected). You can then ask him questions. If his answers are not enough, you can then type `superieur` again to talk to a level 3 agent (if there is one connected and available). 

##### BOT answers 

The BOT is able to answer if your message contains one of the following words:
-  `box`
-  `internet`
-  `television`


#### Level 2/3 agent

You'll have to wait until a client contacts you. Then you'll be able to exchange messages with him. If you don't know the answer to his question, you can tell him to type `superieur` to talk to a level 3 agent (if there is one connected and available).

