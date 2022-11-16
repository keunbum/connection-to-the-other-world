# How To Execute

## Compile

### Server Side
```
$ gcc hello_server.c -o hserver
```

### Client Side
```
$ gcc hello_client.c -o hclient
```

## Execute
You should run the server side first.

### Server Side
```bash
$ ./hserver 9190
# Of course, other ports can be used.
```

### Client Side
```bash
$ ./hclient 127.0.0.1 9190
# A different ip address can be used.
```
