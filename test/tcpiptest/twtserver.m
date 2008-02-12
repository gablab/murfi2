
tic

import java.net.ServerSocket
import java.io.*

output_port = 15001;

for i = 1:5
    server_socket = ServerSocket(output_port);
    server_socket.setSoTimeout(2000 * 1000);

    skt = server_socket.accept;
    out = PrintWriter(skt.getOutputStream(), true);

    out.println(['Hello World #' num2str(i)]);
    out.close;
    skt.close;
    server_socket.close;
    toc
end

