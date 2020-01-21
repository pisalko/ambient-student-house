
#!/usr/bin/env python
"""
Startup:
    ./dummy-web-server.py -h
    ./dummy-web-server.py -l localhost -p 8000
Send a GET request:
    curl http://localhost:8000
Send a HEAD request:
    curl -I http://localhost:8000
Send a POST request:
    curl -d "foo=bar&bin=baz" http://localhost:8000
"""

import socket
import argparse
from http.server import HTTPServer, BaseHTTPRequestHandler


class S(BaseHTTPRequestHandler):

    post_data = "" 
    message = "" 
    listFridge = "Empty"
    

   
      
    def _set_headers(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()   
        
    def get_ip():
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            # doesn't even have to be reachable
            s.connect(('10.255.255.255', 1))
            IP = s.getsockname()[0]
        except:
            IP = '127.0.0.1'
        finally:
            s.close()
        return IP

    def do_HEAD(self):
        self.send_response(200, message = "valeee")
        self.send_header("Content-type", "text/html")
        self.end_headers()   

    def do_GET(self):
        global post_data #"Hi!"
        global message
        
        self._set_headers()
        #message = "Hello this is server!"
        print(message) 
        b = bytearray()
        b.extend(message) #Either a string that says on order yet
        self.wfile.write(b)

    def do_POST(self):
        global post_data
        global message
        global listFridge
        
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length) # <--- Gets the data itself
        #post_data = post_data[2:-1]
        print (post_data)
        #self._set_headers()   
        da = post_data.decode("utf-8")
        if da.startswith('='):
            listFridge = da[1:]
            self._set_headers()
            print(listFridge)

        elif da == ">":   
            self.send_response(200, listFridge)
            self.send_header("Content-type", "text/plain")  
            self.send_header("Content-length", len(listFridge))              
            self.end_headers()
            
            #self._set_headers()
            self.wfile.write(listFridge.encode("UTF-8"))

            print (listFridge)

        else:
            message = post_data
            self._set_headers()

        
        
        

def run(server_class=HTTPServer, handler_class=S, addr=S.get_ip(), port=42069):
    #addr = S.get_ip() #Gets localhost IP
    server_address = (addr, port)
    httpd = server_class(server_address, handler_class)

    print(f"Starting http server on {addr}:{port}")
    httpd.serve_forever()

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Run a simple HTTP server")
    parser.add_argument(
        "-l",
        "--listen",
        default=S.get_ip(),
        help="Specify the IP address on which the server listens",
    )
    parser.add_argument(
        "-p",
        "--port",
        type=int,
        default=42069,
        help="Specify the port on which the server listens",
    )
    args = parser.parse_args()
    run(addr=args.listen, port=args.port)

