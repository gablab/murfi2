%
%
function tcpiptest_server(output_port, output_host, timeout)

  if(nargin < 1)
    output_port = 15001;
  end
  
  if(nargin < 2)
    output_host = 'localhost';
  end
  
  if(nargin < 3)
    timeout = 20*1000;
  end
  
  tic

  import java.net.ServerSocket
  import java.net.InetSocketAddress
  import java.io.*

  address = InetSocketAddress(output_host, output_port);
  server_socket = ServerSocket;
  server_socket.bind(address);
  server_socket.setSoTimeout(timeout);
  server_socket.setReuseAddress(true);

  skt = server_socket.accept;
  output_stream = skt.getOutputStream;
  buffered_output_stream = BufferedOutputStream(output_stream);
  d_output_stream = DataOutputStream(buffered_output_stream);
  input_stream = skt.getInputStream;
  buffered_input_stream = BufferedInputStream(input_stream);
  d_input_stream = DataInputStream(buffered_input_stream);

  while(1)
    msg = [];
    while d_input_stream.available
      s = d_input_stream.read();
      msg = [msg char(s)];
    end
    if ~(isempty(msg)) disp(msg); toc; end
  end

  server_socket.close
  skt.close
  output_stream.close
  buffered_output_stream.close
  d_output_stream.close
  input_stream.close
  buffered_input_stream.close
  d_input_stream.close

return