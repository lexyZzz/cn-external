
import socket

client = socket.socket()
client.connect(("localhost", 12345))

client.send("Hello Server".encode())

msg = client.recv(1024).decode()
print("Server:", msg)

client.close()
