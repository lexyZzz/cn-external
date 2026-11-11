
import socket

server = socket.socket()
server.bind(("localhost", 12345))
server.listen(1)

print("Server waiting...")
conn, addr = server.accept()
print("Client connected")

msg = conn.recv(1024).decode()
print("Client:", msg)

conn.send("Message received".encode())

conn.close()
server.close()
